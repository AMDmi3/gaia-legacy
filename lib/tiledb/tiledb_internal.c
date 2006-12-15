#include "tiledb_file_locking.h"
#include "tiledb_file_io.h"
#include "tiledb_internal.h"
#include "tiledb_endian.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#if LOG_DB
#define db_printf(...) printf(__VA_ARGS__)
#else
#define db_printf(...) while(0){}
#endif

void tiledb_create_new_db_v0(DB_Handle* db_handle) {
	db_handle->version = 0;
	db_handle->index_page_size = sizeof(tiledb_index_entry_v0);
	db_handle->db_endianess = tiledb_get_endian();
	db_handle->pc_endianess = tiledb_get_endian();

	//write header
	tiledb_index_header header;
	header.version = (db_handle->pc_endianess == ENDIANESS_LITTLE)? 0: SWAPBYTES_32(0);
	header.endianess = (db_handle->pc_endianess == ENDIANESS_LITTLE)? db_handle->db_endianess: SWAPBYTES_32(db_handle->db_endianess);
	tiledb_store_data_to_file(db_handle->index_file, 0, &header, sizeof(tiledb_index_header));

	//write root index entry
	tiledb_index_entry_v0 index_entry;
	tiledb_init_index_entry_v0(db_handle, &index_entry, 0);
	tiledb_store_index_page(db_handle, 0, &index_entry);
}

void tiledb_init_index_entry_v0(DB_Handle* db_handle, tiledb_index_entry_v0 *entry, tiledb_index_page_ref parent)
{
	int i;
	entry->parent_index_page = tiledb_switch_int(db_handle, parent);

	for (i=0; i<sizeof(entry->objects)/sizeof(tiledb_index_object_v0); i++)
	{
		entry->objects[i].offset = tiledb_switch_int(db_handle, 0);
		entry->objects[i].checksum = tiledb_switch_int(db_handle, 0);
		entry->objects[i].size = tiledb_switch_int(db_handle, 0);
	}
	for (i=0; i<sizeof(entry->child_entries)/sizeof(uint32_t); i++)
	{
		entry->child_entries[i] = tiledb_switch_int(db_handle, -1);
	}
}

tiledb_array_index tiledb_get_next_offset_v0(unsigned int start_level, unsigned int x, unsigned int y, unsigned int level)
{
	tiledb_array_index offset = -1;
	unsigned int current_node_x = 0;
	unsigned int current_node_y = 0;
	unsigned int current_node_level = start_level;
	unsigned int objects_index  = 0;
	for (; current_node_level<LEVELS_PER_PYRAMID+start_level && current_node_level < level; current_node_level++)
	{
		if (current_node_level <LEVELS_PER_PYRAMID+start_level-1)
		{
			objects_index  += 1<<(2*(current_node_level-start_level));
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
			//printf("i:%d->", offset);
			//printf("continue at child_entries index=%d\n", offset);
			if (offset >= 1024) {
				db_error("index out of bounds");
				exit(-1);
			}
			return offset;
		}
	}
	offset = objects_index  + current_node_x + current_node_y*(1<<(current_node_level-start_level));
	//printf("d:%d\n", offset);
	//printf("index node found, using objects index=%d\n", offset);
	if (offset >= 1+4+16+64+256) {
		db_error("index out of bounds");
		exit(-1);
	}
	return offset;
}

tiledb_error tiledb_get_index_page_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, tiledb_index_entry_v0 *index_entry, tiledb_index_page_ref *locked_page, tiledb_array_index *objects_index, int create_if_not_exists) {
	unsigned int current_node_level = 0;
	tiledb_array_index offset = 0;
	tiledb_index_page_ref index_page_ref = 0;
	//printf("i:0->");
	tiledb_index_page_ref parent_index_page_ref = -1;
	while (current_node_level <= level) {
		//file_offset is offset in bytes in index file
		if (index_page_ref == -1 && !create_if_not_exists) {
			//next index entry does not exist yet, and should not be created

			//old_file_offset is still lock
			release_index_page_lock(db_handle, parent_index_page_ref);

			//all index entries are unlocked
			*locked_page = -1;
			return TILEDB_INDEX_ENTRY_NOT_EXISTS;
		} else if (index_page_ref == -1) { //next index entry does not exist yet
			//create new index entry
			acquire_index_lock(db_handle);

			index_page_ref = tiledb_get_free_index_page(db_handle);

			acquire_index_page_lock(db_handle, index_page_ref);

			//store new index entry
			tiledb_index_entry_v0 new_index_entry;
			tiledb_init_index_entry_v0(db_handle, &new_index_entry, parent_index_page_ref);
			tiledb_store_index_page(db_handle, index_page_ref, &new_index_entry);

			//update parent index entry
			index_entry->child_entries[offset] = tiledb_switch_int(db_handle, index_page_ref);
			tiledb_store_index_page(db_handle, parent_index_page_ref, index_entry); //TODO Don't save total entry

			release_index_lock(db_handle);
			release_index_page_lock(db_handle, parent_index_page_ref);

			//copy newIndex to current entry
			memcpy(index_entry, &new_index_entry, sizeof(tiledb_index_entry_v0));
		} else {
			//read child index entry from file
			acquire_index_page_lock(db_handle, index_page_ref);
			release_index_page_lock(db_handle, parent_index_page_ref);

			if (tiledb_read_index_page(db_handle, index_page_ref, index_entry) != TILEDB_OK) {
				release_index_page_lock(db_handle, index_page_ref);
				db_error("on reading index page");
				return TILEDB_SYSCALL_ERROR;
			}
		}
		offset = tiledb_get_next_offset_v0(current_node_level, x, y, level);
		parent_index_page_ref = index_page_ref;
		index_page_ref = tiledb_switch_int(db_handle, index_entry->child_entries[offset]);
		current_node_level += LEVELS_PER_PYRAMID;
	}
	// index_entry holds index entry for x/y/level
	// index entry (locked_page) is locked
	// offset holds index for x/y/level in the index_entry->objects field
	*locked_page = parent_index_page_ref;
	*objects_index = offset;
	return TILEDB_OK;
}

