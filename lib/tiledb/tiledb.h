#ifndef _INDEX_STORAGE_
#define _INDEX_STORAGE_

#include <stdio.h>

#include "tiledb_elements.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DUMMY = 0,
	CREATE_IF_NOT_EXISTS = 1,
	CLEAR_CACHE = 2,
} DB_Open_Flags;

/* opens a tile db */
DB_Handle *tiledb_open(char *filepath, int flags);

/* internal */
void tiledb_create_new_cache(char *filepath);

/* closes tile db */
void tiledb_close(DB_Handle* db_handle);

/* adds a tile to db */
tiledb_error tiledb_put(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level, char* data, size_t size);

/* loads a tile from db */
tiledb_error tiledb_get(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level);

/* deletes a tile from db */
tiledb_error tiledb_remove(DB_Handle* db_handle, unsigned int x, unsigned int y, unsigned int level);

/* reads loaded data */
size_t tiledb_get_data_size(DB_Handle* db_handle);

/* reads loaded data size */
unsigned char *tiledb_get_data_ptr(DB_Handle* db_handle);

/* locks database, put/get needs no locking, db is locked for other processes */
tiledb_error tiledb_enable_lazylock(DB_Handle* db_handle);

/* unlocks database */
tiledb_error tiledb_disable_lazylock(DB_Handle* db_handle);

/* defragment data file */
tiledb_error tiledb_defragment_data_file(DB_Handle* db_handle);

#ifdef __cplusplus
}
#endif

#endif
