#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tiledb.h"

int main(int argc, char** argv)
{
	//create_initial_index();

	DB_Handle *cache = tiledb_open(".", CREATE_IF_NOT_EXISTS);

	char* hello = "HELLO WORLD ;-) ";
	tiledb_put(cache, 0, 0, 15, hello, strlen(hello));
	printf("\n");

	if (tiledb_get(cache, 0, 0, 15) == TILEDB_OK) {
		tiledb_get_data_ptr(cache)[tiledb_get_data_size(cache)-1] = 0;
		printf("%s\n", tiledb_get_data_ptr(cache));
	}

tiledb_close(cache);
}
