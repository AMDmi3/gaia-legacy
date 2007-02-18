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

#include "GlobeEarthView.h"

#include "PreloadedTextureManager.h"

namespace gaia {

GlobeEarthView::GlobeEarthView(EarthView *ancestor): EarthView(ancestor) {
}

GlobeEarthView::~GlobeEarthView() {
}

#define XSTEP 15
#define YSTEP 15

void GlobeEarthView::Render() {
	/* setup projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_Eye.yfov((double)m_ViewportWidth/(double)m_ViewportHeight)/M_PI*180.0, (double)m_ViewportWidth/(double)m_ViewportHeight, 0.001, 10);
	glTranslatef(0, 0, -1.0-m_Eye.h);
	glRotatef(-90.0 + m_Eye.y*360.0, 1, 0, 0);
	glRotatef(-m_Eye.x*360.0 + 180.0, 0, 0, 1);


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2,2);

	double model[16];
	double proj[16];
	int view[4];

	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetIntegerv(GL_VIEWPORT, view);

	Region rgn;
	int i, j;
	for (i = -180; i < 180; i += XSTEP) {
		for (j = -90+YSTEP; j < 90-YSTEP; j += YSTEP) {
			rgn.v[0] = Vector3d( -sin((double)i * M_PI/180.0) * cos((double)j * M_PI/180.0),
					cos((double)i * M_PI/180.0) * cos((double)j * M_PI/180.0),
					sin((double)j * M_PI/180.0));

			rgn.v[1] = Vector3d( -sin((double)(i+XSTEP) * M_PI/180.0) * cos((double)j * M_PI/180.0),
					cos((double)(i+XSTEP) * M_PI/180.0) * cos((double)j * M_PI/180.0),
					sin((double)j * M_PI/180.0));

			rgn.v[2] = Vector3d(-sin((double)(i+XSTEP) * M_PI/180.0) * cos((double)(j+YSTEP) * M_PI/180.0),
					cos((double)(i+XSTEP) * M_PI/180.0) * cos((double)(j+YSTEP) * M_PI/180.0),
					sin((double)(j+YSTEP) * M_PI/180.0));

			rgn.v[3] = Vector3d(-sin((double)i * M_PI/180.0) * cos((double)(j+YSTEP) * M_PI/180.0),
					cos((double)i * M_PI/180.0) * cos((double)(j+YSTEP) * M_PI/180.0),
					sin((double)(j+YSTEP) * M_PI/180.0));

			rgn.w[0] = Vector2d((double)i / 360.0, (double)j / 360.0);
			rgn.w[1] = Vector2d((double)(i+XSTEP) / 360.0, (double)(j+YSTEP) / 360.0);

			rgn.calc_proj(model, proj, view);
			rgn.reset_proj_z();

			/* call layers rendering routines */
			for (std::vector<Layer*>::iterator i = m_Layers.begin(); i < m_Layers.end(); i++)
				(*i)->RenderRegion(&rgn);
		}
	}
}

void GlobeEarthView::Animate(double delta) {
	if (m_CurrentMovementFlags & NAV_ZOOM_IN)
		m_Eye.h *= delta < 1.0 ? 1.0 - delta : 0.1;
	if (m_CurrentMovementFlags & NAV_ZOOM_OUT)
		m_Eye.h *= 1.0 + delta;

	if (m_CurrentMovementFlags & NAV_PAN_UP)
		m_Eye.y += m_Eye.h * delta * 0.3;
	if (m_CurrentMovementFlags & NAV_PAN_DOWN)
		m_Eye.y -= m_Eye.h * delta * 0.3;
	if (m_CurrentMovementFlags & NAV_PAN_LEFT)
		m_Eye.x -= m_Eye.h * delta * 0.3;
	if (m_CurrentMovementFlags & NAV_PAN_RIGHT)
		m_Eye.x += m_Eye.h * delta * 0.3;

	NormalizeEye();
}

/* keyboard navigation */
int GlobeEarthView::StartMovement(int flags) {
	m_CurrentMovementFlags |= flags;
	return 1;
}

int GlobeEarthView::StopMovement(int flags) {
	m_CurrentMovementFlags &= ~flags;
	return 1;
}

int GlobeEarthView::SingleMovement(int flags) {
	if (flags & NAV_ZOOM_IN)
		m_Eye.h /= 1.3;

	if (flags & NAV_ZOOM_OUT)
		m_Eye.h *= 1.3;

	NormalizeEye();

	return 1;
}

/* private sphere-specific functions */
void GlobeEarthView::NormalizeEye() {
	for (; m_Eye.x < -0.5; m_Eye.x += 1.0);
	for (; m_Eye.x > 0.5; m_Eye.x -= 1.0);

	if (m_Eye.y < -0.25)	m_Eye.y = -0.25;
	if (m_Eye.y > 0.25)	m_Eye.y = 0.25;

	/* temporary strict limits */
#define MIN_HEIGHT 0.5
#define MAX_HEIGHT 2.0
	if (m_Eye.h < MIN_HEIGHT)	m_Eye.h = MIN_HEIGHT;
	if (m_Eye.h > MAX_HEIGHT)	m_Eye.h = MAX_HEIGHT;
}

} /* namespace gaia */
