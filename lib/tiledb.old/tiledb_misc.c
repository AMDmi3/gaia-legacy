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

tiledb_error tiledb_coords_to_string(int x, int y, int level, char *buffer, size_t buffersize) {
	char *i = buffer;

	/* form index string */
	for (; level >= 0 && i < buffer + buffersize; level--) {
		int middle = 1 << level;

		if (x < middle && y < middle)
			*i++ = '0';
		else if (x >= middle && y < middle)
			*i++ = '1';
		else if (x < middle && y >= middle)
			*i++ = '3';
		else
			*i++ = '2';

		x %= middle;
		y %= middle;
	}

	if (i > buffer + buffersize - 1)
		return TILEDB_SMALL_BUFFER;

	*i = '\0';

	return TILEDB_OK;
}
