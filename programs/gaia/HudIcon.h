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

#ifndef __GAIA__HUDICON_H__
#define __GAIA__HUDICON_H__

#include "HudObject.h"
#include "PreloadedTextureManager.h"

namespace gaia {

/**
 * Hud icon
 */
class HudIcon: public HudObject {
public:
	HudIcon(Vector2i pos, Vector2i size, Vector2i center, int layer, int flags, Vector2d tpos, Vector2d tsize, int tid) :
		HudObject(pos, size, center, layer, flags),
		m_TexturePos(tpos), m_TextureSize(tsize),
		m_TextureID(tid) {}

	HudIcon(Vector2i pos, int layer, int flags, const HudIcon &t) :
		HudObject(pos, t.m_Size, t.m_Center, layer, flags),
		m_TexturePos(t.m_TexturePos), m_TextureSize(t.m_TextureSize),
		m_TextureID(t.m_TextureID) {}

	virtual ~HudIcon() {}

	void Render();

protected:
	Vector2d	m_TexturePos;
	Vector2d	m_TextureSize;
	int		m_TextureID;
};

} /* namespace gaia */

#endif
