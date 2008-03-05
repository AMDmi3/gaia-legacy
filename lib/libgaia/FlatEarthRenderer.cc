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

#include <libgaia/FlatEarthRenderer.h>

#include <GL/gl.h>

#include <libgaia/Region.h>
#include <libgaia/Layer.h>
#include <libgaia/Math.h>

namespace gaia {

FlatEarthRenderer::FlatEarthRenderer(EarthRenderer *ancestor): EarthRenderer(ancestor) {
	current_movement_flags_ = 0;
}

FlatEarthRenderer::~FlatEarthRenderer() {
}

void FlatEarthRenderer::Render() {
	// x and y span of viewable size in global coords
	double yspan = eye_.yspan((double)viewport_width_/(double)viewport_height_);
	double xspan = eye_.xspan((double)viewport_width_/(double)viewport_height_);

	xspan *= AspectCorrection();

	// setup projection
	glMatrixMode(GL_PROJECTION);
	double m[16] = {
		2.0/viewport_width_,	0,			0,	0,
		0,			2.0/viewport_height_,	0,	0,
		0,			0,			1.0,	0,
		-1.0,			-1.0,			0,	1.0,
	};
	glLoadMatrixd(m);

	// calculate virtual coordinates for sides of world rectangle
	double world_left_virtual = ((-0.5 - eye_.x) * (double)viewport_width_/xspan) + double(viewport_width_)/2.0;
	double world_right_virtual = ((0.5 - eye_.x) * (double)viewport_width_/xspan) + double(viewport_width_)/2.0;
	double world_top_virtual = ((0.25 - eye_.y) * (double)viewport_height_/yspan) + double(viewport_height_)/2.0;
	double world_bottom_virtual = ((-0.25 - eye_.y) * (double)viewport_height_/yspan) + double(viewport_height_)/2.0;

	Region rgn(Vector3d(0, 0, 0),
		Vector3d((double)viewport_width_, 0, 0),
		Vector3d((double)viewport_width_, (double)viewport_height_, 0),
		Vector3d(0, (double)viewport_height_, 0),

		Vector2d(eye_.x - xspan/2.0, eye_.y - yspan/2.0),
		Vector2d(eye_.x + xspan/2.0, eye_.y + yspan/2.0),

		Vector3d(0, 0, 0),
		Vector3d((double)viewport_width_, 0, 0),
		Vector3d((double)viewport_width_, (double)viewport_height_, 0),
		Vector3d(0, (double)viewport_height_, 0));

	// tune coords for the cases where earth bounds appear on screen
	if (world_left_virtual > 0.0) {
		rgn.v[0].x = rgn.v[3].x = world_left_virtual;
		rgn.p[0].x = rgn.p[3].x = world_left_virtual;
		rgn.w[0].x = -0.5;
	}

	if (world_right_virtual < (double)viewport_width_) {
		rgn.v[1].x = rgn.v[2].x = world_right_virtual;
		rgn.p[1].x = rgn.p[2].x = world_right_virtual;
		rgn.w[1].x = 0.5;
	}

	if (world_bottom_virtual > 0.0) {
		rgn.v[0].y = rgn.v[1].y = world_bottom_virtual;
		rgn.p[0].y = rgn.p[1].y = world_bottom_virtual;
		rgn.w[0].y = -0.25;
	}

	if (world_top_virtual < (double)viewport_height_) {
		rgn.v[2].y = rgn.v[3].y = world_top_virtual;
		rgn.p[2].y = rgn.p[3].y = world_top_virtual;
		rgn.w[1].y = 0.25;
	}

	// TODO(amdmi3): remove this; for testing only
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rgn.v[0].x, rgn.v[0].y, rgn.v[0].z);
	glVertex3f(rgn.v[1].x, rgn.v[1].y, rgn.v[1].z);
	glVertex3f(rgn.v[2].x, rgn.v[2].y, rgn.v[2].z);
	glVertex3f(rgn.v[3].x, rgn.v[3].y, rgn.v[3].z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(rgn.v[0].x, rgn.v[0].y, rgn.v[0].z);
	glVertex3f(rgn.v[2].x, rgn.v[2].y, rgn.v[2].z);
	glVertex3f(rgn.v[1].x, rgn.v[1].y, rgn.v[1].z);
	glVertex3f(rgn.v[3].x, rgn.v[3].y, rgn.v[3].z);
	glEnd();

	// call layers rendering routines
	for (std::vector<Layer*>::iterator i = layers_.begin(); i < layers_.end(); i++)
		(*i)->RenderRegion(&rgn);
}

void FlatEarthRenderer::Animate(double delta) {
	if (current_movement_flags_ & NAV_ZOOM_IN)
		eye_.h *= delta < 1.0 ? 1.0 - delta : 0.1;
	if (current_movement_flags_ & NAV_ZOOM_OUT)
		eye_.h *= 1.0 + delta;
	if (current_movement_flags_ & NAV_PAN_UP)
		eye_.y += eye_.h * delta * 0.7;
	if (current_movement_flags_ & NAV_PAN_DOWN)
		eye_.y -= eye_.h * delta * 0.7;
	if (current_movement_flags_ & NAV_PAN_LEFT)
		eye_.x -= eye_.h * delta * 0.7 * AspectCorrection();
	if (current_movement_flags_ & NAV_PAN_RIGHT)
		eye_.x += eye_.h * delta * 0.7 * AspectCorrection();

	NormalizeEye();
}

// mouse navigation
int FlatEarthRenderer::StartDrag(int x, int y, int flags) {
	if (flags & NAV_DRAG_PAN)
		saved_pan_eye_ = eye_;
	if (flags & NAV_DRAG_ZOOM)
		saved_zoom_eye_ = eye_;

	return 1;
}

int FlatEarthRenderer::Drag(int fromx, int fromy, int x, int y, int flags) {
	double yspan = eye_.yspan((double)viewport_width_/(double)viewport_height_);
	double xspan = eye_.xspan((double)viewport_width_/(double)viewport_height_) * AspectCorrection();;

	if (flags & NAV_DRAG_PAN) {
		eye_.y = saved_pan_eye_.y + double(y - fromy)/double(viewport_height_)*yspan;
		eye_.x = saved_pan_eye_.x - double(x - fromx)/double(viewport_width_)*xspan;	// not exactly correct, but usable
	}
	if (flags & NAV_DRAG_ZOOM) {
		if (y - fromy < 0)
			eye_.h = saved_zoom_eye_.h * (1.0 + double(y - fromy)/double(viewport_height_));
		else 
			eye_.h = saved_zoom_eye_.h / (1.0 - double(y - fromy)/double(viewport_height_));
	}

	return 1;
}

// keyboard navigation
int FlatEarthRenderer::StartMovement(int flags) {
	current_movement_flags_ |= flags;
	return 1;
}

int FlatEarthRenderer::StopMovement(int flags) {
	current_movement_flags_ &= ~flags;
	return 1;
}

int FlatEarthRenderer::SingleMovement(int flags) {
	if (flags & NAV_ZOOM_IN)
		eye_.h /= 1.3;
	if (flags & NAV_ZOOM_OUT)
		eye_.h *= 1.3;
	NormalizeEye();

	return 1;
}

// private flat-specific functions
void FlatEarthRenderer::NormalizeEye() {
	if (eye_.x < -0.5)	eye_.x = -0.5;
	if (eye_.x > 0.5)	eye_.x = 0.5;

	if (eye_.y < -0.25)	eye_.y = -0.25;
	if (eye_.y > 0.25)	eye_.y = 0.25;

#define MIN_HEIGHT 10.0/40000000.0
#define MAX_HEIGHT 1.0
	if (eye_.h < MIN_HEIGHT)	eye_.h = MIN_HEIGHT;
	if (eye_.h > MAX_HEIGHT)	eye_.h = MAX_HEIGHT;
}

double FlatEarthRenderer::AspectCorrection() {
	double yspan = eye_.yspan((double)viewport_width_/(double)viewport_height_);

	// we need this for flat view model, because the closer we are to poles,
	// the more stretched (in longitude axis) earth surface will be. The stretch
	// is proportional to cos(abs(latitude)), so we'll correct it, but no more
	// that 6x. This will give us undeformed surface for latitudes [-80..80],
	// which should be enough
	double k = 1.0;
	if (eye_.y - yspan/2.0 >= 0.0)	k = 1.0/cos((eye_.y - yspan/2.0)*2.0*M_PI);
	if (eye_.y + yspan/2.0 <= 0.0)	k = 1.0/cos((-eye_.y - yspan/2.0)*2.0*M_PI);
	if (k > 6.0)	k = 6.0;
	if (k < 1.0)	k = 1.0;

	return k;
}

double FlatEarthRenderer::AspectCorrection(double y) {
	double yspan = eye_.yspan((double)viewport_width_/(double)viewport_height_);

	double k = 1.0;
	if (y - yspan/2.0 >= 0.0)	k = 1.0/cos((y - yspan/2.0)*2.0*M_PI);
	if (y + yspan/2.0 <= 0.0)	k = 1.0/cos((-y - yspan/2.0)*2.0*M_PI);
	if (k > 6.0)	k = 6.0;
	if (k < 1.0)	k = 1.0;

	return k;
}

} // namespace gaia
