#ifndef _INDEX_STORAGE_CONFIG_
#define _INDEX_STORAGE_CONFIG_

#include <stdint.h>

#define LOG_LOCKING 0
#define LOG_IO 0

#define db_error(...) printf("DB-ERROR: ");					\
			printf(__VA_ARGS__);						\
			printf(" in File(%s:%d)\n", __FILE__, __LINE__);

typedef struct DB_Handle {
	int index_file;
	int data_file;
	uint32_t version;

	size_t		current_size;
	unsigned char	*current_data;
} DB_Handle;

typedef struct tiledb_index_header {
	uint32_t version;
} tiledb_index_header;

#endif