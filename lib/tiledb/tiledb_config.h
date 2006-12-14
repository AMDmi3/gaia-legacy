#ifndef _INDEX_STORAGE_CONFIG_
#define _INDEX_STORAGE_CONFIG_

#include <stdint.h>
#include <stdio.h>

#define LOG_LOCKING 0
#define LOG_IO 0

#define db_error(...) printf("DB-ERROR: ");					\
			printf(__VA_ARGS__);						\
			printf(" (%s:%d)\n", __FILE__, __LINE__);

/* endianess */
typedef enum {
	ENDIANESS_LITTLE,
	ENDIANESS_BIG,
	ENDIANESS_OTHER,
} tiledb_endianess;

typedef struct DB_Handle {
	int index_file;
	int data_file;

	int version;
	int index_page_size;
	tiledb_endianess db_endianess;
	tiledb_endianess pc_endianess;

	int lazy_locking;

	size_t		current_size;
	unsigned char	*current_data;
} DB_Handle;

typedef struct tiledb_index_header {
	uint32_t version; //stored in little endian
	uint32_t endianess; //stored in little endian
} tiledb_index_header;

typedef int tiledb_index_page_ref;
typedef int tiledb_array_index;

#endif
