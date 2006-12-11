#ifndef _INDEX_STORAGE_FILE_IO_
#define _INDEX_STORAGE_FILE_IO_

#include <stdint.h>
#include "tiledb_config.h"

void store_data_to_file(int file, uint32_t offset, void* data, uint32_t size);
uint32_t read_data_to_buffer(int file, uint32_t offset, void* buffer, uint32_t size);

uint32_t get_cache_version(DB_Handle* db_handle);

#endif
