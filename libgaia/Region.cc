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

#include "Region.h"

#include <GL/glu.h>

namespace gaia {

Region::Region() {
}

Region::Region(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d v3, Vector2d w0, Vector2d w1) {
	v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;
	w[0] = w0; w[1] = w1;
}

Region::Region(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d v3, Vector2d w0, Vector2d w1, Vector3d p0, Vector3d p1, Vector3d p2, Vector3d p3) {
	v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3;
	w[0] = w0; w[1] = w1;
	p[0] = p0; p[1] = p1; p[2] = p2; p[3] = p3;
}

void Region::calc_proj(const GLdouble *model, const GLdouble *proj, const GLint *view) {
	gluProject(v[0].x, v[0].y, v[0].z, model, proj, view, &p[0].x, &p[0].y, &p[0].z);
	gluProject(v[1].x, v[1].y, v[1].z, model, proj, view, &p[1].x, &p[1].y, &p[1].z);
	gluProject(v[2].x, v[2].y, v[2].z, model, proj, view, &p[2].x, &p[2].y, &p[2].z);
	gluProject(v[3].x, v[3].y, v[3].z, model, proj, view, &p[3].x, &p[3].y, &p[3].z);

	p[0].z = p[1].z = p[2].z = p[3].z = 0.0;
}

void Region::reset_proj_z() {
	p[0].z = p[1].z = p[2].z = p[3].z = 0.0;
}

double Region::proj_length(int i, int j) {
	return sqrt( (p[i].x - p[j].x) * (p[i].x - p[j].x) + (p[i].y - p[j].y) * (p[i].y - p[j].y) );
}

} // namespace gaia
