#include "tiledb.h"
#include "tiledb_file_io.h"
#include "tiledb_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


size_t tiledb_get_data_size(DB_Handle* db_handle) {
	return db_handle->current_data ? db_handle->current_size : 0;
}

unsigned char *tiledb_get_data_ptr(DB_Handle* db_handle) {
	return db_handle->current_data;
}

DB_Handle *tiledb_open(char *path, int flags) {
	DB_Handle *db_handle = (DB_Handle *)malloc(sizeof(DB_Handle));
	memset(db_handle, 0, sizeof(DB_Handle));

	if (flags & CLEAR_CACHE) {
		printf("Clear cache\n");
		tiledb_create_new_cache(""); // TODO name handling
	}
	db_handle->index_file = open("gaia_storage_cache.idx", O_RDWR | O_SYNC);
	if (db_handle->index_file == -1) {
		if (flags & CREATE_IF_NOT_EXISTS) {
			printf("Create cache\n");
			tiledb_create_new_cache(""); // TODO name handling
		}
		db_handle->index_file = open("gaia_storage_cache.idx", O_RDWR | O_SYNC);
		if (db_handle->index_file == -1) {
			db_error("on open index");
			exit(-1);
		}
	}
	db_handle->data_file = open("gaia_storage_cache.dat", O_RDWR | O_SYNC);
	if (db_handle->data_file == -1) {
		db_error("on open data");
		exit(-1);
	}
	db_handle->version = get_cache_version(db_handle);
	return db_handle;
}

void tiledb_close(DB_Handle* db_handle) {
	if (close(db_handle->index_file) == -1) {
		db_error("on close index");
		exit(-1);
	}

	if (close(db_handle->data_file) == -1) {
		db_error("on close data");
		exit(-1);
	}

	if (db_handle->current_data) {
		free(db_handle->current_data);
	}

	free(db_handle);
}

void tiledb_create_new_cache(char *name)
{
	DB_Handle handle;

	handle.index_file = open("gaia_storage_cache.idx", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
	if (handle.index_file == -1) {
		db_error("on creating new index file");
		exit(-1);
	}

	handle.data_file = open("gaia_storage_cache.dat", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
	if (handle.index_file == -1) {
		db_error("on creating new index file");
		exit(-1);
	}

	create_new_db(&handle);

	if (close(handle.index_file) == -1) {
		db_error("on close index");
		exit(-1);
	}
	if (close(handle.data_file) == -1) {
		db_error("on close data");
		exit(-1);
	}
}

tiledb_error tiledb_put(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level, char* data, size_t size)
{
	printf("tiledb_put(x=%d, y=%d, level=%d, size=%d)\n", x, y, level, size);

	if (db_handle->version == 0) {
		return tiledb_put_v0(db_handle, x, y, level, data, size);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}

tiledb_error tiledb_get(DB_Handle* db_handle, uint32_t x, uint32_t y, uint32_t level)
{
	printf("tiledb_get(x=%d, y=%d, level=%d)\n", x, y, level);

	if (db_handle->version == 0) {
		return tiledb_get_v0(db_handle, x, y, level);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}