tiledb_error tiledb_put_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, char* data, size_t size) {
	tiledb_index_entry_v0 entry;
	tiledb_index_page_ref locked_page;
	tiledb_array_index objects_index;

	tiledb_error result;
	if ((result = tiledb_get_index_page_v0(db_handle, x, y, level, &entry, &locked_page, &objects_index, 1)) != TILEDB_OK) {
		//index entry for x/y/level could not be loaded
		return result;
	}

	//entry holds index for x/y/level and entry is locked(locked_page)
	//objects_index  holds index for x/y/level in the index_entry->objects field

	//lock data file for writing at the end
	acquire_data_lock(db_handle);

	//allocate memory in data file
	size_t data_file_offset = tiledb_get_file_size(db_handle->data_file);

	// store data meta informations
	int data_to_store_size = sizeof(tiledb_data_entry_v0)+size;
	// data is stored in pages
	data_to_store_size = (data_to_store_size / db_handle->data_page_size + 1) * db_handle->data_page_size;

	char data_to_store[data_to_store_size];
	tiledb_data_entry_v0 *data_entry = (tiledb_data_entry_v0 *)&data_to_store;
	data_entry->mtime = tiledb_switch_int(db_handle, (uint32_t)time(NULL));
	data_entry->index_page = tiledb_switch_int(db_handle, locked_page);
	data_entry->index = tiledb_switch_int(db_handle, objects_index);
	data_entry->size = tiledb_switch_int(db_handle, size);

	memcpy(((char *)&data_to_store)+sizeof(tiledb_data_entry_v0), data, size);
	//store data_entry and data with same syscall
	if (tiledb_store_data_to_file(db_handle->data_file, data_file_offset, data_to_store, sizeof(data_to_store)) != TILEDB_OK) {
		release_data_lock(db_handle);
		release_index_page_lock(db_handle, locked_page);

		//new data could not be written -> db is in consistent state
		return TILEDB_SYSCALL_ERROR;
	}

	//save old data, mark as invalid data after updating index
	int old_data_size = tiledb_switch_int(db_handle, entry.objects[objects_index].size);
	int old_data_offset = tiledb_switch_int(db_handle, entry.objects[objects_index].offset);

	//update index
	entry.objects[objects_index].offset = tiledb_switch_int(db_handle, data_file_offset);
	entry.objects[objects_index].checksum = tiledb_switch_int(db_handle, 0); //TODO set checksum
	entry.objects[objects_index].size = tiledb_switch_int(db_handle, size);
	if (tiledb_store_index_page(db_handle, locked_page, &entry) != TILEDB_OK) {//TODO Don't save total entry
		release_data_lock(db_handle);
		release_index_page_lock(db_handle, locked_page);

		//index could not be updated, still using old data, new data is garbage
		//garbage collection:
		//(data_entry->index_page->objects[data_entry->index].offset != data_entry) => garbage
		return TILEDB_SYSCALL_ERROR;
	}

	if (old_data_size > 0) {
		// read meta informations of old data
		tiledb_data_entry_v0 data_entry;
		if (tiledb_read_data_to_buffer(db_handle->data_file, old_data_offset, &data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
			release_data_lock(db_handle);
			release_index_page_lock(db_handle, locked_page);
			db_printf("read error\n");

			//old data is garbage
			//garbage collection:
			//(data_entry->index_page->objects[data_entry->index].offset != data_entry) => garbage
			return TILEDB_SYSCALL_ERROR;
		}

		//mark old data as invalid
		data_entry.mtime = tiledb_switch_int(db_handle, 0);
		data_entry.index_page = tiledb_switch_int(db_handle, -1);
		data_entry.index = tiledb_switch_int(db_handle, -1);
		if (tiledb_store_data_to_file(db_handle->data_file, old_data_offset, &data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
			release_data_lock(db_handle);
			release_index_page_lock(db_handle, locked_page);
			db_printf("read error\n");

			//old data is garbage
			//garbage collection:
			//(data_entry->index_page->objects[data_entry->index].offset != data_entry) => garbage
			return TILEDB_SYSCALL_ERROR;
		}
	}
	release_data_lock(db_handle);
	release_index_page_lock(db_handle, locked_page);
	return TILEDB_OK;
}

tiledb_error tiledb_get_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level) {
	tiledb_index_entry_v0 entry;
	tiledb_index_page_ref locked_page;
	tiledb_array_index objects_index;

	tiledb_error result;
	if ((result = tiledb_get_index_page_v0(db_handle, x, y, level, &entry, &locked_page, &objects_index, 0)) != TILEDB_OK) {
		//index entry for x/y/level could not be loaded
		db_printf("element not found or error occurred\n");
		return result;
	}

	//entry holds index for x/y/level and entry is locked(locked_page)
	//objects_index holds index for x/y/level in the index_entry->objects field

	if (entry.objects[objects_index].size == tiledb_switch_int(db_handle, 0)) {
		// data element not found or nodata is stored
		release_index_page_lock(db_handle, locked_page);
		db_printf("element not found\n");
		return TILEDB_NOT_FOUND;
	}

	//free old loaded data
	if (db_handle->current_data)
		free(db_handle->current_data);

	db_handle->current_size = tiledb_switch_int(db_handle, entry.objects[objects_index].size);
	db_handle->current_data = (unsigned char *)malloc(db_handle->current_size);

	off_t offset = tiledb_switch_int(db_handle, entry.objects[objects_index].offset);

	//read data meta informations
	//tiledb_data_entry_v0 data_entry;
	//if (tiledb_read_data_to_buffer(db_handle->data_file, offset, &data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
	//	release_index_page_lock(db_handle, locked_page);
	//	db_printf("read error\n");
	//	return TILEDB_SYSCALL_ERROR;
	//}

	offset += sizeof(tiledb_data_entry_v0);
	if (tiledb_read_data_to_buffer(db_handle->data_file, offset, db_handle->current_data, db_handle->current_size) != TILEDB_OK) {
		release_index_page_lock(db_handle, locked_page);
		db_printf("read error\n");
		return TILEDB_SYSCALL_ERROR;
	}

	release_index_page_lock(db_handle, locked_page);
	db_printf("tile found\n");
	return TILEDB_OK;
}

tiledb_error tiledb_remove_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level) {
	tiledb_index_entry_v0 entry;
	tiledb_index_page_ref locked_page;
	tiledb_array_index objects_index;

	tiledb_error result;
	if ((result = tiledb_get_index_page_v0(db_handle, x, y, level, &entry, &locked_page, &objects_index, 0)) != TILEDB_OK) {
		//index entry for x/y/level could not be loaded
		db_printf("element not found or error occurred\n");
		return result;
	}

	//entry holds index for x/y/level and entry is locked(locked_page)
	//objects_index holds index for x/y/level in the index_entry->objects field

	if (entry.objects[objects_index].size == tiledb_switch_int(db_handle, 0)) {
		// data element not found or nodata is stored
		release_index_page_lock(db_handle, locked_page);
		db_printf("element not found\n");
		return TILEDB_NOT_FOUND;
	}

	//update index
	entry.objects[objects_index].offset = tiledb_switch_int(db_handle, 0);
	entry.objects[objects_index].checksum = tiledb_switch_int(db_handle, 0);
	entry.objects[objects_index].size = tiledb_switch_int(db_handle, 0);
	if (tiledb_store_index_page(db_handle, locked_page, &entry) != TILEDB_OK) {//TODO Don't save total entry
		release_data_lock(db_handle);
		release_index_page_lock(db_handle, locked_page);

		//index could not be updated -> db is in consistent state
		return TILEDB_SYSCALL_ERROR;
	}

	acquire_data_lock(db_handle);

	off_t offset = tiledb_switch_int(db_handle, entry.objects[objects_index].offset);
	printf("%d offset \n", (int)offset);

	//read data meta informations
	tiledb_data_entry_v0 data_entry;
	if (tiledb_read_data_to_buffer(db_handle->data_file, offset, &data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
		release_data_lock(db_handle);
		release_index_page_lock(db_handle, locked_page);
		db_printf("read error\n");
		return TILEDB_SYSCALL_ERROR;
	}

	//update data meta informations
	data_entry.mtime = tiledb_switch_int(db_handle, 0);
	data_entry.index_page = tiledb_switch_int(db_handle, -1);
	data_entry.index = tiledb_switch_int(db_handle, -1);

	//store data meta informations
	if (tiledb_store_data_to_file(db_handle->data_file, offset, &data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
		release_data_lock(db_handle);
		release_index_page_lock(db_handle, locked_page);
		db_printf("write error\n");
		return TILEDB_SYSCALL_ERROR;
	}

	release_data_lock(db_handle);
	release_index_page_lock(db_handle, locked_page);
	db_printf("tile deleted\n");
	return TILEDB_OK;
}
