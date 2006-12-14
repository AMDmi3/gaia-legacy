#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tiledb_filesystem_import.h"
#include "tiledb.h"

int main(int argc, char** argv)
{
	tiledb_filesystem_import();
	exit(0);

	DB_Handle *cache = tiledb_open("gaia_storage_table", CREATE_IF_NOT_EXISTS);

	char* hello = "HELLO WORLD ;-) ";
	tiledb_put(cache, 0, 0, 15, hello, strlen(hello));
	printf("\n");

	if (tiledb_get(cache, 0, 0, 15) == TILEDB_OK) {
		tiledb_get_data_ptr(cache)[tiledb_get_data_size(cache)-1] = 0;
		printf("%s\n", tiledb_get_data_ptr(cache));
	}

	tiledb_close(cache);
}
