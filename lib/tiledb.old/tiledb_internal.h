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

#ifndef __TILEDB_INTERNAL_H__
#define __TILEDB_INTERNAL_H__

#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "tiledb.h"

/* endianess */
enum {
	ENDIANESS_LITTLE,
	ENDIANESS_BIG,
	ENDIANESS_OTHER,
};

/* BE <=> LE swap macros for aligned data */
#define SWAPBYTES_32(a) {						\
	uint32_t _tmp;							\
	_tmp = (uint32_t)a;						\
	((uint8_t *)&a)[0] = ((uint8_t *)&_tmp)[3];			\
	((uint8_t *)&a)[1] = ((uint8_t *)&_tmp)[2];			\
	((uint8_t *)&a)[2] = ((uint8_t *)&_tmp)[1];			\
	((uint8_t *)&a)[3] = ((uint8_t *)&_tmp)[0];			\
}

#define SWAPBYTES_16(a) {						\
	uint16_t _tmp;							\
	_tmp = (uint16_t)a;						\
	((uint8_t *)&a)[0] = ((uint8_t *)&_tmp)[1];			\
	((uint8_t *)&a)[1] = ((uint8_t *)&_tmp)[0];			\
}

/* returns index for specified object */
tiledb_error tiledb_get_index_for(tiledb *handle, unsigned int x, unsigned int y, unsigned short level, tiledb_index **resindex);

/* return standart encoding of x/y/level into string */
tiledb_error tiledb_coords_to_string(int x, int y, int level, char *buffer, size_t buffersize);

#endif
