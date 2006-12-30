#include "tiledb_defrag.h"
#include "tiledb_internal.h"
#include "tiledb_file_io.h"
#include "tiledb_endian.h"

tiledb_error tiledb_move_data_v0(DB_Handle *db_handle, int from, int to, int size) {
	printf("tiledb_move_data_v0 %d => %d (%d)\n", from, to, size);
	//old and new location must not overlap
	tiledb_error result;

	char buffer[size];
	if ((result = tiledb_read_data_to_buffer(db_handle->data_file, from, &buffer, size)) != TILEDB_OK) {
		return result;
	}
	tiledb_data_entry_v0 *data_entry = (tiledb_data_entry_v0 *)&buffer;

	//copy data to new location
	if ((result = tiledb_store_data_to_file(db_handle->data_file, to, &buffer, size)) != TILEDB_OK) {
		return result;
	}

	//read old index
	tiledb_index_object_v0 index_object;
	if ((result = tiledb_read_index_object_v0(db_handle, &index_object, data_entry->index_page, data_entry->index)) != TILEDB_OK) {
		return result;
	}

	//update index, set new data location
	index_object.offset = tiledb_switch_int(db_handle, to);

	//store new index
	if ((result = tiledb_store_index_object_v0(db_handle, &index_object, data_entry->index_page, data_entry->index)) != TILEDB_OK) {
		return result;
	}

	//mark old location invalid
	data_entry->mtime = tiledb_switch_int(db_handle, 0);
	if ((result = tiledb_store_data_to_file(db_handle->data_file, from, &buffer, sizeof(tiledb_data_entry_v0))) != TILEDB_OK) {
		return result;
	}

	return TILEDB_OK;
}

int tiledb_check_data_entry_for_garbage_v0(DB_Handle *db_handle, tiledb_data_entry_v0 *data_entry, int data_entry_offset) {
	if (data_entry->mtime == tiledb_switch_int(db_handle, 0)) {
		return 1;
	}

	tiledb_index_object_v0 index_object;
	int index_page = tiledb_switch_int(db_handle, data_entry->index_page);
	int index = tiledb_switch_int(db_handle, data_entry->index);
	if (tiledb_read_index_object_v0(db_handle, &index_object, index_page, index) != TILEDB_OK) {
		return 0; //error => dont change anything //TODO exit garbage collection
	}

	if (index_object.offset != tiledb_switch_int(db_handle, data_entry_offset)) {
		return 1;
	}

	return 0;
}

tiledb_error tiledb_search_next_non_garbage_entry_starting_with_v0(DB_Handle *db_handle, int *offset, tiledb_data_entry_v0 *data_entry, int max) {
	while ((*offset) < max) {
		//read data meta informations
		if (tiledb_read_data_to_buffer(db_handle->data_file, *offset, data_entry, sizeof(tiledb_data_entry_v0)) != TILEDB_OK) {
			return TILEDB_SYSCALL_ERROR;
		}

		if (tiledb_check_data_entry_for_garbage_v0(db_handle, data_entry, *offset) == 0) {
			return TILEDB_OK;
		}
		(*offset) += tiledb_get_paged_data_size(db_handle, data_entry->size+sizeof(tiledb_data_entry_v0));
	}
	return TILEDB_NOT_FOUND;
}

tiledb_error tiledb_defragment_data_file_v0(DB_Handle *db_handle) {
	tiledb_error result;

	int free_location = 0;
	tiledb_data_entry_v0 data_entry;
	int data_entry_offset = 0;

	int data_file_size = tiledb_get_file_size(db_handle->data_file);

	while (tiledb_search_next_non_garbage_entry_starting_with_v0(db_handle, &data_entry_offset, &data_entry, data_file_size) == TILEDB_OK) {
		int data_entry_size = tiledb_get_paged_data_size(db_handle, data_entry.size+sizeof(tiledb_data_entry_v0));

		if (free_location == data_entry_offset) {
			//nothing to do
		} else if (free_location+data_entry_size < data_entry_offset) { //free space is larger than data
			//mark free space after new copied data as free
			tiledb_data_entry_v0 free_space_marker;
			int size = data_entry_offset-(free_location+data_entry_size)-sizeof(tiledb_data_entry_v0);
			free_space_marker.size = tiledb_switch_int(db_handle, size);
			free_space_marker.mtime = tiledb_switch_int(db_handle, 0);
			if ((result = tiledb_store_data_to_file(db_handle->data_file, free_location+data_entry_size, &free_space_marker, sizeof(tiledb_data_entry_v0))) != TILEDB_OK) {
				return result;
			}
			if ((result = tiledb_move_data_v0(db_handle, data_entry_offset, free_location, data_entry_size)) != TILEDB_OK) {
				return result;
			}
		} else if (free_location+data_entry_size == data_entry_offset) { //free space has same size as data
			if ((result = tiledb_move_data_v0(db_handle, data_entry_offset, free_location, data_entry_size)) != TILEDB_OK) {
				return result;
			}
		} else {
			ftruncate(db_handle->data_file, data_file_size); //drop end of file, (else current data < last data => corrupt db)
			if ((result = tiledb_move_data_v0(db_handle, data_entry_offset, data_file_size, data_entry_size)) != TILEDB_OK) {
				return result;
			}
			if ((result = tiledb_move_data_v0(db_handle, data_file_size, free_location, data_entry_size)) != TILEDB_OK) {
				return result;
			}
		}
		free_location += data_entry_size;
		data_entry_offset += data_entry_size;
	}
	ftruncate(db_handle->data_file, free_location);
	return TILEDB_OK;
}
