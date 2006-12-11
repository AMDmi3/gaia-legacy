#ifndef _INDEX_STORAGE_FILE_LOCKING_
#define _INDEX_STORAGE_FILE_LOCKING_

#include <stdlib.h>

#include "tiledb_config.h"

void acquire_data_lock(DB_Handle* db_handle);
void release_data_lock(DB_Handle* db_handle);
void acquire_index_lock(DB_Handle* db_handle);
void release_index_lock(DB_Handle* db_handle);
void acquire_index_entry_lock(DB_Handle* db_handle, long offset, size_t entry_size);
void release_index_entry_lock(DB_Handle* db_handle, long offset, size_t entry_size);

#endif
