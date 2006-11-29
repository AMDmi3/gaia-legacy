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

#include "TestSlaveLayer.h"

#define RENDER_DIAGONALS

TestSlaveLayer::TestSlaveLayer() {
	m_Caps = SLAVELAYERCAP_OVERDRAW;
}

TestSlaveLayer::~TestSlaveLayer() {
}

void TestSlaveLayer::Overdraw(Region *rgn) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rgn->v[0].x, rgn->v[0].y, rgn->v[0].z);
	glVertex3f(rgn->v[1].x, rgn->v[1].y, rgn->v[1].z);
	glVertex3f(rgn->v[2].x, rgn->v[2].y, rgn->v[2].z);
	glVertex3f(rgn->v[3].x, rgn->v[3].y, rgn->v[3].z);
	glEnd();
#ifdef RENDER_DIAGONALS
	glBegin(GL_LINES);
	glVertex3f(rgn->v[0].x, rgn->v[0].y, rgn->v[0].z);
	glVertex3f(rgn->v[2].x, rgn->v[2].y, rgn->v[2].z);
	glVertex3f(rgn->v[1].x, rgn->v[1].y, rgn->v[1].z);
	glVertex3f(rgn->v[3].x, rgn->v[3].y, rgn->v[3].z);
	glEnd();
#endif
}
