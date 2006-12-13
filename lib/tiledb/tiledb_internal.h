#ifndef _INDEX_STORAGE_PRIVATE_
#define _INDEX_STORAGE_PRIVATE_

#include "tiledb_elements.h"
#include "tiledb.h"

void create_new_db(DB_Handle* db_handle);

void init_index_entry_v0(tiledb_index_entry_v0 *entry);
uint32_t get_next_offset_v0(tiledb_index_entry_v0 *entry, unsigned int start_level, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_put_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, char* data, size_t size);
tiledb_error tiledb_get_v0(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level);

#endif
