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

#ifndef __LIBGAIA__REGION_H__
#define __LIBGAIA__REGION_H__

#include <GL/gl.h>

#include <libgaia/Math.h>

namespace gaia {

/// Minimal renderable part of Earth
///
/// Region is rectangular piece of earth surface, with sides parallel
/// to parallels and meridians. Thus, it has only world coordinates
/// w[0] is lower left and w[1] is top right (height is likely to be
/// added as well). However, this rectangle may be rendered to screen
/// in almost arbitary way, (twisted, skewed, rotated etc.) - so virtual
/// coordinates for each vertex of the rectangle are also stored (those
/// are used in OpenGL rendering). Vertices are counted [0..3], starting
/// with bottom-left (that one thet is w[0]), CCW. Projected coordinates
/// are stored as well. Those are useful for HUD icons, for example.
class Region {
public:
	/// Constructor
	Region();

	/// Constructor with all put projected coords given
	Region(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d v3, Vector2d w0, Vector2d w1);

	/// Constructor with all coordinates given
	Region(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d v3, Vector2d w0, Vector2d w1, Vector3d p0, Vector3d p1, Vector3d p2, Vector3d p3);

	/// Calculate projected coordinates
	void calc_proj(const GLdouble *model, const GLdouble *proj, const GLint *view);

	/// Reset Z-values of projected coords
	void reset_proj_z();

	/// Length of edge (i,j)
	double proj_length(int i, int j);

public:
	/// Virtual coordinates (used in rendering)
	Vector3d v[4];

	/// World coordinates (corners of parallel/meridian - oriented rectangle)
	Vector2d w[2];

	/// Virtual coordinates after projection to screen (in pixels)
	Vector3d p[4];

};

} // namespace gaia

#endif
