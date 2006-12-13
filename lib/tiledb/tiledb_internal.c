#include "tiledb_file_locking.h"
#include "tiledb_file_io.h"
#include "tiledb_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

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
	for (i=0; i<sizeof(entry->data_elements)/sizeof(tiledb_index_entry_ptr_v0); i++)
	{
		entry->data_elements[i].offset = 0;
		entry->data_elements[i].last_modification_time = 0;
		entry->data_elements[i].size = 0;
	}
	for (i=0; i<sizeof(entry->child_entries)/sizeof(uint32_t); i++)
	{
		entry->child_entries[i] = 0;
	}
}

uint32_t get_next_offset_v0(tiledb_index_entry_v0 *entry, unsigned int start_level, unsigned int x, unsigned int y, unsigned int level)
{
	unsigned int offset;
	unsigned int current_node_x = 0;
	unsigned int current_node_y = 0;
	unsigned int current_node_level = start_level;
	unsigned int data_element_index = 0;
	for (; current_node_level<LEVELS_PER_PYRAMID+start_level && current_node_level < level; current_node_level++)
	{
		if (current_node_level <LEVELS_PER_PYRAMID+start_level-1)
		{
			data_element_index += 1<<(2*(current_node_level-start_level));
		}
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
		if (((current_node_level+1) % LEVELS_PER_PYRAMID) == 0) {
			offset = current_node_x + current_node_y*(1<<(LEVELS_PER_PYRAMID));
			printf("i:%d->", offset);
			//printf("continue at child_entries index=%d\n", offset);
			if (offset >= 1024) {
				db_error("index out of bounds");
				exit(-1);
			}
			return offset;
		}
	}
	offset = data_element_index + current_node_x + current_node_y*(1<<(current_node_level-start_level));
	printf("d:%d\n", offset);
	//printf("index node found, using data_element index=%d\n", offset);
	if (offset >= 1+4+16+64+256) {
		db_error("index out of bounds");
		exit(-1);
	}
	return offset;
}

tiledb_error tiledb_get_index_entry_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, tiledb_index_entry_v0 *index_entry, unsigned int *locked_offset, unsigned int *data_element_index, int create_if_not_exists) {
	unsigned int current_node_level = 0;
	unsigned int offset = 0;
	unsigned int file_offset = sizeof(tiledb_index_header);
	printf("i:0->");
	unsigned int old_file_offset = 0;
	while (current_node_level <= level) {
		//file_offset is offset in bytes in index file
		if (file_offset == 0 && !create_if_not_exists) {
			//next index entry does not exist yet, and should not be created

			//old_file_offset is still lock
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			//all index entries are unlocked
			*locked_offset = 0;
			return TILEDB_INDEX_ENTRY_NOT_EXISTS;
		} else if (file_offset == 0) { //next index entry does not exist yet
			//create new index entry
			acquire_index_lock(db_handle);

			file_offset = tiledb_get_file_size(db_handle->index_file);

			acquire_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));

			//store new index entry
			tiledb_index_entry_v0 newIndex;
			init_index_entry_v0(&newIndex);
			store_data_to_file(db_handle->index_file, file_offset, &newIndex, sizeof(newIndex));

			//update parent index entry
			index_entry->child_entries[offset] = file_offset;
			store_data_to_file(db_handle->index_file, old_file_offset, index_entry, sizeof(tiledb_index_entry_v0)); //TODO Don't save total entry

			release_index_lock(db_handle);
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			//copy newIndex to current entry
			memcpy(index_entry, &newIndex, sizeof(tiledb_index_entry_v0));
		} else {
			//read child index entry from file
			acquire_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
			release_index_entry_lock(db_handle, old_file_offset, sizeof(tiledb_index_entry_v0));

			if (read_data_to_buffer(db_handle->index_file, file_offset, index_entry, sizeof(tiledb_index_entry_v0)) != sizeof(tiledb_index_entry_v0)) {
				release_index_entry_lock(db_handle, file_offset, sizeof(tiledb_index_entry_v0));
				return TILEDB_SYSCALL_ERROR;
			}
		}
		offset = get_next_offset_v0(index_entry, current_node_level, x, y, level);
		old_file_offset = file_offset;
		file_offset = index_entry->child_entries[offset];
		current_node_level += LEVELS_PER_PYRAMID;
	}
	// index_entry holds index entry for x/y/level
	// index entry (locked_offset) is locked
	// offset holds index for x/y/level in the index_entry->data_element field
	*locked_offset = old_file_offset;
	*data_element_index = offset;
	return TILEDB_OK;
}

