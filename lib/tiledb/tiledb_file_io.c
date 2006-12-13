#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "tiledb_file_io.h"

#if LOG_IO
#define io_printf(...) printf(__VA_ARGS__)
#else
#define io_printf(...) while(0){}
#endif

size_t tiledb_get_file_size(int file) {
	struct stat file_status;
	fstat(file, &file_status);
	return file_status.st_size;
}

off_t tiledb_page_ref_to_offset(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref) {
	return sizeof(tiledb_index_header) + index_page_ref*db_handle->index_page_size;
}

tiledb_index_page_ref tiledb_get_free_index_page(DB_Handle* db_handle) {
	size_t index_size = tiledb_get_file_size(db_handle->index_file);
	return (index_size - sizeof(tiledb_index_header)) / db_handle->index_page_size;
}

void tiledb_store_data_to_file(int file, off_t offset, void* data, size_t size)
{
	io_printf("store_data_to_file(offset=%d, size=%d)\n", (int)offset, size);
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (write(file, data, size) != size) {
		db_error("on write");
		exit(-1);
	}
}

void tiledb_store_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry) {
	off_t offset = tiledb_page_ref_to_offset(db_handle, index_page_ref);
	tiledb_store_data_to_file(db_handle->index_file, offset, index_entry, db_handle->index_page_size);
}

size_t tiledb_read_data_to_buffer(int file, off_t offset, void* buffer, size_t size)
{
	io_printf("read_data_to_buffer(offset=%d, size=%d) %s:%d\n", (int)offset, size, __FILE__, __LINE__);
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (read(file, buffer, size) != size) {
		db_error("on read");
		exit(-1);
	}
	return size;
}

int tiledb_read_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry) {
	off_t offset = tiledb_page_ref_to_offset(db_handle, index_page_ref);
	size_t size = tiledb_read_data_to_buffer(db_handle->index_file, offset, index_entry, db_handle->index_page_size);
	if (size != db_handle->index_page_size) {
		return 1;
	}
	return 0;
}

int tiledb_get_version(DB_Handle* db_handle) {
	if (lseek(db_handle->index_file, 0, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	int version;
	if (read(db_handle->index_file, &version, sizeof(version)) != sizeof(version)) {
		db_error("on read");
		exit(-1);
	}
	return version;
}
