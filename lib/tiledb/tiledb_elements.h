#ifndef _INDEX_STORAGE_ELEMENTS_
#define _INDEX_STORAGE_ELEMENTS_

#include <stdint.h>

#include "tiledb_config.h"

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

#endif
