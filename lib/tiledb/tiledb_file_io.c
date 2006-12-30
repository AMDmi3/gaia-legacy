#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "tiledb_file_io.h"
#include "tiledb_config.h"
#include "tiledb_endian.h"

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

tiledb_error tiledb_store_data_to_file(int file, off_t offset, void* data, size_t size) {
	io_printf("store_data_to_file(file=%d, offset=%d, size=%d)\n", file, (int)offset, size);
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		return TILEDB_SYSCALL_ERROR;
	}
	if (write(file, data, size) != size) {
		db_error("on write");
		return TILEDB_SYSCALL_ERROR;
	}
	return TILEDB_OK;
}

tiledb_error tiledb_store_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry) {
	off_t offset = tiledb_page_ref_to_offset(db_handle, index_page_ref);
	return tiledb_store_data_to_file(db_handle->index_file, offset, index_entry, db_handle->index_page_size);
}

tiledb_error tiledb_read_data_to_buffer(int file, off_t offset, void* buffer, size_t size)
{
	io_printf("read_data_to_buffer(file=%d, offset=%d, size=%d)\n", file, (int)offset, size);
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		return TILEDB_SYSCALL_ERROR;
	}
	if (read(file, buffer, size) != size) {
		db_error("on read");
		return TILEDB_SYSCALL_ERROR;
	}
	return TILEDB_OK;
}

tiledb_error tiledb_read_index_page(DB_Handle *db_handle, tiledb_index_page_ref index_page_ref, void *index_entry) {
	off_t offset = tiledb_page_ref_to_offset(db_handle, index_page_ref);
	return tiledb_read_data_to_buffer(db_handle->index_file, offset, index_entry, db_handle->index_page_size);
}

tiledb_error tiledb_read_settings(DB_Handle* db_handle) {
	tiledb_index_header header;
	if (lseek(db_handle->index_file, 0, 0) == -1) {
		db_error("on lseek");
		return TILEDB_SYSCALL_ERROR;
	}
	if (read(db_handle->index_file, &header, sizeof(tiledb_index_header)) != sizeof(tiledb_index_header)) {
		db_error("on read");
		return TILEDB_SYSCALL_ERROR;
	}
	db_handle->pc_endianess = tiledb_get_endian();
	db_handle->version = (db_handle->pc_endianess == ENDIANESS_LITTLE)?header.version:SWAPBYTES_32(header.version);
	db_handle->db_endianess =  (db_handle->pc_endianess == ENDIANESS_LITTLE)?header.endianess:SWAPBYTES_32(header.endianess);
	return TILEDB_OK;
}
