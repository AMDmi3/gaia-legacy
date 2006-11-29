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

#ifndef __GAIA__TILESTORAGE_H__
#define __GAIA__TILESTORAGE_H__

#include "Tile.h"

/**
 * Abstract tile storage.
 *
 * Tile storages deal with loading and saving of all tiles.
 *
 * @deprecated should be restructured
 */
class TileStorage {
public:
	/**
	 * Constructor.
	 */
	TileStorage() {}

	/**
	 * Destructor.
	 */
	virtual ~TileStorage() {}

	/**
	 * Start procesing of tile, whatever that means.
	 */
	virtual void Enqueue(TilePtr tile) = 0;
};

#endif
