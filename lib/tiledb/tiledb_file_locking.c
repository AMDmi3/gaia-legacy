#include <unistd.h>
#include <stdio.h>

#include "tiledb_file_locking.h"

#if LOG_LOCKING
#define lock_printf(...) printf(__VA_ARGS__)
#else
#define lock_printf(...) while(0){}
#endif

tiledb_error acquire_file_lock(int file, off_t offset, off_t size) {
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		return TILEDB_SYSCALL_ERROR;
	}
	if (lockf(file, F_LOCK, size) != 0) {
		db_error("on lock data");
		return TILEDB_SYSCALL_ERROR;
	}
	return TILEDB_OK;
}

tiledb_error release_file_lock(int file, off_t offset, off_t size) {
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		return TILEDB_SYSCALL_ERROR;
	}
	if (lockf(file, F_ULOCK, size) != 0) {
		db_error("on unlock data");
		return TILEDB_SYSCALL_ERROR;
	}
	return TILEDB_OK;
}

tiledb_error acquire_data_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	lock_printf("acquire_data_lock()\n");
	return acquire_file_lock(db_handle->data_file, 0, 1);
}

tiledb_error release_data_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	lock_printf("release_data_lock()\n");
	return release_file_lock(db_handle->data_file, 0, 1);
}

tiledb_error acquire_index_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	lock_printf("acquire_index_lock()\n");
	return acquire_file_lock(db_handle->index_file, 0, 1);
}

tiledb_error release_index_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	lock_printf("release_index_lock()\n");
	return release_file_lock(db_handle->index_file, 0, 1);
}

tiledb_error acquire_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	if (page_ref < 0) return TILEDB_OK; //page_ref must be greater equal zero
	lock_printf("acquire_index_page_lock(page_ref=%d)\n", page_ref);
	off_t offset = sizeof(tiledb_index_header) + page_ref*db_handle->index_page_size;
	return acquire_file_lock(db_handle->index_file, offset, (off_t)db_handle->index_page_size);
}

tiledb_error release_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref) {
	if (db_handle->lazy_locking) return TILEDB_OK;
	if (page_ref < 0) return TILEDB_OK; //page_ref must be greater equal zero
	lock_printf("release_index_page_lock(page_ref=%d)\n", page_ref);
	off_t offset = sizeof(tiledb_index_header) + page_ref*db_handle->index_page_size;
	return release_file_lock(db_handle->index_file, offset, (off_t)db_handle->index_page_size);
}
