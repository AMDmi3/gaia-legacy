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

#include <QTimer>

#include "GLWidget.h"

#include "FlatEarthView.h"
#include "GlobeEarthView.h"

namespace gaia {

GLWidget::GLWidget(QWidget* parent): QGLWidget(parent) {
	// init preloaded texture manager
	//PreloadedTextureManager::Instance()->LoadPNG(TEXTURE_FONT, DATADIR "/font.png");

	// create default EarthView
	earth_view_ = new FlatEarthView();

	// create default Layers
	/*for (LayerMeta *meta = LayerMeta::first; meta; meta = meta->next)
		if (meta->initiallyactive)
			earth_view_->ActivateLayer(meta);*/

	mouse_down_mask_ = 0;

//	SetMouseTracking(1);
	/*setFocusPolicy(Qt::WheelFocus);*/

	last_render_.start();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer->start(30);
}

GLWidget::~GLWidget() {
	delete earth_view_;
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (earth_view_) {
		earth_view_->Animate((double)last_render_.elapsed()/1000.0);
		earth_view_->Resize(width(), height());
		earth_view_->Render();

		last_render_.start();
	}
}

void GLWidget::initializeGL() {
	qglClearColor(Qt::black);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
}

// events
void GLWidget::mousePressEvent(QMouseEvent *e) {
	mouse_down_mask_ |= e->button();

	switch(e->button()) {
	case Qt::LeftButton:
		mouse_left_anchor_ = e->pos();
		earth_view_->StartDrag(e->x(), e->y(), NAV_DRAG_PAN);
		break;
	case Qt::MidButton:
		mouse_mid_anchor_ = e->pos();
		break;
	case Qt::RightButton:
		mouse_right_anchor_ = e->pos();
		earth_view_->StartDrag(e->x(), e->y(), NAV_DRAG_ZOOM);
		break;
	default:
		break;
	}

	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	mouse_down_mask_ &= ~e->button();

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
	if (mouse_down_mask_ & Qt::LeftButton)
		earth_view_->Drag(mouse_left_anchor_.x(), mouse_left_anchor_.y(), e->x(), e->y(), NAV_DRAG_PAN);
	if (mouse_down_mask_ & Qt::RightButton)
		earth_view_->Drag(mouse_right_anchor_.x(), mouse_right_anchor_.y(), e->x(), e->y(), NAV_DRAG_ZOOM);

	updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *e) {
	if (e->delta() > 0)
		earth_view_->SingleMovement(NAV_ZOOM_IN);
	else
		earth_view_->SingleMovement(NAV_ZOOM_OUT);

	updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->isAutoRepeat()) {
		e->ignore();
		return;
	}

	switch(e->key()) {
		case Qt::Key_Left:
			earth_view_->StartMovement(NAV_PAN_LEFT);
			break;
		case Qt::Key_Right:
			earth_view_->StartMovement(NAV_PAN_RIGHT);
			break;
		case Qt::Key_Down:
			earth_view_->StartMovement(NAV_PAN_DOWN);
			break;
		case Qt::Key_Up:
			earth_view_->StartMovement(NAV_PAN_UP);
			break;
		case Qt::Key_PageUp:
		case Qt::Key_Equal:
			earth_view_->StartMovement(NAV_ZOOM_IN);
			break;
		case Qt::Key_PageDown:
		case Qt::Key_Minus:
			earth_view_->StartMovement(NAV_ZOOM_OUT);
			break;
		default:
			e->ignore();
			return;
	}

	e->accept();
	updateGL();
}

void GLWidget::keyReleaseEvent(QKeyEvent *e) {
	if (e->isAutoRepeat()) {
		e->ignore();
		return;
	}

	switch(e->key()) {
		case Qt::Key_Left:
			earth_view_->StopMovement(NAV_PAN_LEFT);
			break;
		case Qt::Key_Right:
			earth_view_->StopMovement(NAV_PAN_RIGHT);
			break;
		case Qt::Key_Down:
			earth_view_->StopMovement(NAV_PAN_DOWN);
			break;
		case Qt::Key_Up:
			earth_view_->StopMovement(NAV_PAN_UP);
			break;
		case Qt::Key_PageUp:
		case Qt::Key_Equal:
			earth_view_->StopMovement(NAV_ZOOM_IN);
			break;
		case Qt::Key_PageDown:
		case Qt::Key_Minus:
			earth_view_->StopMovement(NAV_ZOOM_OUT);
			break;
		default:
			e->ignore();
			return;
	}

	e->accept();
	updateGL();
}

void GLWidget::SetFlatEarthView() {
	EarthView *newearthview = new FlatEarthView(earth_view_);

	delete earth_view_;

	earth_view_ = newearthview;
}

void GLWidget::SetGlobeEarthView() {
	EarthView *newearthview = new GlobeEarthView(earth_view_);

	delete earth_view_;

	earth_view_ = newearthview;
}

/*void GLWidget::ActivateLayer(LayerMeta *meta) {
	earth_view_->ActivateLayer(meta);

	updateGL();
}

void GLWidget::DeactivateLayer(LayerMeta *meta) {
	earth_view_->DeactivateLayer(meta);

	updateGL();
}

void GLWidget::MoveToPosition(double x, double y) {
	earth_view_->MoveToPosition(x, y);

	updateGL();
}*/

} // namespace gaia
