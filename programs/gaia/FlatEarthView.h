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

#ifndef __GAIA__FLATEARTHVIEW_H__
#define __GAIA__FLATEARTHVIEW_H__

#include "PreloadedTextureManager.h"
#include "EarthView.h"

namespace gaia {

/**
 * Flat representation of Earth.
 *
 * Simplest possible earth representation. Orthogonal projection, pitch
 * cannot be changed and north is always 'up'.
 */
class FlatEarthView: public EarthView {
public:
	/**
	 * Constructor.
	 */
	FlatEarthView();

	/**
	 * Destructor.
	 */
	virtual ~FlatEarthView();

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

	int StartDrag(int x, int y, int flags);
	int Drag(int fromx, int fromy, int x, int y, int flags);

	int StartMovement(int flags);
	int StopMovement(int flags);
	int SingleMovement(int flags);

private:
	/**
	 * Fix eye coordinates after movements
	 */
	void NormalizeEye();

	/**
	 * Calculate aspect ratio correction value
	 */
	double AspectCorrection();

	/**
	 * Calculate aspect ratio correction value for given latitude
	 */
	double AspectCorrection(double y);

protected:
	int		m_CurrentMovementFlags;
	Eye		m_SavedZoomEye;
	Eye		m_SavedPanEye;
};

} /* namespace gaia */

#endif
