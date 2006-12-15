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

#include "FlatEarthView.h"
#include "GlobeEarthView.h"

#include "TestLayer.h"

namespace gaia {

GLWidget::GLWidget(QWidget* parent, const char* name): QGLWidget(parent, name) {
	m_EarthView = new FlatEarthView();
	m_EarthView->BindLayer(new TestLayer);
}

GLWidget::~GLWidget() {
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_EarthView) {
		m_EarthView->Resize(width(), height());
		m_EarthView->Render();
	}
}

void GLWidget::initializeGL() {
	qglClearColor(black);
}

void GLWidget::SetFlatEarthView() {
	EarthView *newearthview = new FlatEarthView();

	if (m_EarthView)
		delete m_EarthView;

	m_EarthView = newearthview;
	m_EarthView->BindLayer(new TestLayer);

	updateGL();
}

void GLWidget::SetGlobeEarthView() {
	EarthView *newearthview = new GlobeEarthView();

	if (m_EarthView)
		delete m_EarthView;

	m_EarthView = newearthview;
	m_EarthView->BindLayer(new TestLayer);

	updateGL();
}

} /* namespace gaia */
