/*
 * gaia - opensource 3D interface to the planet
 * Copyright (C) 2006 gaia project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "gaia-cache.h"

#define DEFAULT_IMPORT_BUFFER_SIZE	64*1024

size_t	import_buffer_size = 0;
void	*import_buffer = 0;

static int import_image(FTSENT *ftsent) {
	int x = 0;
	int y = 0;
	int level = -1;

	/* check number part of filename, calculating x/y/level on the fly */
	char *cur;
	for (cur = ftsent->fts_name; *cur && *cur != '.'; cur++) {
		x <<= 1;
		y <<= 1;
		level++;
		switch(*cur) {
		case '0':
			break;
		case '1':
			x |= 1;
			break;
		case '2':
			x |= 1;
			y |= 1;
			break;
		case '3':
			y |= 1;
			break;
		default:
			return 1;	/* not an image */
		}
	}

	/* check remainder of file name */
	if (cur[0] != '.' || cur[1] != 'j' || cur[2] != 'p' || cur[3] != 'g')
		return 1;	/* not an image */

	/* ensure buffer is large enough */
	if (ftsent->fts_statp->st_size > import_buffer_size) {
		void *newp = realloc(import_buffer, ftsent->fts_statp->st_size);
		if (newp == 0) {
			fprintf(stderr, "%s: cannot allocate buffer\n", ftsent->fts_path);
			return 0;
		}
		import_buffer = newp;
		import_buffer_size = ftsent->fts_statp->st_size;
	}

	/* open file */
	int f;
	if ((f = open(ftsent->fts_accpath, O_RDONLY)) == -1) {
		fprintf(stderr, "%s: cannot open\n", ftsent->fts_path);
		return 0;
	}

	/* read file */
	if (read(f, import_buffer, ftsent->fts_statp->st_size) != ftsent->fts_statp->st_size) {
		close(f);
		fprintf(stderr, "%s: short read\n", ftsent->fts_path);
		return 0;
	}

	/* put into cache */
	int res;
	if ((res = tiledb_put(&g_tiledb, x, y, level, import_buffer, ftsent->fts_statp->st_size)) != TILEDB_OK) {
		fprintf(stderr, "%s: cannot import: %s (code %d)\n",ftsent->fts_path ,tiledb_strerror(res), res);
		close(f);
		return 0;
	}

	/* close file */
	close(f);

	if (g_verboseflag)
		fprintf(stderr, "%s: added %d %d %d\n", ftsent->fts_path, x, y, level);

	return 1;
}

int action_import() {
	char *ftsargv[2] = { g_filesdir, 0 };
	FTS *fts;
	FTSENT *ftsent;

	/* pre-alloc buffer */
	if ((import_buffer = malloc(DEFAULT_IMPORT_BUFFER_SIZE)) == 0)
		err(1, "cannot allocate buffer");

	import_buffer_size = DEFAULT_IMPORT_BUFFER_SIZE;

	/* open fts */
	if ((fts = fts_open(ftsargv, FTS_LOGICAL, 0)) == 0) {
		free(import_buffer);
		err(1, "cannot open import directory");
	}

	int successresult = 1;

	//tiledb_enable_lazylock(&g_tiledb);

	/* traverse dirtree */
	while ((ftsent = fts_read(fts))) {
		if (ftsent->fts_info == FTS_F || ftsent->fts_info == FTS_NS || ftsent->fts_info == FTS_NSOK) {
			if (g_typemask && TYPEMASK_IMAGE)
				successresult &= import_image(ftsent);
		}
	}

	//tiledb_disable_lazylock(&g_tiledb);

	/* close fts */
	fts_close(fts);

	/* free buffer */
	free(import_buffer);

	return successresult;
}

void rec_export(int x, int y, int level) {
	tiledb_error result = tiledb_exists(&g_tiledb, x, y, level);
	if (result == TILEDB_OK) {
		if ((result = tiledb_get(&g_tiledb, x, y, level)) == TILEDB_OK) {
			char path[1024];
			char name[1024];
			int i;
			int c = 0;
			for (i = 0; i <= level; i++) {
				int bit = 1 << (level-i);

				if (x & bit) {
					if (y & bit)    name[i] = '2';
					else            name[i] = '1';
				} else {
					if (y & bit)    name[i] = '3';
					else            name[i] = '0';
				}

				if (i < ((level+1) & ~0x3)) {
					if (x & bit) {
						if (y & bit)    path[c++] = '2';
						else            path[c++] = '1';
					} else {
						if (y & bit)    path[c++] = '3';
						else            path[c++] = '0';
					}

					if (i % 4 == 3)
						path[c++] = '/';
				}
			}
			name[i] = 0;
			path[c] = 0;
			char file[1024];
			snprintf((char*)&file, 1024, "%s/%s", g_filesdir, (char*)&path);
			if (!(mkdir((char*)&file, 0777) != 0 && errno != EEXIST)) {
				snprintf((char*)&file, 1024, "%s/%s/%s.jpg", g_filesdir, (char*)&path, (char*)&name);
				int f;
				if ((f = open((char*)&file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) != -1) {
					size_t size = tiledb_get_data_size(&g_tiledb);
					if (write(f, tiledb_get_data_ptr(&g_tiledb), size) != size) {
						printf("oops2 %d\n", errno);
						unlink(file);	/* ensure no broken files get into cache */
						close(f);
					}
					close(f);
				} else {
					printf("oops3 %d %d\n", f, errno);
					//error open file
				}
			} else {
				printf("oops4 %d\n", errno);
				//error open file
			}
		}
		if (level == 3) printf(".");fflush(stdout);
	}

	if ((result == TILEDB_OK || result == TILEDB_NOT_FOUND) && level < 20) {
		rec_export((x<<1)+0, (y<<1)+0, level+1);
		rec_export((x<<1)+1, (y<<1)+0, level+1);
		rec_export((x<<1)+0, (y<<1)+1, level+1);
		rec_export((x<<1)+1, (y<<1)+1, level+1);
	}
}

int action_export() {
	rec_export(0, 0, 0);
	printf("\n");
	return 1;
}
