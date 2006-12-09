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

#include "GridLayer.h"

GridLayer::GridLayer() {
}

GridLayer::~GridLayer() {
}

#define EPSILON 0.00001

void GridLayer::RenderRegion(Region *rgn) {
	double xstep = min2(GetGridStep(rgn->w[1].x - rgn->w[0].x, rgn->proj_length(0,1)), GetGridStep(rgn->w[1].x - rgn->w[0].x, rgn->proj_length(2,3)));
	double ystep = min2(GetGridStep(rgn->w[1].y - rgn->w[0].y, rgn->proj_length(1,2)), GetGridStep(rgn->w[1].y - rgn->w[0].y, rgn->proj_length(3,0)));

	int i;

	int left = (int)((rgn->w[0].x + 0.5) / xstep + 1.0 - EPSILON);
	int right = (int)((rgn->w[1].x + 0.5) / xstep + EPSILON);

	int bottom = (int)((rgn->w[0].y + 0.5) / ystep + 1.0 - EPSILON);
	int top = (int)((rgn->w[1].y + 0.5) / ystep + EPSILON);

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	for (i = left; i <= right; i++) {
		double x = (double)i * xstep - 0.5;
		double k = (x - rgn->w[0].x)/(rgn->w[1].x - rgn->w[0].x);

		Vector3d point0 = rgn->v[0] * (1.0 - k) + rgn->v[1] * k;
		Vector3d point1 = rgn->v[3] * (1.0 - k) + rgn->v[2] * k;

		glVertex3d(point0.x, point0.y, point0.z);
		glVertex3d(point1.x, point1.y, point1.z);
	}
	for (i = bottom; i <= top; i++) {
		double y = (double)i * ystep - 0.5;
		double k = (y - rgn->w[0].y)/(rgn->w[1].y - rgn->w[0].y);

		Vector3d point0 = rgn->v[0] * (1.0 - k) + rgn->v[3] * k;
		Vector3d point1 = rgn->v[1] * (1.0 - k) + rgn->v[2] * k;

		glVertex3d(point0.x, point0.y, point0.z);
		glVertex3d(point1.x, point1.y, point1.z);
	}
	glEnd();
}

double GridLayer::GetGridStep(double wlen, double plen) {
	/* XXX: don't use binary division until we reach parts of seconds */
	/* candidate angles to be used:
	 * Degrees: 90 30 10 2 1
	 * Min/sec: 60 30 10 2 1
	 * what to do with large 5x gap between 10 and 2? */

	double grid = 0.25;
	while(1) {
		if (grid/2.0/wlen*plen < MIN_GRID_DISTANCE)
			return grid;
		grid /= 2.0;
	}
}
