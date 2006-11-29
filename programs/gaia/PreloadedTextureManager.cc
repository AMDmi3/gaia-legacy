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

#include "PreloadedTextureManager.h"

PreloadedTextureManager *PreloadedTextureManager::m_Instance = 0;

PreloadedTextureManager *PreloadedTextureManager::Instance() {
	if (!m_Instance)
		m_Instance = new PreloadedTextureManager;

	return m_Instance;
}

PreloadedTextureManager::PreloadedTextureManager() {
}

PreloadedTextureManager::~PreloadedTextureManager() {
	Clear();
}

void PreloadedTextureManager::SetTexture(int id) {
	if (m_PreloadedTextures.count(id) != 1)
		throw Exception("texture not found");

	m_PreloadedTextures.find(id)->second->SetTexture();
}

void PreloadedTextureManager::LoadJPEG(int id, char *filename) {
	if (m_PreloadedTextures.count(id) > 0)
		throw Exception("texture already loaded");


	Texture *tex = new Texture();

	try {
		tex->LoadJPEG(TEXTURE_SOURCE_FILE, filename);
	} catch(...) {
		delete tex;
		throw;
	}

	m_PreloadedTextures.insert(std::make_pair(id, tex));
}

void PreloadedTextureManager::LoadPNG(int id, char *filename) {
	if (m_PreloadedTextures.count(id) > 0)
		throw Exception("texture already loaded");

	Texture *tex = new Texture();

	try {
		tex->LoadPNG(TEXTURE_SOURCE_FILE, filename);
	} catch(...) {
		delete tex;
		throw;
	}

	m_PreloadedTextures.insert(std::make_pair(id, tex));
}

void PreloadedTextureManager::Clear() {
	for (std::map <int, Texture*>::iterator i = m_PreloadedTextures.begin(); i != m_PreloadedTextures.end(); i++) 
		delete i->second;

	m_PreloadedTextures.clear();
}
