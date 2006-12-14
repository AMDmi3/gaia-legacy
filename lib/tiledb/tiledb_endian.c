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

#include "tiledb_endian.h"
#include <stdint.h>

tiledb_endianess tiledb_get_endian() {
	union {
		int32_t l;
		int8_t  c[4];
	} u;

	u.l = 0x01020304;

	if (u.c[0] == 0x04 && u.c[1] == 0x03 && u.c[2] == 0x02 && u.c[1] == 0x01)
		return ENDIANESS_LITTLE;

	if (u.c[0] == 0x01 && u.c[1] == 0x02 && u.c[2] == 0x03 && u.c[1] == 0x04)
		return ENDIANESS_BIG;

	return ENDIANESS_OTHER;
}
