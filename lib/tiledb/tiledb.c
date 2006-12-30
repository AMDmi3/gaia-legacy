#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tiledb.h"
#include "tiledb_endian.h"
#include "tiledb_file_io.h"
#include "tiledb_file_locking.h"
#include "tiledb_internal.h"
#include "tiledb_cache.h"
#include "tiledb_defrag.h"

#if LOG_DB
#define db_printf(...) printf(__VA_ARGS__)
#else
#define db_printf(...) while(0){}
#endif

size_t tiledb_get_data_size(DB_Handle *db_handle) {
	if (db_handle == NULL || !db_handle->initialized) return 0;

	return db_handle->current_data ? db_handle->current_size : 0;
}

unsigned char *tiledb_get_data_ptr(DB_Handle *db_handle) {
	if (db_handle == NULL || !db_handle->initialized) return NULL;

	return db_handle->current_data;
}

tiledb_error tiledb_open(DB_Handle *db_handle, char *filepath, int flags) {
	db_printf("tiledb_open(%s)\n", filepath);
	tiledb_error result;

	if (db_handle == NULL) return TILEDB_INVALID_HANDLE;
	memset(db_handle, 0, sizeof(DB_Handle));

	if (flags & CLEAR_CACHE) {
		db_printf("Clear cache\n");
		if ((result = tiledb_create_new_cache(filepath)) != TILEDB_OK) return result;
	}

	int filepath_len = strlen(filepath);
	char db_path[filepath_len+4+1]; //add '.idx|.dat' and \0
	strcpy((char*)&db_path, filepath);

	strcpy(((char*)&db_path)+filepath_len, ".idx");
	db_handle->index_file = open((char*)&db_path, O_RDWR | O_SYNC);
	if (db_handle->index_file == -1) {
		if (flags & CREATE_IF_NOT_EXISTS) {
			db_printf("Create cache\n");
			if ((result = tiledb_create_new_cache(filepath)) != TILEDB_OK) return result;
		}
		db_handle->index_file = open((char*)&db_path, O_RDWR | O_SYNC);
		if (db_handle->index_file == -1) {
			db_error("on open index");
			return TILEDB_SYSCALL_ERROR;
		}
	}

	strcpy(((char*)&db_path)+filepath_len, ".dat");
	db_handle->data_file = open((char*)&db_path, O_RDWR | O_SYNC);
	if (db_handle->data_file == -1) {
		db_error("on open data");
		tiledb_close(db_handle);
		return TILEDB_SYSCALL_ERROR;
	}

	if ((result = tiledb_read_settings(db_handle)) != TILEDB_OK) {
		tiledb_close(db_handle);
		return result;
	}

	if (db_handle->version == 0) {
		db_handle->index_page_size = sizeof(tiledb_index_entry_v0);
		db_handle->data_page_size = 512;
	} else {
		tiledb_close(db_handle);
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}

	tiledb_cache_init(db_handle);

	db_handle->initialized = 1;

	return TILEDB_OK;
}

tiledb_error tiledb_close(DB_Handle *db_handle) {
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	db_handle->initialized = 0;

	tiledb_error result = TILEDB_OK;
	if (close(db_handle->index_file) == -1) {
		db_error("on close index");
		result = TILEDB_SYSCALL_ERROR;
	}

	if (close(db_handle->data_file) == -1) {
		db_error("on close data");
		result = TILEDB_SYSCALL_ERROR;
	}

	if (db_handle->current_data) {
		free(db_handle->current_data);
	}

	return result;
}

