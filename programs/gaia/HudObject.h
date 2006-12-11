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

#ifndef __GAIA__HUDOBJECT_H__
#define __GAIA__HUDOBJECT_H__

#include "Math.h"

#define HUDFLAG_INBOUND		0x01
#define HUDFLAG_KILLUNDER	0x02
#define HUDFLAG_FADEUNDER	0x04

namespace gaia {

/**
 * One object on hud
 *
 * May
 */
class HudObject {
public:
	/**
	 * Constructor
	 *
	 * @param pos object position on screen coordinates
	 * @param size object size in pixels
	 * @param center coordinate of objects's center in pixels
	 * @param layer determines order of object rendering (has nothing to do with geo Layers)
	 * @param flags flags to determine object behavior
	 */
	HudObject(Vector2i pos, Vector2i size, Vector2i center, int layer, int flags): m_Size(size), m_Center(center), m_Pos(pos), m_Layer(layer), m_Flags(flags) {}

	/**
	 * Destructor
	 */
	virtual ~HudObject() {}

	/**
	 * Correct (if needed) object position and check whether it's visible
	 *
	 * @return 1 if object is visible on the screen, 0 otherwise
	 */
	int CheckBounds(int width, int height);

	/**
	 * Check if object intersects with another object
	 */
	int IsShaded(HudObject *target);

	/**
	 * Check one of object's flags
	 */
	int CheckFlag(int f);

	/**
	 * Return layer of an object
	 */
	int GetLayer();

	/**
	 * Renders object
	 */
	virtual void Render() = 0;

protected:
	Vector2i m_Size;
	Vector2i m_Center;
	Vector2i m_Pos;
	int	m_Layer;
	int	m_Flags;

	friend int HudObject_Compare(const void *, const void *);
};

int HudObject_Compare(const void *a, const void *b);

} /* namespace gaia */

#endif
