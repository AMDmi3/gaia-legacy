/* 
 * Copyright (C) 2006-2008 Dmitry Marakasov
 *
 * This file is part of Gaia.
 *
 * Gaia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gaia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gaia.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIBGAIA__FLATEARTHVIEW_H__
#define __LIBGAIA__FLATEARTHVIEW_H__

#include <libgaia/EarthRenderer.h>

namespace gaia {

/// Flat earth renderer.
///
/// Simplest possible earth representation. Orthogonal projection, pitch
/// cannot be changed and north is always 'up'.
class FlatEarthRenderer: public EarthRenderer {
public:
	/// Constructor.
	FlatEarthRenderer();

	/// Destructor.
	virtual ~FlatEarthRenderer();

	/// Render one frame.
	void Render();

	/// Animate view.
	///
	/// @param delta time since last frame
	void Animate(double delta);

	int StartDrag(int x, int y, int flags);
	int Drag(int fromx, int fromy, int x, int y, int flags);

	int StartMovement(int flags);
	int StopMovement(int flags);
	int SingleMovement(int flags);

private:
	/// Fix eye coordinates after movements
	void NormalizeEye();

	/// Calculate aspect ratio correction value
	double AspectCorrection();

	/// Calculate aspect ratio correction value for given latitude
	double AspectCorrection(double y);

protected:
	int		current_movement_flags_;
	Eye		saved_zoom_eye_;
	Eye		saved_pan_eye_;
};

} // namespace gaia

#endif
