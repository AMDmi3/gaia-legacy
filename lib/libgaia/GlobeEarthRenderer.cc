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

#include <libgaia/GlobeEarthRenderer.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <libgaia/Region.h>
#include <libgaia/Math.h>
#include <libgaia/Layer.h>

namespace gaia {

GlobeEarthRenderer::GlobeEarthRenderer(EarthRenderer *ancestor): EarthRenderer(ancestor) {
}

GlobeEarthRenderer::~GlobeEarthRenderer() {
}

#define XSTEP 15
#define YSTEP 15

void GlobeEarthRenderer::Render() {
	// setup projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(eye_.yfov((double)viewport_width_/(double)viewport_height_)/M_PI*180.0, (double)viewport_width_/(double)viewport_height_, 0.001, 10);
	glTranslatef(0, 0, -1.0-eye_.h);
	glRotatef(-90.0 + eye_.y*360.0, 1, 0, 0);
	glRotatef(-eye_.x*360.0 + 180.0, 0, 0, 1);


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

			// call layers rendering routines
			for (std::vector<Layer*>::iterator i = layers_.begin(); i < layers_.end(); i++)
				(*i)->RenderRegion(&rgn);
		}
	}
}

void GlobeEarthRenderer::Animate(double delta) {
	if (current_movement_flags_ & NAV_ZOOM_IN)
		eye_.h *= delta < 1.0 ? 1.0 - delta : 0.1;
	if (current_movement_flags_ & NAV_ZOOM_OUT)
		eye_.h *= 1.0 + delta;

	if (current_movement_flags_ & NAV_PAN_UP)
		eye_.y += eye_.h * delta * 0.3;
	if (current_movement_flags_ & NAV_PAN_DOWN)
		eye_.y -= eye_.h * delta * 0.3;
	if (current_movement_flags_ & NAV_PAN_LEFT)
		eye_.x -= eye_.h * delta * 0.3;
	if (current_movement_flags_ & NAV_PAN_RIGHT)
		eye_.x += eye_.h * delta * 0.3;

	NormalizeEye();
}

// keyboard navigation
int GlobeEarthRenderer::StartMovement(int flags) {
	current_movement_flags_ |= flags;
	return 1;
}

int GlobeEarthRenderer::StopMovement(int flags) {
	current_movement_flags_ &= ~flags;
	return 1;
}

int GlobeEarthRenderer::SingleMovement(int flags) {
	if (flags & NAV_ZOOM_IN)
		eye_.h /= 1.3;

	if (flags & NAV_ZOOM_OUT)
		eye_.h *= 1.3;

	NormalizeEye();

	return 1;
}

// private sphere-specific functions
void GlobeEarthRenderer::NormalizeEye() {
	for (; eye_.x < -0.5; eye_.x += 1.0);
	for (; eye_.x > 0.5; eye_.x -= 1.0);

	if (eye_.y < -0.25)	eye_.y = -0.25;
	if (eye_.y > 0.25)	eye_.y = 0.25;

	// temporary strict limits
#define MIN_HEIGHT 0.5
#define MAX_HEIGHT 2.0
	if (eye_.h < MIN_HEIGHT)	eye_.h = MIN_HEIGHT;
	if (eye_.h > MAX_HEIGHT)	eye_.h = MAX_HEIGHT;
}

} // namespace gaia
