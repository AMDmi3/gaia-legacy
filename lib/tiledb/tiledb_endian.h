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

#ifndef _TILEDB_ENDIAN_H_
#define _TILEDB_ENDIAN_H_

#include "tiledb_config.h"

/* BE <=> LE swap macros for aligned data */
#define SWAPBYTES_32(a) (		\
	(a&0x000000ff)<<24 |		\
	(a&0x0000ff00)<< 8 |		\
	(a&0x00ff0000)>> 8 |		\
	(a&0xff000000)>>24)

tiledb_endianess tiledb_get_endian();

#define tiledb_switch_int(db_handle, value) (				\
	((db_handle)->db_endianess != (db_handle)->pc_endianess)?	\
		(value):SWAPBYTES_32(value))

#endif
