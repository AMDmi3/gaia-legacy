/* 
 * Copyright (C) 2006-2008 Dmitry Marakasov
 *
 * This file is part of Gaia.
 *
 * Gaia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gaia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gaia.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GAIA__TILESTORAGE_H__
#define __GAIA__TILESTORAGE_H__

#include "Tile.h"

namespace gaia {

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

} /* namespace gaia */

#endif
