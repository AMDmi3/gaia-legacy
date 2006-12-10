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

#ifndef __TILEDB_H__
#define __TILEDB_H__

#define TILEDB_FLAG_BIGENDIAN	0x01

/* error codes */
typedef enum {
	/* no error */
	TILEDB_OK = 0,

	/* malloc failed */
	TILEDB_MALLOC_FAILED,

	/* internal buffer too small */
	TILEDB_SMALL_BUFFER,

	/* syscall error */
	TILEDB_SYSCALL_ERROR,

	/* requested data was not found */
	TILEDB_NOT_FOUND,

	/* only big/little andian archs supported */
	TILEDB_UNSUPPORTED_ENDIANESS,

	/* only big/little andian archs supported */
	TILEDB_CORRUPT_DATABASE,
} tiledb_error;

/* index */
typedef struct tiledb_index_entry {
	uint32_t	x;
	uint32_t	y;
	uint16_t	level;
	uint16_t	type;
	uint32_t	mtime;
	uint32_t	offset;
	uint32_t	size;
} tiledb_index_entry;

typedef struct tiledb_index tiledb_index;

struct tiledb_index {
	unsigned int	x;
	unsigned int	y;
	unsigned short	level;

	unsigned short	nentries;
	unsigned short	nallocated;
	tiledb_index_entry	*entries;

	tiledb_index	*next;
	tiledb_index	*prev;
};

/* tiledb handle */
typedef struct tiledb {
	char		*path;
	int		lasterrno;
	unsigned int	flags;

	size_t		currentsize;
	unsigned char	*currentdata;

	size_t		indexsize;
	size_t		indexmaxsize;
	tiledb_index	*indexfirst;
	tiledb_index	*indexlast;
} tiledb;

/* open/close */
tiledb_error	tiledb_open(tiledb *handle, char *path);
void		tiledb_close(tiledb *handle);

/* options */
tiledb_error	tiledb_set_limit(tiledb *handle, size_t limit);

/* errors */
const char	*tiledb_strerror(int err);
int		tiledb_getlasterrno(tiledb *handle);

/* single methods */
tiledb_error	tiledb_put(tiledb *handle, unsigned short type, unsigned int x, unsigned int y, unsigned short level, unsigned char *data, size_t size);
tiledb_error	tiledb_get(tiledb *handle, unsigned short type, unsigned int x, unsigned int y, unsigned short level);

/* data retrieving */
size_t		tiledb_get_data_size(tiledb *handle);
unsigned char	*tiledb_get_data_ptr(tiledb *handle);

#endif
