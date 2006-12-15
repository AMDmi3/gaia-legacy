#ifndef _INDEX_STORAGE_PRIVATE_
#define _INDEX_STORAGE_PRIVATE_

#include "tiledb_elements.h"
#include "tiledb.h"

void tiledb_create_new_db_v0(DB_Handle* db_handle);

void tiledb_init_index_entry_v0(DB_Handle* db_handle, tiledb_index_entry_v0 *entry, tiledb_index_page_ref parent, tiledb_array_index index);
tiledb_array_index tiledb_get_next_offset_v0(unsigned int start_level, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_get_index_page_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, tiledb_index_entry_v0 *index_entry, tiledb_index_page_ref *locked_page, tiledb_array_index *data_element_index, int create_if_not_exists);
tiledb_error tiledb_put_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, char* data, size_t size);
tiledb_error tiledb_get_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_remove_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level);

#endif
