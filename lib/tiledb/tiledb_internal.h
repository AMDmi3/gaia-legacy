#ifndef _TILEDB_PRIVATE_
#define _TILEDB_PRIVATE_

#include "tiledb_config.h"
#include "tiledb.h"

tiledb_error tiledb_create_new_cache(char *filepath);
tiledb_error tiledb_create_new_db_v0(DB_Handle *db_handle);

int tiledb_checksum(DB_Handle *db_handle, unsigned char *data, size_t size);
int tiledb_get_paged_data_size(DB_Handle *db_handle, int size);
void tiledb_init_index_entry_v0(DB_Handle *db_handle, tiledb_index_entry_v0 *entry, tiledb_index_page_ref parent, tiledb_array_index index);
tiledb_array_index tiledb_get_next_offset_v0(unsigned int start_level, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_read_index_object_v0(DB_Handle *db_handle, tiledb_index_object_v0 *index_object, tiledb_index_page_ref page, tiledb_array_index index);
tiledb_error tiledb_store_index_object_v0(DB_Handle *db_handle, tiledb_index_object_v0 *index_object, tiledb_index_page_ref page, tiledb_array_index index);
tiledb_error tiledb_read_index_page_child_entry_v0(DB_Handle *db_handle, tiledb_index_page_ref page, tiledb_array_index index, tiledb_index_page_ref *index_offset);
tiledb_error tiledb_store_index_page_child_entry_v0(DB_Handle *db_handle, tiledb_index_page_ref page, tiledb_array_index index, tiledb_index_page_ref child_page);
tiledb_error tiledb_get_index_object_v0(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level, tiledb_index_object_v0 *index_object, tiledb_index_page_ref *locked_page, tiledb_array_index *objects_index, int create_if_not_exists);

tiledb_error tiledb_put_v0(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level, unsigned char * data, size_t size);
tiledb_error tiledb_get_v0(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_remove_v0(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level);
tiledb_error tiledb_exists_v0(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level);

#endif
