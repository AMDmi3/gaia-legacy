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

#ifndef __GAIA__DATA_H__
#define __GAIA__DATA_H__

enum {
	TEXTURE_FONT
};

#define ICON_GPS_POSITION(x, y, l) \
	new HudIcon(Vector2i(x, y), Vector2i(21, 21), Vector2i(10, 10), l, 0, Vector2d(1.0/256.0, 114.0/256.0), Vector2d(21.0/256.0, 21.0/256.0), TEXTURE_FONT)
#define ICON_GPS_POSITION_INVALID(x, y, l) \
	new HudIcon(Vector2i(x, y), Vector2i(21, 21), Vector2i(10, 10), l, 0, Vector2d(1.0/256.0, 159.0/256.0), Vector2d(21.0/256.0, 21.0/256.0), TEXTURE_FONT)
#define ICON_GPS_POINT(x, y, l) \
	new HudIcon(Vector2i(x, y), Vector2i(7, 7), Vector2i(3, 3), l, 0, Vector2d(29.0/256.0, 114.0/256.0), Vector2d(7.0/256.0, 7.0/256.0), TEXTURE_FONT)

#endif
