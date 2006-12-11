#ifndef _INDEX_STORAGE_ELEMENTS_
#define _INDEX_STORAGE_ELEMENTS_

#include <stdint.h>

#include "tiledb_config.h"

#define LEVELS_PER_PYRAMID 5

typedef struct tiledb_index_entry_ptr_v0 {
	uint32_t used:1;
	uint32_t space:31;
	uint32_t offset;
	uint32_t last_modification_time;
	uint32_t size;
} tiledb_index_entry_ptr_v0;

/**
 * Index Entry storing a pyramid (5 levels of the spatial tree)
 */
typedef struct tiledb_index_entry_v0 {
	uint32_t dirty:1;
	uint32_t deleted:1;
	uint32_t space:30; //space for more flags
	uint32_t create_time;
	uint32_t last_access_time;
	tiledb_index_entry_ptr_v0 data_elements[1+4+16+64+256];
	uint32_t child_entries[1024]; //offset in index file with type IndexEntry; 0=>index entry does not exist

} tiledb_index_entry_v0;

#endif
