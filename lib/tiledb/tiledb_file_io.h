#ifndef _INDEX_STORAGE_FILE_IO_
#define _INDEX_STORAGE_FILE_IO_

#include <stdint.h>
#include "tiledb_config.h"


size_t tiledb_get_file_size(int file);

void store_data_to_file(int file, uint32_t offset, void* data, size_t size);
uint32_t read_data_to_buffer(int file, uint32_t offset, void* buffer, size_t size);

uint32_t tiledb_get_version(DB_Handle* db_handle);

#endif
