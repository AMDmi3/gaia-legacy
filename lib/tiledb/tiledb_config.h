#ifndef _TILEDB_CONFIG_
#define _TILEDB_CONFIG_

#include <stdint.h>
#include <stdio.h>

#define LOG_LOCKING 0
#define LOG_IO 0
#define LOG_DB 0

#define db_error(...) printf("DB-ERROR: ");					\
			printf(__VA_ARGS__);						\
			printf(" (%s:%d)\n", __FILE__, __LINE__);

/* endianess */
typedef enum {
	ENDIANESS_LITTLE,
	ENDIANESS_BIG,
	ENDIANESS_OTHER,
} tiledb_endianess;

/* error codes */
typedef enum {
	/* no error */
	TILEDB_OK = 0,

	/* malloc failed */
	TILEDB_MALLOC_FAILED,

	/* internal buffer too small */
	TILEDB_SMALL_BUFFER,

	/* syscall error */
	TILEDB_SYSCALL_ERROR,

	/* requested data was not found */
	TILEDB_NOT_FOUND,

	/* only big/little andian archs supported */
	TILEDB_UNSUPPORTED_ENDIANESS,

	/* only big/little endian archs supported */
	TILEDB_CORRUPT_DATABASE,

	/* db handle is not initialized or null pointer */
	TILEDB_INVALID_HANDLE,

	/* index not found -> no tiles for location with givben level (or deeper) does not exists */
	TILEDB_INDEX_ENTRY_NOT_EXISTS,

	/* db version is not sopported */
	TILEDB_UNSUPPORTED_DB_VERSION,
} tiledb_error;

typedef int tiledb_index_page_ref;
typedef int tiledb_array_index;

#define LEVELS_PER_PYRAMID 5

typedef struct tiledb_index_object_v0 {
	uint32_t offset;
	uint32_t size; // 0 = no data stored
	uint32_t checksum;
} tiledb_index_object_v0;

/**
 * Index Entry storing a pyramid (5 levels of the spatial tree)
 */
typedef struct tiledb_index_entry_v0 {
	int32_t parent_index_page; //root page has itself as parent; -1 => page is not used
	int32_t parent_index;      //index in parent_index_page->child_entries; root page => -1
	tiledb_index_object_v0 objects[1+4+16+64+256];
	int32_t child_entries[1024]; //page ref in index file with type IndexEntry; -1=>index entry does not exist
} tiledb_index_entry_v0;

typedef struct tiledb_data_entry_v0 {
	uint32_t mtime;      //0 => entry has been deleted
	int32_t index_page;  //index page using this data entry; -1=>index entry does not exist
	int32_t index;       //index in index_page->objects
	uint32_t size;       //size of the data following this entry in the file
} tiledb_data_entry_v0;

/**
 * used to cache tiledb_index_entry_v0->child_entries values
 */
typedef struct tiledb_cache_object_v0 {
	tiledb_index_page_ref page;
	tiledb_index_entry_v0 entry;
	int last_access;
} tiledb_cache_object_v0;

typedef struct DB_Handle {
	int initialized;

	int index_file;
	int data_file;

	int version;
	int index_page_size;
	int data_page_size;
	tiledb_endianess db_endianess;
	tiledb_endianess pc_endianess;

	int lazy_locking;

	size_t		current_size;
	unsigned char	*current_data;

	//cache for fast tree traversaling
	tiledb_cache_object_v0 subtree_cache[10];
} DB_Handle;

typedef struct tiledb_index_header {
	uint32_t version; //stored in little endian
	uint32_t endianess; //stored in little endian
} tiledb_index_header;

#endif
