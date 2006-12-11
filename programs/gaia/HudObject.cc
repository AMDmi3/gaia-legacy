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

#include "HudObject.h"

namespace gaia {

int HudObject::CheckBounds(int width, int height) {
	Vector2i corner0 = m_Pos - m_Center;
	Vector2i corner1 = m_Pos - m_Center + m_Size;

	if (m_Flags & HUDFLAG_INBOUND) {
		if (corner0.x < 0)	m_Pos.x -= corner0.x;
		if (corner0.y < 0)	m_Pos.y -= corner0.y;

		if (corner1.x >= width)		m_Pos.x += width - corner1.x;
		if (corner1.y >= height)	m_Pos.y += height - corner1.y;
	} else if (corner1.x < 0 || corner1.y < 0 || corner0.x >= width || corner0.y >= height)
		return 0;

	return 1;
}

int HudObject::IsShaded(HudObject *target) {
	Vector2i corner0 = m_Pos - m_Center;
	Vector2i corner1 = m_Pos - m_Center + m_Size;

	Vector2i tcorner0 = target->m_Pos - target->m_Center;
	Vector2i tcorner1 = target->m_Pos - target->m_Center + target->m_Size;

	if (corner1.x < tcorner0.x || corner0.x > tcorner1.x || corner1.y < tcorner0.y || corner0.y > tcorner1.y)
		return 0;

	return 1;
}

int HudObject::CheckFlag(int f) {
	return m_Flags & f;
}

int HudObject::GetLayer() {
	return m_Layer;
}

int HudObject_Compare(const void *a, const void *b) {
	HudObject *aa = *((HudObject**)a);
	HudObject *bb = *((HudObject**)b);
	if (aa->m_Layer < bb->m_Layer)
		return -1;
	if (aa->m_Layer > bb->m_Layer)
		return 1;
	return 0;
}

} /* namespace gaia */
