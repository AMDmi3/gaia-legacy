#ifndef _TILEDB_CACHE_
#define _TILEDB_CACHE_

#include "tiledb_config.h"

void tiledb_cache_init(DB_Handle *db_handle);
int tiledb_get_from_cache(DB_Handle *db_handle, tiledb_index_page_ref page, tiledb_index_entry_v0 **entry);

#endif
