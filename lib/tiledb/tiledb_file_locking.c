#include <unistd.h>
#include <stdio.h>

#include "tiledb_file_locking.h"

#if LOG_LOCKING
#define lock_printf(...) printf(__VA_ARGS__)
#else
#define lock_printf(...) while(0){}
#endif

void acquire_file_lock(int file, off_t offset, off_t size) {
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (lockf(file, F_LOCK, size) != 0) {
		db_error("on lock data");
		exit(-1);
	}
}

void release_file_lock(int file, off_t offset, off_t size) {
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (lockf(file, F_ULOCK, size) != 0) {
		db_error("on unlock data");
		exit(-1);
	}
}

void acquire_data_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return;
	lock_printf("acquire_data_lock()\n");
	acquire_file_lock(db_handle->data_file, 0, 1);
}

void release_data_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return;
	lock_printf("release_data_lock()\n");
	release_file_lock(db_handle->data_file, 0, 1);
}

void acquire_index_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return;
	lock_printf("acquire_index_lock()\n");
	acquire_file_lock(db_handle->index_file, 0, 1);
}

void release_index_lock(DB_Handle* db_handle) {
	if (db_handle->lazy_locking) return;
	lock_printf("release_index_lock()\n");
	release_file_lock(db_handle->index_file, 0, 1);
}

void acquire_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref) {
	if (db_handle->lazy_locking) return;
	if (page_ref < 0) return; //page_ref must be greater equal zero
	lock_printf("acquire_index_page_lock(page_ref=%d)\n", page_ref);
	off_t offset = sizeof(tiledb_index_header) + page_ref*db_handle->index_page_size;
	acquire_file_lock(db_handle->index_file, offset, (off_t)db_handle->index_page_size);
}

void release_index_page_lock(DB_Handle* db_handle, tiledb_index_page_ref page_ref) {
	if (db_handle->lazy_locking) return;
	if (page_ref < 0) return; //page_ref must be greater equal zero
	lock_printf("release_index_page_lock(page_ref=%d)\n", page_ref);
	off_t offset = sizeof(tiledb_index_header) + page_ref*db_handle->index_page_size;
	release_file_lock(db_handle->index_file, offset, (off_t)db_handle->index_page_size);
}
