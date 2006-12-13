#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
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

void store_data_to_file(int file, uint32_t offset, void* data, size_t size)
{
	io_printf("store_data_to_file(offset=%d, size=%d)\n", offset, size);
	if (lseek(file, offset, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	if (write(file, data, size) != size) {
		db_error("on write");
		exit(-1);
	}
}

uint32_t read_data_to_buffer(int file, uint32_t offset, void* buffer, size_t size)
{
	io_printf("read_data_to_buffer(offset=%d, size=%d) %s:%d\n", offset, size, __FILE__, __LINE__);
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

uint32_t tiledb_get_version(DB_Handle* db_handle) {
	if (lseek(db_handle->index_file, 0, 0) == -1) {
		db_error("on lseek");
		exit(-1);
	}
	uint32_t version;
	if (read(db_handle->index_file, &version, sizeof(version)) != sizeof(version)) {
		db_error("on read");
		exit(-1);
	}
	return version;
}
