#ifndef _INDEX_STORAGE_FILE_IO_
#define _INDEX_STORAGE_FILE_IO_

#include <unistd.h>
#include "tiledb_config.h"


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "tiledb_file_io.h"

size_t tiledb_get_file_size(int file);

off_t tiledb_page_ref_to_offset(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref);
tiledb_index_page_ref tiledb_get_free_index_page(DB_Handle* db_handle);

void tiledb_store_data_to_file(int file, off_t offset, void* data, size_t size);
void tiledb_store_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry);

size_t tiledb_read_data_to_buffer(int file, off_t offset, void* buffer, size_t size);
int tiledb_read_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry);

void tiledb_read_settings(DB_Handle* db_handle);

#endif
