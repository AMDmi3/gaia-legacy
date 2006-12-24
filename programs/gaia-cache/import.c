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

	/* traverse dirtree */
	while ((ftsent = fts_read(fts))) {
		if (ftsent->fts_info == FTS_F || ftsent->fts_info == FTS_NS || ftsent->fts_info == FTS_NSOK) {
			if (g_typemask && TYPEMASK_IMAGE)
				successresult &= import_image(ftsent);
		}
	}

	/* close fts */
	fts_close(fts);

	/* free buffer */
	free(import_buffer);

	return successresult;
}

int action_export() {
	return 1;
}
