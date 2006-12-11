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

#ifndef __GAIA__HUD_H__
#define __GAIA__HUD_H__

#include <vector>
#include <GL/gl.h>

#include "HudObject.h"

namespace gaia {

/**
 * Displays icons and text over 3d view
 *
 * XXX: THIS SHOULD NOT BE A SINGLETON!
 * Singletonizing HUD is tempotal measure, until I find clear way of passing
 * Hud object to EarthView's and Layers. I expect for good inter-(hud/view/layer)
 * interface to coe into my mind after more work on SphereEarthView, and correct
 * navigation
 */
class Hud {
public:
	static Hud *Instance();

	/**
	 * Destructor
	 */
	virtual ~Hud();

	/**
	 * Add one object for rendering at next frame
	 */
	void AddObject(HudObject *o);

	/**
	 * Render all objects and delete them afterwards
	 */
	void Render(int width, int height);

private:
	/**
	 * Constructor
	 */
	Hud();

	static Hud	*m_Instance;

protected:
	std::vector<HudObject*>	m_Objects;	///< Array of objects
};

} /* namespace gaia */

#endif
