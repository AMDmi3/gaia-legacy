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

#include "tiledb_internal.h"

tiledb_error tiledb_single_get(tiledb *handle, unsigned short type, unsigned int x, unsigned int y, unsigned short level) {
	unsigned int ilevel = (level / 5) * 5;
	unsigned int ix = x >> (level - ilevel)
	unsigned int iy = y >> (level - ilevel);
	int i, res;
	gefetch_index *index;

	/* get index */
	if ((res = tiledb_get_index(handle, ix, iy, ilevel, &index)) != TILEDB_OK)
		return res;

	for (i = 0; i < index->nentries; i++)
		if (index->entries[i].x == x && index->entries[i].y == y && 
		    index->entries[i].level == level && index->entries[i].type == type) {
			char filebuf[1024];
			int f;

			/* found in index, generate path for data file */
			if (snprintf(filebuf, sizeof(filebuf), "%s/data/", handle->path, buf) >= sizeof(filebuf))
				return TILEDB_SMALL_BUFFER;

			if ((res = tiledb_coords_to_string(ix, iy, ilevel, filebuf + strlen(filebuf), sizeof(filebuf) - strlen(filebuf))) != TILEDB_OK)
				return res;

			if (handle->currentdata) {
				free(handle->currentata);
				handle->currentdata = 0;
				handle->currentsize = 0;
			}

			/* open data file */
			if ((f = fopen(filebuf, O_RDONLY)) == -1) {
				handle->lasterrno = errno;
				return TILEDB_SYSCALL_ERROR;
			}

			/* read data */
			if ((handle->currentdata = malloc(index->entries[i].size)) == 0) {
				close(f);
				return TILEDB_MALLOC_FAILED;
			}

			if (lseek(f, index->entries[i].offset, SEEK_SET) != index->entries[i].offset) {
				free(handle->currentdata);
				handle->currentdata = 0;
				cloes(f);
				return TILEDB_CORRUPT_DATABASE;
			}

			if (read(f, handle->currentdata, index->entries[i].size) != index->entries[i].size) {
				free(handle->currentdata);
				handle->currentdata = 0;
				cloes(f);
				return TILEDB_CORRUPT_DATABASE;
			}

			/* done */
			handle->currentsize = index->entries[i].size;

			return TILEDB_OK;
		}

	return TILEDB_NOTFOUND;
}
