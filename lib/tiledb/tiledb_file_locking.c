#include <unistd.h>
#include <stdio.h>

#include "tiledb_file_locking.h"

#if LOG_LOCKING
#define lock_printf(...) printf(__VA_ARGS__)
#else
#define lock_printf(...) while(0){}
#endif

void acquire_file_lock(int file, long offset, off_t size) {
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (lockf(file, F_LOCK, size) != 0) {
		db_error("on lock data");
		exit(-1);
	}
}

void release_file_lock(int file, long offset, off_t size) {
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
	lock_printf("acquire_data_lock()\n");
	acquire_file_lock(db_handle->data_file, 0, 1);
}

void release_data_lock(DB_Handle* db_handle) {
	lock_printf("release_data_lock()\n");
	release_file_lock(db_handle->data_file, 0, 1);
}

void acquire_index_lock(DB_Handle* db_handle) {
	lock_printf("acquire_index_lock()\n");
	acquire_file_lock(db_handle->index_file, 0, 1);
}

void release_index_lock(DB_Handle* db_handle) {
	lock_printf("release_index_lock()\n");
	release_file_lock(db_handle->index_file, 0, 1);
}

void acquire_index_entry_lock(DB_Handle* db_handle, long offset, size_t entry_size) {
	if (offset == 0) return; //offset must be greater than zero, file begins with header
	lock_printf("acquire_index_entry_lock(offset=%d)\n", offset);
	acquire_file_lock(db_handle->index_file, offset, (off_t)entry_size);
}

void release_index_entry_lock(DB_Handle* db_handle, long offset, size_t entry_size) {
	if (offset == 0) return; //offset must be greater than zero, file begins with header
	lock_printf("release_index_entry_lock(offset=%d)\n", offset);
	release_file_lock(db_handle->index_file, offset, (off_t)entry_size);
}
