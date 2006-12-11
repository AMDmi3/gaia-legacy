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

#include "HudIcon.h"

namespace gaia {

void HudIcon::Render() {
	Vector2i v0 = m_Pos - m_Center;
	Vector2i v1 = m_Pos - m_Center + m_Size;

	Vector2d t0 = m_TexturePos;
	Vector2d t1 = m_TexturePos + m_TextureSize;

	PreloadedTextureManager::Instance()->SetTexture(m_TextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(t0.x, t0.y);
	glVertex3i(v0.x, v0.y, 0);
	glTexCoord2d(t0.x, t1.y);
	glVertex3i(v0.x, v1.y, 0);
	glTexCoord2d(t1.x, t1.y);
	glVertex3i(v1.x, v1.y, 0);
	glTexCoord2d(t1.x, t0.y);
	glVertex3i(v1.x, v0.y, 0);
	glEnd();
}

} /* namespace gaia */
