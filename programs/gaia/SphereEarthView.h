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

#ifndef __GAIA__SPHEREEARTHVIEW_H__
#define __GAIA__SPHEREEARTHVIEW_H__

#include "Viewpoint.h"
#include "MasterLayer.h"
#include "PreloadedTextureManager.h"
#include "EarthView.h"

/**
 * Flat representation of Earth.
 *
 * Simplest possible earth representation. Orthogonal projection, pitch
 * cannot be changed and north is always 'up'.
 */
class SphereEarthView: public EarthView {
public:
	/**
	 * Constructor.
	 */
	SphereEarthView(MasterLayer *ml);

	/**
	 * Destructor.
	 */
	virtual ~SphereEarthView();

	/**
	 * Render one frame.
	 */
	void Render();

	/**
	 * Animate view.
	 * 
	 * @param delta time since last frame
	 */
	void Animate(double delta);

	int StartMovement(int flags);
	int StopMovement(int flags);
	int SingleMovement(int flags);

private:
	/**
	 * Fix eye coordinates after movements
	 */
	void NormalizeEye();

protected:
	int	m_CurrentMovementFlags;
};

#endif