const char *tiledb_strerror(int err) {
	switch (err) {
	case TILEDB_OK:
		return "No error";
	case TILEDB_MALLOC_FAILED:
		return "Malloc failed";
	case TILEDB_SMALL_BUFFER:
		return "Internal buffer is too small (may be bug in library)";
	case TILEDB_SYSCALL_ERROR:	/* see errno() */
		return "Syscall failed";
	case TILEDB_NOT_FOUND:
		return "Not found";
	case TILEDB_UNSUPPORTED_ENDIANESS:
		return "DB is not big or little endian";
	case TILEDB_CORRUPT_DATABASE:
		return "DB is corrupt";
	case TILEDB_INVALID_HANDLE:
		return "DB handle is not initialized or null pointer";
	case TILEDB_INDEX_ENTRY_NOT_EXISTS:
		return "Index entry not exists";
	case TILEDB_UNSUPPORTED_DB_VERSION:
		return "DB version is not supported";
	default:
		return "Unknown error";
	}
}

tiledb_error tiledb_put(DB_Handle *db_handle, uint32_t x, uint32_t y, uint32_t level, unsigned char *data, size_t size) {
	db_printf("tiledb_put(x=%d, y=%d, level=%d, size=%d)\n", x, y, level, size);
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	if (db_handle->version == 0) {
		return tiledb_put_v0(db_handle, x, y, level, data, size);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}

tiledb_error tiledb_get(DB_Handle *db_handle, uint32_t x, uint32_t y, uint32_t level) {
	db_printf("tiledb_get(x=%d, y=%d, level=%d)\n", x, y, level);
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	if (db_handle->version == 0) {
		return tiledb_get_v0(db_handle, x, y, level);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}

tiledb_error tiledb_remove(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level) {
	db_printf("tiledb_remove(x=%d, y=%d, level=%d)\n", x, y, level);
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	if (db_handle->version == 0) {
		return tiledb_remove_v0(db_handle, x, y, level);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}

tiledb_error tiledb_exists(DB_Handle *db_handle, unsigned int x, unsigned int y, unsigned int level) {
	db_printf("tiledb_exists(x=%d, y=%d, level=%d)\n", x, y, level);
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	if (db_handle->version == 0) {
		return tiledb_exists_v0(db_handle, x, y, level);
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}

tiledb_error tiledb_enable_lazylock(DB_Handle *db_handle) {
	db_printf("tiledb_enable_lazylock\n");
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;
	tiledb_error result;

	//lock index page 0, put/get of other processes will block
	if ((result = acquire_index_page_lock(db_handle, 0)) != TILEDB_OK) {
		return result;
	}
	//other processes, could have locked other pages in the index_file
	//acquire lock on hole file
	size_t size = tiledb_get_file_size(db_handle->index_file);
	if ((result = acquire_file_lock(db_handle->index_file, 0, size)) != TILEDB_OK) {
		release_index_page_lock(db_handle, 0);
		return result;
	}

	db_handle->lazy_locking = 1;
	return TILEDB_OK;
}

tiledb_error tiledb_disable_lazylock(DB_Handle *db_handle) {
	db_printf("tiledb_disable_lazylock\n");
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;
	tiledb_error result;

	db_handle->lazy_locking = 0;

	//release hole file lock
	size_t size = tiledb_get_file_size(db_handle->index_file);
	if ((result = release_file_lock(db_handle->index_file, 0, size)) != TILEDB_OK) {
		release_index_page_lock(db_handle, 0);
		return result;
	}
	//release index lock
	if ((result = release_index_page_lock(db_handle, 0)) != TILEDB_OK) {
		return result;
	}

	return TILEDB_OK;
}

tiledb_error tiledb_defragment_data_file(DB_Handle *db_handle) {
	db_printf("tiledb_defragment_data_file()\n");
	if (db_handle == NULL || !db_handle->initialized) return TILEDB_INVALID_HANDLE;

	if (db_handle->version == 0) {
		tiledb_error result;
		if ((result = tiledb_enable_lazylock(db_handle)) != TILEDB_OK) {
			return result;
		}
		if ((result = tiledb_defragment_data_file_v0(db_handle)) != TILEDB_OK) {
			return result;
		}
		if ((result = tiledb_disable_lazylock(db_handle)) != TILEDB_OK) {
			return result;
		}
		return TILEDB_OK;
	} else {
		return TILEDB_UNSUPPORTED_DB_VERSION;
	}
}
