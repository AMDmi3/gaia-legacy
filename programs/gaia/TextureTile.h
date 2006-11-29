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

#ifndef __GAIA__TEXTURETILE_H__
#define __GAIA__TEXTURETILE_H__

#include "Tile.h"
#include "Texture.h"

class TextureTile;
typedef SmartPtr<TextureTile> TextureTilePtr;

/**
 * Tile containing one image of earth surface.
 *
 * Texture tiles are intended to be organized as quadtree.
 * TextureTile is wrapper around actual Texture object.
 */
class TextureTile: public Tile {
	//friend class	SmartPtr<Tile>;
	friend class	SmartPtr<TextureTile>;
	friend class	WorldWindTileManager;	/* XXX: don't do that!!! */
public:
	/**
	 * Constructor. Creates empty TextureTile.
	 *
	 * @param x x coordinate of tile
	 * @param y y coordinate of tile
	 * @param level level of tile
	 * @param parent parent in quadtree (NULL for root)
	 */
	TextureTile(int x, int y, int level, TextureTilePtr parent);

	/**
	 * Destructor.
	 */
	virtual ~TextureTile();

	/**
	 * Get type of a tile (here TILETYPE_TEXTURE).
	 */
	virtual int GetType();

	/**
	 * Get parent in quadtree
	 */
	TextureTilePtr GetParent();

	/**
	 * Load actual texture data into tile.
	 *
	 * See Tile::Load
	 */
	void Load(RawBuffer *data, int keep);

	/**
	 * Test whether this tile is loaded with data.
	 *
	 * See Tile::IsLoaded
	 */
	int IsLoaded();
	/* note: IsReady of base class works perfectly */

	/**
	 * Set texture in OpenGL context.
	 *
	 * See Texture::SetTexture
	 */
	void SetTexture();

	/**
	 * Forcibly unload texture from OpenGL.
	 */
	void Unload();

protected:
	/**
	 * Get child in quadtree.
	 *
	 * /param n child id [0..3]
	 *
	 * Children are numbered counter-clockwise starting from bottom left:
	 * <table>
	 * <tr><td> 3 </td><td> 2 </td></tr>
	 * <tr><td> 0 </td><td> 1 </td></tr>
	 * </table>
	 */
	TextureTilePtr GetChild(int n);

	/**
	 * Get child in quadtree.
	 *
	 * /param x x coordinate (0|1)
	 * /param y y coordinate (0|1)
	 */
	TextureTilePtr GetChild(int x, int y);

	/**
	 * Set (or replace) child in quadtree.
	 *
	 * See GetChild
	 */
	void SetChild(int n, TextureTilePtr child);

	/**
	 * Set (or replace) child in quadtree.
	 *
	 * See GetChild
	 */
	void SetChild(int x, int y, TextureTilePtr child);

	/**
	 * Test whether this tile has no children in quadtree.
	 */
	int IsLeaf();

protected:
	TextureTilePtr	m_Parent;	///< Parent tile in quadtree
	TextureTilePtr	m_Child[4];	///< Child tiles in quadtree

private:
	Texture		*m_Texture;	///< The texture itself
};

#endif
