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

#ifndef __GAIA__WORLDWINDTILEMANAGER_H__
#define __GAIA__WORLDWINDTILEMANAGER_H__

#include <map>

#include "Messages.h"
#include "TextureTile.h"
#include "TileStorage.h"

#define DEFAULT_MAX_TEXTURES 100

namespace gaia {

/**
 * Tile manager.
 *
 * Handles requests for tiles, starts retrival of needed tiles from
 * storages, removes unneeded tiles from memory.
 *
 * @deprecated should be restructured
 */
class WorldWindTileManager {
public:
	/**
	 * Constructor.
	 *
	 * @param ts tile storage to pass tile to for loading
	 */
	WorldWindTileManager(TileStorage *ts);

	/**
	 * Destructor.
	 */
	virtual ~WorldWindTileManager();

	/**
	 * Gets texture tile with given coords.
	 */
	TextureTilePtr GetTexture(int x, int y, int level);

	/**
	 * Removes unneeded objects from memory.
	 *
	 * Currently removes one texture each call, until number
	 * of textures >= DEFAULT_MAX_TEXTURES
	 *
	 * @todo implement tunable limits
	 */
	int Cleanup();

private:
	/**
	 * Finds best texture tile to remove from memory.
	 */
	TextureTilePtr FindTextureToDrop(TextureTilePtr cur, TextureTilePtr best);

private:
	TileStorage	*m_FirstTileStorage;	///< Tile storage to pass tile to for loading

	int		m_nTextureTiles;	///< Root element of TextureTile quadtree
	TextureTilePtr	m_TextureRoot;		///< Number of texture tiles in a tree
};

} /* namespace gaia */

#endif
