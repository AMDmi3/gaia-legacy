#ifndef _INDEX_STORAGE_
#define _INDEX_STORAGE_

#include <stdio.h>

#include "tiledb_elements.h"

/* error codes */
typedef enum {
	/* no error */
	TILEDB_OK = 0,

	/* malloc failed */
	TILEDB_MALLOC_FAILED,

	/* internal buffer too small */
	TILEDB_SMALL_BUFFER,

	/* syscall error */
	TILEDB_SYSCALL_ERROR,

	/* requested data was not found */
	TILEDB_NOT_FOUND,

	/* only big/little andian archs supported */
	TILEDB_UNSUPPORTED_ENDIANESS,

	/* only big/little andian archs supported */
	TILEDB_CORRUPT_DATABASE,

	TILEDB_UNSUPPORTED_DB_VERSION,
} tiledb_error;

typedef enum {
	DUMMY = 0,
	CREATE_IF_NOT_EXISTS = 1,
	CLEAR_CACHE = 2,
} DB_Open_Flags;

/* opens a tile db */
DB_Handle *tiledb_open(char *path, int flags);

/* internal */
void tiledb_create_new_cache(char *name);

/* closes tile db */
void tiledb_close(DB_Handle* db_handle);

/* adds a tile to db */
tiledb_error tiledb_put(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level, char* data, size_t size);

/* loads a tile from db */
tiledb_error tiledb_get(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level);

size_t tiledb_get_data_size(DB_Handle* db_handle);

unsigned char *tiledb_get_data_ptr(DB_Handle* db_handle);

#endif
