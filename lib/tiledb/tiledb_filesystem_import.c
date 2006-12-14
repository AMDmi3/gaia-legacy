#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "tiledb_filesystem_import.h"

//filepath must contain / and .
void tiledb_extract_xyl_from_path(char *filepath, int *x, int *y, int *level) {
	filepath += strlen(filepath);

	while (*--filepath != '.') {}
	while (*--filepath != '/') {}
	filepath++;

	*x = 0;
	*y = 0;
	*level = -1;

	while (*filepath != '.') {
		*x*=2;
		*y*=2;
		*level+=1;
		switch(*filepath) {
			case '0': break;
			case '1': *x+=1;break;
			case '2': *x+=1;*y+=1;break;
			case '3': *y+=1;break;
		}
		filepath++;
	}
}

void tiledb_import_tile(DB_Handle *cache, char *filepath) {
	int file = open(filepath, O_RDONLY);

	struct stat file_status;
	fstat(file, &file_status);
	uint32_t file_size = file_status.st_size;

	char buf[file_size];
	read(file, &buf, file_size);
	close(file);

	int x, y, level;
	tiledb_extract_xyl_from_path(filepath, &x, &y, &level);

	printf("import file %s x=%d, y=%d, level=%d, size=%d\n",
		filepath, x, y, level, file_size);

	if (file_size > 0)
		tiledb_put(cache, x, y, level, (char*)&buf, file_size);
}

void tiledb_import_directory(DB_Handle *cache, char *directoy) {
	struct  dirent  *dptr;
	DIR     *dirp;
	dirp = opendir(directoy);
	while((dptr = readdir(dirp)))
	{
		char buf[1024];
		snprintf((char*)&buf, 1024, "%s/%s", directoy, dptr->d_name);
		if (dptr->d_name[strlen(dptr->d_name)-1] == 'g') {
			tiledb_import_tile(cache, (char*)&buf);
		} else if (dptr->d_name[0] != '.') {
			tiledb_import_directory(cache, (char*)&buf);
		}
	}
	closedir(dirp);
}

void tiledb_filesystem_import() {
	DB_Handle *cache;

	char db_path[1024];
	snprintf((char*)&db_path, 1024, "%s/.gaia/gaia_storage_table", getenv("HOME"));

	cache = tiledb_open((char*)&db_path, CLEAR_CACHE);
	char buf[1024];
	snprintf((char*)&buf, 1024, "%s/.gaia/cache", getenv("HOME"));
	tiledb_enable_lazylock(cache);
	tiledb_import_directory(cache, (char*)&buf);
	tiledb_disable_lazylock(cache);
	tiledb_close(cache);
}
