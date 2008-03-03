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

#include "FlatEarthView.h"

#include "Region.h"
#include "Layer.h"
#include "Math.h"

#include <GL/gl.h>

namespace gaia {

FlatEarthView::FlatEarthView(EarthView *ancestor): EarthView(ancestor) {
	m_CurrentMovementFlags = 0;
}

FlatEarthView::~FlatEarthView() {
}

void FlatEarthView::Render() {
	/* x and y span of viewable size in global coords */
	double yspan = m_Eye.yspan((double)m_ViewportWidth/(double)m_ViewportHeight);
	double xspan = m_Eye.xspan((double)m_ViewportWidth/(double)m_ViewportHeight);

	xspan *= AspectCorrection();

	/* setup projection */
	glMatrixMode(GL_PROJECTION);
	double m[16] = {
		2.0/m_ViewportWidth,	0,			0,	0,
		0,			2.0/m_ViewportHeight,	0,	0,
		0,			0,			1.0,	0,
		-1.0,			-1.0,			0,	1.0,
	};
	glLoadMatrixd(m);

	/* calculate virtual coordinates for sides of world rectangle */
	double world_left_virtual = ((-0.5 - m_Eye.x) * (double)m_ViewportWidth/xspan) + double(m_ViewportWidth)/2.0;
	double world_right_virtual = ((0.5 - m_Eye.x) * (double)m_ViewportWidth/xspan) + double(m_ViewportWidth)/2.0;
	double world_top_virtual = ((0.25 - m_Eye.y) * (double)m_ViewportHeight/yspan) + double(m_ViewportHeight)/2.0;
	double world_bottom_virtual = ((-0.25 - m_Eye.y) * (double)m_ViewportHeight/yspan) + double(m_ViewportHeight)/2.0;

	Region rgn(Vector3d(0, 0, 0),
		Vector3d((double)m_ViewportWidth, 0, 0),
		Vector3d((double)m_ViewportWidth, (double)m_ViewportHeight, 0),
		Vector3d(0, (double)m_ViewportHeight, 0),

		Vector2d(m_Eye.x - xspan/2.0, m_Eye.y - yspan/2.0),
		Vector2d(m_Eye.x + xspan/2.0, m_Eye.y + yspan/2.0),

		Vector3d(0, 0, 0),
		Vector3d((double)m_ViewportWidth, 0, 0),
		Vector3d((double)m_ViewportWidth, (double)m_ViewportHeight, 0),
		Vector3d(0, (double)m_ViewportHeight, 0));

	/* tune coords for the cases where earth bounds appear on screen */
	if (world_left_virtual > 0.0) {
		rgn.v[0].x = rgn.v[3].x = world_left_virtual;
		rgn.p[0].x = rgn.p[3].x = world_left_virtual;
		rgn.w[0].x = -0.5;
	}

	if (world_right_virtual < (double)m_ViewportWidth) {
		rgn.v[1].x = rgn.v[2].x = world_right_virtual;
		rgn.p[1].x = rgn.p[2].x = world_right_virtual;
		rgn.w[1].x = 0.5;
	}

	if (world_bottom_virtual > 0.0) {
		rgn.v[0].y = rgn.v[1].y = world_bottom_virtual;
		rgn.p[0].y = rgn.p[1].y = world_bottom_virtual;
		rgn.w[0].y = -0.25;
	}

	if (world_top_virtual < (double)m_ViewportHeight) {
		rgn.v[2].y = rgn.v[3].y = world_top_virtual;
		rgn.p[2].y = rgn.p[3].y = world_top_virtual;
		rgn.w[1].y = 0.25;
	}

	/* call layers rendering routines */
	for (std::vector<Layer*>::iterator i = m_Layers.begin(); i < m_Layers.end(); i++)
		(*i)->RenderRegion(&rgn);
}

void FlatEarthView::Animate(double delta) {
	if (m_CurrentMovementFlags & NAV_ZOOM_IN)
		m_Eye.h *= delta < 1.0 ? 1.0 - delta : 0.1;
	if (m_CurrentMovementFlags & NAV_ZOOM_OUT)
		m_Eye.h *= 1.0 + delta;
	if (m_CurrentMovementFlags & NAV_PAN_UP)
		m_Eye.y += m_Eye.h * delta * 0.7;
	if (m_CurrentMovementFlags & NAV_PAN_DOWN)
		m_Eye.y -= m_Eye.h * delta * 0.7;
	if (m_CurrentMovementFlags & NAV_PAN_LEFT)
		m_Eye.x -= m_Eye.h * delta * 0.7 * AspectCorrection();
	if (m_CurrentMovementFlags & NAV_PAN_RIGHT)
		m_Eye.x += m_Eye.h * delta * 0.7 * AspectCorrection();

	NormalizeEye();
}

/* mouse navigation */
int FlatEarthView::StartDrag(int x, int y, int flags) {
	if (flags & NAV_DRAG_PAN)
		m_SavedPanEye = m_Eye;
	if (flags & NAV_DRAG_ZOOM)
		m_SavedZoomEye = m_Eye;

	return 1;
}

int FlatEarthView::Drag(int fromx, int fromy, int x, int y, int flags) {
	double yspan = m_Eye.yspan((double)m_ViewportWidth/(double)m_ViewportHeight);
	double xspan = m_Eye.xspan((double)m_ViewportWidth/(double)m_ViewportHeight) * AspectCorrection();;

	if (flags & NAV_DRAG_PAN) {
		m_Eye.y = m_SavedPanEye.y + double(y - fromy)/double(m_ViewportHeight)*yspan;
		m_Eye.x = m_SavedPanEye.x - double(x - fromx)/double(m_ViewportWidth)*xspan;	/* not exactly correct, but usable */
	}
	if (flags & NAV_DRAG_ZOOM) {
		if (y - fromy < 0)
			m_Eye.h = m_SavedZoomEye.h * (1.0 + double(y - fromy)/double(m_ViewportHeight));
		else 
			m_Eye.h = m_SavedZoomEye.h / (1.0 - double(y - fromy)/double(m_ViewportHeight));
	}

	return 1;
}

/* keyboard navigation */
int FlatEarthView::StartMovement(int flags) {
	m_CurrentMovementFlags |= flags;
	return 1;
}

int FlatEarthView::StopMovement(int flags) {
	m_CurrentMovementFlags &= ~flags;
	return 1;
}

int FlatEarthView::SingleMovement(int flags) {
	if (flags & NAV_ZOOM_IN)
		m_Eye.h /= 1.3;
	if (flags & NAV_ZOOM_OUT)
		m_Eye.h *= 1.3;
	NormalizeEye();

	return 1;
}

/* private flat-specific functions */
void FlatEarthView::NormalizeEye() {
	if (m_Eye.x < -0.5)	m_Eye.x = -0.5;
	if (m_Eye.x > 0.5)	m_Eye.x = 0.5;

	if (m_Eye.y < -0.25)	m_Eye.y = -0.25;
	if (m_Eye.y > 0.25)	m_Eye.y = 0.25;

#define MIN_HEIGHT 10.0/40000000.0
#define MAX_HEIGHT 1.0
	if (m_Eye.h < MIN_HEIGHT)	m_Eye.h = MIN_HEIGHT;
	if (m_Eye.h > MAX_HEIGHT)	m_Eye.h = MAX_HEIGHT;
}

double FlatEarthView::AspectCorrection() {
	double yspan = m_Eye.yspan((double)m_ViewportWidth/(double)m_ViewportHeight);

	/* we need this for flat view model, because the closer we are to poles,
	 * the more stretched (in longitude axis) earth surface will be. The stretch
	 * is proportional to cos(abs(latitude)), so we'll correct it, but no more
	 * that 6x. This will give us undeformed surface for latitudes [-80..80],
	 * which should be enough */
	double k = 1.0;
	if (m_Eye.y - yspan/2.0 >= 0.0)	k = 1.0/cos((m_Eye.y - yspan/2.0)*2.0*M_PI);
	if (m_Eye.y + yspan/2.0 <= 0.0)	k = 1.0/cos((-m_Eye.y - yspan/2.0)*2.0*M_PI);
	if (k > 6.0)	k = 6.0;
	if (k < 1.0)	k = 1.0;

	return k;
}

double FlatEarthView::AspectCorrection(double y) {
	double yspan = m_Eye.yspan((double)m_ViewportWidth/(double)m_ViewportHeight);

	double k = 1.0;
	if (y - yspan/2.0 >= 0.0)	k = 1.0/cos((y - yspan/2.0)*2.0*M_PI);
	if (y + yspan/2.0 <= 0.0)	k = 1.0/cos((-y - yspan/2.0)*2.0*M_PI);
	if (k > 6.0)	k = 6.0;
	if (k < 1.0)	k = 1.0;

	return k;
}

} /* namespace gaia */
