#include "tiledb_file_locking.h"
#include "tiledb_file_io.h"
#include "tiledb_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

void create_new_db(DB_Handle* db_handle) {
	//write header
	tiledb_index_header header;
	header.version = 0;
	store_data_to_file(db_handle->index_file, 0, &header, sizeof(tiledb_index_header));

	//write root index entry
	tiledb_index_entry_v0 index_entry;
	init_index_entry_v0(&index_entry);
	store_data_to_file(db_handle->index_file, sizeof(tiledb_index_header), &index_entry, sizeof(tiledb_index_entry_v0));
}

void init_index_entry_v0(tiledb_index_entry_v0 *entry)
{
	int i;
	entry->dirty = 0;
	entry->deleted = 0;
	entry->create_time = 0; //TODO
	entry->last_access_time = 0; //TODO
	for (i=0; i<sizeof(entry->data_elements)/sizeof(tiledb_index_entry_ptr_v0); i++)
	{
		entry->data_elements[i].used = 0;
		entry->data_elements[i].offset = 0;
		entry->data_elements[i].last_modification_time = 0;
		entry->data_elements[i].size = 0;
	}
	for (i=0; i<sizeof(entry->child_entries)/sizeof(uint32_t); i++)
	{
		entry->child_entries[i] = 0;
	}
}

uint32_t get_next_offset_v0(tiledb_index_entry_v0 *entry, uint32_t start_level, uint32_t x, uint32_t y, uint32_t level)
{
	uint32_t offset;
	uint32_t current_node_x = 0;
	uint32_t current_node_y = 0;
	uint32_t current_node_level = start_level;
	uint32_t data_element_index = 0;
	for (; current_node_level<LEVELS_PER_PYRAMID+start_level && current_node_level < level; current_node_level++)
	{
		if (((current_node_level+1) % LEVELS_PER_PYRAMID) == 0) {
			offset = current_node_x + current_node_y*(1<<(LEVELS_PER_PYRAMID));
			printf("continue at child_entries index=%d\n", offset);
			return offset;
		}
		if (current_node_level <LEVELS_PER_PYRAMID+start_level-1)
		{
			data_element_index += 1<<(2*(current_node_level-start_level));
			uint32_t mask = 1 << (level-current_node_level-1);
			if ((x & mask) == 0) {
				current_node_x *= 2;
			} else {
				current_node_x *= 2;
				current_node_x++;
			}
			if ((y & mask) == 0) {
				current_node_y *= 2;
			} else {
				current_node_y *= 2;
				current_node_y++;
			}
		}
	}
	offset = data_element_index + x + y*(1<<(current_node_level-start_level+1));
	printf("node found, using data_element index=%d\n", offset);
	return offset;
}

tiledb_error tiledb_put_v0(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level, char* data, uint32_t size) {
	tiledb_index_entry_v0 entry;

	uint32_t current_node_level = 0;
	uint32_t offset = 0;
	uint32_t file_offset = sizeof(tiledb_index_header);
	uint32_t old_file_offset = 0;
	while (current_node_level <= level) {
		//file_offset is offset in bytes in index file
		if (file_offset == 0) { //next index entry does not exist yet
			//create new index entry
			acquire_index_lock(db_handle);

			struct stat file_status;
			fstat(db_handle->index_file, &file_status);
			file_offset = file_status.st_size;

			acquire_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));

			//store new index entry
			tiledb_index_entry_v0 newIndex;
			init_index_entry_v0(&newIndex);
			store_data_to_file(db_handle->index_file, file_offset, &newIndex, sizeof(newIndex));

			//update old index entry
			entry.child_entries[offset] = file_offset;
			store_data_to_file(db_handle->index_file, old_file_offset, &entry, sizeof(entry)); //TODO Don't save total entry

			release_index_lock(db_handle);
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			//copy newIndex to current entry
			memcpy(&entry, &newIndex, sizeof(entry));
		} else {
			//read child index entry from file
			acquire_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			if (read_data_to_buffer(db_handle->index_file, file_offset, &entry, sizeof(entry)) != sizeof(entry)) {
				release_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
				return TILEDB_SYSCALL_ERROR;
			}
		}
		offset = get_next_offset_v0(&entry, current_node_level, x, y, level);
		old_file_offset = file_offset;
		file_offset = entry.child_entries[offset];
		current_node_level += LEVELS_PER_PYRAMID;
	}
	//offset is index in data_elements
	//old_offset is still locked
	acquire_data_lock(db_handle);

	//allocate memory in data file
	struct stat file_status;
	fstat(db_handle->data_file, &file_status);
	uint32_t data_file_offset = file_status.st_size;

	store_data_to_file(db_handle->data_file, data_file_offset, data, size);

	if (entry.data_elements[offset].used == 1) { // data element is used
		//TODO mark old data as invalid
		printf("TODO mark old data as invalid\n");
	}

	release_data_lock(db_handle);

	//update index
	entry.data_elements[offset].used = 1;
	entry.data_elements[offset].offset = data_file_offset;
	entry.data_elements[offset].last_modification_time = 0; //TODO include time
	entry.data_elements[offset].size = size;
	store_data_to_file(db_handle->index_file, old_file_offset, &entry, sizeof(entry)); //TODO Don't save total entry

	release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));
	return TILEDB_OK;
}

tiledb_error tiledb_get_v0(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level) {
	tiledb_index_entry_v0 entry;
	uint32_t current_node_level = 0;
	uint32_t offset = 0;
	uint32_t file_offset = sizeof(tiledb_index_header);
	uint32_t old_file_offset = 0;
	while (current_node_level <= level) {
		//file_offset is offset in bytes in index file
		if (file_offset == 0) { //next index entry does not exist yet
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));
			return TILEDB_NOT_FOUND;
		} else {
			//read child index entry from file
			acquire_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			if (read_data_to_buffer(db_handle->index_file, file_offset, &entry, sizeof(entry)) != sizeof(entry)) {
				release_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
				return TILEDB_SYSCALL_ERROR;
			}
		}
		offset = get_next_offset_v0(&entry, current_node_level, x, y, level);
		old_file_offset = file_offset;
		file_offset = entry.child_entries[offset];
		current_node_level += LEVELS_PER_PYRAMID;
	}
	//offset is index in data_elements
	//old_offset is still locked

	if (entry.data_elements[offset].used == 1) { // data element found
		if (db_handle->current_data)
			free(db_handle->current_data);

		db_handle->current_size = entry.data_elements[offset].size;
		db_handle->current_data = (unsigned char *)malloc(db_handle->current_size);

		if (read_data_to_buffer(db_handle->data_file, entry.data_elements[offset].offset, db_handle->current_data, db_handle->current_size) == db_handle->current_size) {
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));
			return TILEDB_OK;
		}
		return TILEDB_SYSCALL_ERROR;
	} else {
		printf("element not found\n");
		release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));
		return TILEDB_NOT_FOUND;
	}
}
