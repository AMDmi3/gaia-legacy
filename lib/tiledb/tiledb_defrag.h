#ifndef _TILEDB_DEFRAG_
#define _TILEDB_DEFRAG_

#include "tiledb_config.h"

tiledb_error tiledb_move_data_v0(DB_Handle *db_handle, int from, int to, int size);
int tiledb_check_data_entry_for_garbage_v0(DB_Handle *db_handle, tiledb_data_entry_v0 *data_entry, int data_entry_offset);
tiledb_error tiledb_search_next_non_garbage_entry_starting_with_v0(DB_Handle *db_handle, int *offset, tiledb_data_entry_v0 *data_entry, int max);
tiledb_error tiledb_defragment_data_file_v0(DB_Handle *db_handle);

#endif
