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

#ifndef __GAIA__PRELOADEDTEXTUREMANAGER_H__
#define __GAIA__PRELOADEDTEXTUREMANAGER_H__

#include <map>

#include "Texture.h"

/**
 * Texture manager for preloaded textures.
 * Used to store texture objects that are loaded on software startup
 * indexed by user-given ID's for convenient usage.
 */
class PreloadedTextureManager {
public: 
	static PreloadedTextureManager* Instance();

	/**
	 * Destructor.
	 *
	 * Destroys all loaded textures.
	 */
	virtual ~PreloadedTextureManager();

	/**
	 * Selects into OpenGL texture by given ID
	 */
	void SetTexture(int id);

	/**
	 * Loads new JPEG texture.
	 */
	void LoadJPEG(int id, char *filename);

	/**
	 * Loads new PNG texture.
	 */
	void LoadPNG(int id, char *filename);

	void Clear();

private:
	/**
	 * Constructor.
	 */
	PreloadedTextureManager();

private:
	static PreloadedTextureManager	*m_Instance;

	std::map <int, Texture*> m_PreloadedTextures;	///< Map to store all loaded textures
};

#endif
