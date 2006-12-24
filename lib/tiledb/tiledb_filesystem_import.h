#ifndef _TILEDB_IMPORT_
#define _TILEDB_IMPORT_

#include "tiledb.h"

void tiledb_extract_xyl_from_path(char *filepath, int *x, int *y, int *level);
void tiledb_import_tile(DB_Handle *cache, char *filepath);
void tiledb_import_directory(DB_Handle *cache, char *directoy);

void tiledb_filesystem_import();

#endif
