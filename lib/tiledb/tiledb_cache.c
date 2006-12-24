#include "tiledb_cache.h"

void tiledb_cache_init(DB_Handle *db_handle) {
	int i;
	for (i=0; i<sizeof(db_handle->subtree_cache)/sizeof(tiledb_cache_object_v0); i++) {
		db_handle->subtree_cache[i].page = -1;
		db_handle->subtree_cache[i].last_access = i+1;
	}
}

int tiledb_get_from_cache(DB_Handle *db_handle, tiledb_index_page_ref page, tiledb_index_entry_v0 **entry) {
	int i;
	int found = 0;
	int last_access = 1;

	for (i=0; i<sizeof(db_handle->subtree_cache)/sizeof(tiledb_cache_object_v0); i++) {
		if (db_handle->subtree_cache[i].page == page) {
			//printf("cache hit %d\n", i);
			*entry = &(db_handle->subtree_cache[i].entry);
			found = 1;
			last_access = db_handle->subtree_cache[i].last_access;
		}
	}

	for (i=0; i<sizeof(db_handle->subtree_cache)/sizeof(tiledb_cache_object_v0); i++) {
		if (db_handle->subtree_cache[i].last_access > last_access) {
			db_handle->subtree_cache[i].last_access--;
		} else if (db_handle->subtree_cache[i].last_access == last_access && found) {
			db_handle->subtree_cache[i].last_access = sizeof(db_handle->subtree_cache)/sizeof(tiledb_cache_object_v0);
		} else if (db_handle->subtree_cache[i].last_access == 1 && !found) {
			//printf("cache miss %d\n", i);
			*entry = &(db_handle->subtree_cache[i].entry);
			db_handle->subtree_cache[i].page = page;
			db_handle->subtree_cache[i].last_access = sizeof(db_handle->subtree_cache)/sizeof(tiledb_cache_object_v0);
		}
	}

	return found;
}