tiledb_error tiledb_put_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, char* data, size_t size) {
	tiledb_index_entry_v0 entry;
	unsigned int locked_offset;
	unsigned int data_element_index;

	tiledb_error result;
	if ((result = tiledb_get_index_entry_v0(db_handle, x, y, level, &entry, &locked_offset, &data_element_index, 1)) != TILEDB_OK) {
		//index entry for x/y/level could not be loaded
		return result;
	}

	//entry holds index for x/y/level and entry is locked(locked_offset)
	//data_element_index holds index for x/y/level in the index_entry->data_element field

	printf("locked_offset=%d\n", locked_offset);

	//lock data file for writing at the end
	acquire_data_lock(db_handle);

	//allocate memory in data file
	size_t data_file_offset = tiledb_get_file_size(db_handle->data_file);

	store_data_to_file(db_handle->data_file, data_file_offset, data, size);

	if (entry.data_elements[data_element_index].size > 0) { // data element is used
		//TODO mark old data as invalid
		printf("TODO mark old data as invalid\n");
	}

	release_data_lock(db_handle);


	//update index
	entry.data_elements[data_element_index].offset = data_file_offset;
	entry.data_elements[data_element_index].last_modification_time = 0; //TODO include time
	entry.data_elements[data_element_index].size = size;
	store_data_to_file(db_handle->index_file, locked_offset, &entry, sizeof(entry)); //TODO Don't save total entry

	release_index_entry_lock(db_handle, locked_offset, sizeof(tiledb_index_entry_v0));
	return TILEDB_OK;
}

tiledb_error tiledb_get_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level) {
	tiledb_index_entry_v0 entry;
	unsigned int locked_offset;
	unsigned int data_element_index;

	tiledb_error result;
	if ((result = tiledb_get_index_entry_v0(db_handle, x, y, level, &entry, &locked_offset, &data_element_index, 0)) != TILEDB_OK) {
		//index entry for x/y/level could not be loaded
		printf("element not found or error occurred\n");
		return result;
	}

	//entry holds index for x/y/level and entry is locked(locked_offset)
	//data_element_index holds index for x/y/level in the index_entry->data_element field

	if (entry.data_elements[data_element_index].size > 0) { // data element found and data is stored
		//free old loaded data
		if (db_handle->current_data)
			free(db_handle->current_data);

		db_handle->current_size = entry.data_elements[data_element_index].size;
		db_handle->current_data = (unsigned char *)malloc(db_handle->current_size);

		if (read_data_to_buffer(db_handle->data_file, entry.data_elements[data_element_index].offset, db_handle->current_data, db_handle->current_size) == db_handle->current_size) {
			release_index_entry_lock(db_handle, locked_offset, sizeof(tiledb_index_entry_v0));
			printf("tile found\n");
			return TILEDB_OK;
		}
		release_index_entry_lock(db_handle, locked_offset, sizeof(tiledb_index_entry_v0));
		printf("read error\n");
		return TILEDB_SYSCALL_ERROR;
	} else {
		release_index_entry_lock(db_handle, locked_offset, sizeof(tiledb_index_entry_v0));
		printf("element not found\n");
		return TILEDB_NOT_FOUND;
	}
}
