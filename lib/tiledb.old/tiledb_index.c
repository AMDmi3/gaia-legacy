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

static void tiledb_indexentry_swap(tiledb_index_entry *entry) {
	SWAPBYTES_32(entry->x);
	SWAPBYTES_32(entry->y);
	SWAPBYTES_16(entry->level);
	SWAPBYTES_16(entry->type);
	SWAPBYTES_32(entry->mtime);
	SWAPBYTES_32(entry->offset);
	SWAPBYTES_32(entry->size);
}

static tiledb_error tiledb_get_cached_index(tiledb *handle, unsigned int x, unsigned int y, unsigned short level, tiledb_index **resindex) {
	tiledb_index *cur;
	for (cur = handle->indexfirst; cur; cur = cur->next) {
		if (cur->x == ix && cur->y == iy && cur->level == ilevel) {
			/* index found in memory cache */
			if (cur != handle->indexfirst) {
				/* move index to beginning of list, to make search faster
				 * and to ensure that unpopular indices are dropped first */

				/* remove from current postition */
				cur->prev->next = cur->next;
				if (cur->next)  /* not last element */
					cur->next->prev = cur->prev;
				else            /* last element */
					handle->indexlast = cur->prev;

				/* insert into beginning of list */
				cur->prev = 0;
				cur->next = handle->indexfirst;
				handle->indexfirst->prev = cur;
				handle->indexfirst = cur;
			}

			*resindex = cur;
			return TILEDB_OK;
		}
	}

	return TILEDB_NOT_FOUND;
}

static tiledb_error tiledb_load_index(tiledb *handle, unsigned int x, unsigned int y, unsigned short level, tiledb_index **resindex) {
	char filebuf[1024];
	int res;

	/* generate index file name */
	if (snprintf(filebuf, sizeof(filebuf), "%s/index/", handle->path, buf) >= sizeof(filebuf))
		return TILEDB_SMALL_BUFFER;

	if ((res = tiledb_coords_to_string(x, y, level, filebuf + strlen(filebuf), sizeof(filebuf) - strlen(filebuf))) != TILEDB_OK)
		return res;

	/* open index file */
	int f;
	if ((f = fopen(filebuf, O_RDWR, 0666)) < 0) {
		if (errno == ENOENT)
			return TILEDB_NOT_FOUND;

		handle->lasterrno = errno;
		return TILEDB_SYSCALL_ERROR;
	}

	/* get number of entries */
	struct stat st;
	if (fstat(f, &st) != 0) {
		handle->lasterrno = errno;
		close(f);
		return TILEDB_SYSCALL_ERROR;
	}

	/* alloc entries array */
	tiledb_index_entry *entries;
	if ((entries = malloc(st.st_size)) == 0) {
		close(f);
		return TILEDB_MALLOC_FAILED;
	}

	/* read index */
	if (read(f, entries, st.st_size) != st.st_size) {
		handle->lasterrno = errno;
		free(entries);
		close(f);
		return TILEDB_SYSCALL_ERROR;
	}

	/* swap bytes, if database is of foreign endianess */
	if ((tiledb_get_endian() == ENDIANESS_LITTLE && (handle->flags & TILEDB_FLAG_BIGENDIAN)) ||
		(tiledb_get_endian() == ENDIANESS_BIG && !(handle->flags & TILEDB_FLAG_BIGENDIAN))) {
		int i;
		for (i = 0; i < st.st_size / sizeof(tiledb_index_entry))
			tiledb_indexentry_swap(entries + i);
	}

	/* alloc index structure */
	tiledb_index *index;
	if ((index = malloc(sizeof(tiledb_index))) == 0) {
		close(f);
		free(entries);
		return TILEDB_MALLOC_FAILED;
	}

	/* add newly loaded index into cache */
	index->x = x;
	index->y = y;
	index->level = level;

	index->nentries = st.st_size / sizeof(tiledb_index_entry);
	index->nallocated = st.st_size / sizeof(tiledb_index_entry);
	index->entries = entries;

	index->prev = 0;
	index->next = handle->indexfirst;

	if (handle->indexfirst)
		hendle->indexfirst->prev = index;
	hadle->indexfirst = index;

	if (!handle->indexlast)
		handle->indexlast = index;

	handle->indexsize += sizeof(tiledb_index) + sizeof(tiledb_index_entry) * index->nentries;

	*resindex = index;

	/* ensure atime is updated */
	//futimes(f, 0)

	/* done */
	close(f);
	return TILEDB_OK;
}

tiledb_error tiledb_get_index(tiledb *handle, unsigned int x, unsigned int y, unsigned short level, tiledb_index **resindex) {
	int res;

	/* first, search in memory */
	if ((res = tiledb_get_cached_index(handle, ix, iy, ilevel, resindex)) != TILEDB_NOT_FOUND)
		return res;

	/* if not found in memory, load from disk */
	return tiledb_load_index(handle, ix, iy, ilevel, resindex);
}
