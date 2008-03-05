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

#ifndef __LIBGAIA__GLOBEEARTHVIEW_H__
#define __LIBGAIA__GLOBEEARTHVIEW_H__

#include "EarthRenderer.h"

namespace gaia {

/// Flat representation of Earth.
///
/// Simplest possible earth representation. Orthogonal projection, pitch
/// cannot be changed and north is always 'up'.
class GlobeEarthRenderer: public EarthRenderer {
public:
	/// Constructor.
	GlobeEarthRenderer(EarthRenderer *ancestor = 0);

	/// Destructor.
	virtual ~GlobeEarthRenderer();

	/// Renders one frame.
	void Render();

	/// Animates view.
	///
	/// @param delta time since last frame
	void Animate(double delta);

	int StartMovement(int flags);
	int StopMovement(int flags);
	int SingleMovement(int flags);

private:
	/// Fixes eye coordinates after movements
	void NormalizeEye();

protected:
	int	current_movement_flags_;
};

} // namespace gaia

#endif
