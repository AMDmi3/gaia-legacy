#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tiledb_filesystem_import.h"
#include "tiledb.h"

void import() {
	printf("import\n");
	DB_Handle db_handle;
	char buf[10240];
	int x, y, l;
	tiledb_open(&db_handle, "test_db", CREATE_IF_NOT_EXISTS);
	for (l=0; l<6; l++)
		for (x=0; x<(1<<l); x++)
			for (y=0; y<(1<<l); y++) {
				if (tiledb_put(&db_handle, x, y, l, (unsigned char*)&buf, 10240) != TILEDB_OK) {
					printf("error put %d/%d/%d\n", x, y, l);
				}
			}
	tiledb_close(&db_handle);
}

void export() {
	printf("export\n");
	DB_Handle db_handle;
	int x, y, l;
	tiledb_open(&db_handle, "test_db", CREATE_IF_NOT_EXISTS);
	for (l=0; l<6; l++)
		for (x=0; x<(1<<l); x++)
			for (y=0; y<(1<<l); y++) {
				if (tiledb_get(&db_handle, x, y, l) == TILEDB_OK) {
					if (tiledb_get_data_size(&db_handle) != 10240) {
						printf("error load %d/%d/%d\n", x, y, l);
					}
				}
			}
	tiledb_close(&db_handle);
}

int main(int argc, char** argv)
{
	import();
	export();
	exit(0);

	tiledb_filesystem_import();
	exit(0);

	char db_path[1024];
	snprintf((char*)&db_path, 1024, "%s/.gaia/gaia_storage_table", getenv("HOME"));

	DB_Handle db_handle;
	tiledb_open(&db_handle, (char*)&db_path, CREATE_IF_NOT_EXISTS);
	tiledb_remove(&db_handle, 0, 1, 1);
	tiledb_close(&db_handle);

	exit(0);

	DB_Handle cache;
	tiledb_open(&cache, "gaia_storage_table", CREATE_IF_NOT_EXISTS);

	char* hello = "HELLO WORLD ;-) ";
	tiledb_put(&cache, 0, 0, 15, (unsigned char *)hello, strlen(hello));
	printf("\n");

	if (tiledb_get(&cache, 0, 0, 15) == TILEDB_OK) {
		tiledb_get_data_ptr(&cache)[tiledb_get_data_size(&cache)-1] = 0;
		printf("%s\n", tiledb_get_data_ptr(&cache));
	}

	tiledb_close(&cache);
}
