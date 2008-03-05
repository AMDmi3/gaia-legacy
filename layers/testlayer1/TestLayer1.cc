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

#include <GL/gl.h>

#include <libgaia/Layer.h>
#include <libgaia/LayerMeta.h>
#include <libgaia/Region.h>

namespace gaia {

class TestLayer1 : public Layer {
public:
	TestLayer1() {}
	virtual ~TestLayer1() {}

	virtual void RenderRegion(Region *rgn);
};
REGISTER_GAIA_LAYER(TestLayer1, LayerMeta::LAYERGROUP_TRANSPARENT, true);

void TestLayer1::RenderRegion(Region *rgn) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rgn->v[0].x, rgn->v[0].y, rgn->v[0].z);
	glVertex3f(rgn->v[1].x, rgn->v[1].y, rgn->v[1].z);
	glVertex3f(rgn->v[2].x, rgn->v[2].y, rgn->v[2].z);
	glVertex3f(rgn->v[3].x, rgn->v[3].y, rgn->v[3].z);
	glEnd();
}

} // namespace gaia
