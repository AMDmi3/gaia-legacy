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

#include "PreloadedTextureManager.h"
#include "Data.h"
//Added by qt3to4:
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>

namespace gaia {

GLWidget::GLWidget(QWidget* parent): QGLWidget(parent) {
	/* init preloaded texture manager */
	PreloadedTextureManager::Instance()->LoadPNG(TEXTURE_FONT, DATADIR "/font.png");

	/* create default EarthView */
	m_EarthView = new FlatEarthView();

	/* create default Layers */
	for (LayerMeta *meta = LayerMeta::first; meta; meta = meta->next)
		if (meta->initiallyactive)
			m_EarthView->ActivateLayer(meta);

	m_MouseDownMask = 0;

//	SetMouseTracking(1);
	setFocusPolicy(Qt::WheelFocus);
}

GLWidget::~GLWidget() {
	delete m_EarthView;
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_EarthView) {
		m_EarthView->Resize(width(), height());
		m_EarthView->Render();
	}
}

void GLWidget::initializeGL() {
	qglClearColor(Qt::black);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
}

/* events */
void GLWidget::mousePressEvent(QMouseEvent *e) {
	m_MouseDownMask |= e->button();

	switch(e->button()) {
	case Qt::LeftButton:
		m_MouseLeftAnchor = e->pos();
		m_EarthView->StartDrag(e->x(), e->y(), NAV_DRAG_PAN);
		break;
	case Qt::MidButton:
		m_MouseMidAnchor = e->pos();
		break;
	case Qt::RightButton:
		m_MouseRightAnchor = e->pos();
		m_EarthView->StartDrag(e->x(), e->y(), NAV_DRAG_ZOOM);
		break;
	default:
		break;
	}

	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	m_MouseDownMask &= ~e->button();

	switch(e->button()) {
	case Qt::LeftButton:
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}

	updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
	if (m_MouseDownMask & Qt::LeftButton)
		m_EarthView->Drag(m_MouseLeftAnchor.x(), m_MouseLeftAnchor.y(), e->x(), e->y(), NAV_DRAG_PAN);
	if (m_MouseDownMask & Qt::RightButton)
		m_EarthView->Drag(m_MouseRightAnchor.x(), m_MouseRightAnchor.y(), e->x(), e->y(), NAV_DRAG_ZOOM);

	updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *e) {
	if (e->delta() > 0)
		m_EarthView->SingleMovement(NAV_ZOOM_IN);
	else
		m_EarthView->SingleMovement(NAV_ZOOM_OUT);

	updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
	switch(e->key()) {
		case Qt::Key_Left:
			m_EarthView->StartMovement(NAV_PAN_LEFT);
			break;
		case Qt::Key_Right:
			m_EarthView->StartMovement(NAV_PAN_RIGHT);
			break;
		case Qt::Key_Down:
			m_EarthView->StartMovement(NAV_PAN_DOWN);
			break;
		case Qt::Key_Up:
			m_EarthView->StartMovement(NAV_PAN_UP);
			break;
		case Qt::Key_PageUp:
		case Qt::Key_Equal:
			m_EarthView->StartMovement(NAV_ZOOM_IN);
			break;
		case Qt::Key_PageDown:
		case Qt::Key_Minus:
			m_EarthView->StartMovement(NAV_ZOOM_OUT);
			break;
		default:
			break;
	}

	updateGL();
}

void GLWidget::keyReleaseEvent(QKeyEvent *e) {
	switch(e->key()) {
		case Qt::Key_Left:
			m_EarthView->StartMovement(NAV_PAN_LEFT);
			break;
		case Qt::Key_Right:
			m_EarthView->StartMovement(NAV_PAN_RIGHT);
			break;
		case Qt::Key_Down:
			m_EarthView->StartMovement(NAV_PAN_DOWN);
			break;
		case Qt::Key_Up:
			m_EarthView->StartMovement(NAV_PAN_UP);
			break;
		case Qt::Key_PageUp:
		case Qt::Key_Equal:
			m_EarthView->StartMovement(NAV_ZOOM_IN);
			break;
		case Qt::Key_PageDown:
		case Qt::Key_Minus:
			m_EarthView->StartMovement(NAV_ZOOM_OUT);
			break;
		default:
			break;
	}

	updateGL();
}

void GLWidget::SetFlatEarthView() {
	EarthView *newearthview = new FlatEarthView(m_EarthView);

	delete m_EarthView;

	m_EarthView = newearthview;
}

void GLWidget::SetGlobeEarthView() {
	EarthView *newearthview = new GlobeEarthView(m_EarthView);

	delete m_EarthView;

	m_EarthView = newearthview;
}

void GLWidget::ActivateLayer(LayerMeta *meta) {
	m_EarthView->ActivateLayer(meta);

	updateGL();
}

void GLWidget::DeactivateLayer(LayerMeta *meta) {
	m_EarthView->DeactivateLayer(meta);

	updateGL();
}

void GLWidget::MoveToPosition(double x, double y) {
	m_EarthView->MoveToPosition(x, y);

	updateGL();
}

} /* namespace gaia */
