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

#define DEFAULT_INDEXCACHE_SIZE 1024*1024
#define DEFAULT_FDESCCACHE_SIZE 6

tiledb_error tiledb_open(tiledb *handle, char *path) {
	memset(handle, 0, sizeof(tiledb));

	/* checks */
	if (tiledb_get_endian() == ENDIANESS_OTHER)
		return TILEDB_UNSUPPORTED_ENDIANESS;

	/* init certain fields */
	handle->indexcache_maxsize = DEFAULT_INDEXCACHE_SIZE;

	if ((handle->path = (char*)malloc(strlen(path)+1)) == 0) {
		gefetch_close(handle);
		return GEFETCH_MALLOC_FAILED;
	}

	strcpy(handle->url, url);

	return TILEDB_OK;
}

void tiledb_close(tiledb *handle) {
	if (handle->path)
		free(handle->path);

	if (handle->currentdata)
		free(handle->currentdata);
}

tiledb_error tiledb_set_updateatime(tiledb *handle, int b) {
	handle->updateatime = b;

	return TILEDB_OK;
}
