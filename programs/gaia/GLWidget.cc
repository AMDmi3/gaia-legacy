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

#include "GLWidget.h"

GLWidget::GLWidget(QWidget* parent, const char* name): QGLWidget(parent, name) {
}

GLWidget::~GLWidget() {
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_LOOP);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glEnd();
}

void GLWidget::initializeGL() {
	qglClearColor(black);
}

void GLWidget::resizeGL( int w, int h ) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
}
