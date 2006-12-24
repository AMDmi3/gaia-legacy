#ifndef _TILEDB_FILE_LOCKING_
#define _TILEDB_FILE_LOCKING_

#include <stdlib.h>

#include "tiledb_config.h"

tiledb_error acquire_file_lock(int file, off_t offset, off_t size);
tiledb_error release_file_lock(int file, off_t offset, off_t size);

tiledb_error acquire_data_lock(DB_Handle* db_handle);
tiledb_error release_data_lock(DB_Handle* db_handle);
tiledb_error acquire_index_lock(DB_Handle* db_handle);
tiledb_error release_index_lock(DB_Handle* db_handle);
tiledb_error acquire_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref);
tiledb_error release_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref);

#endif
