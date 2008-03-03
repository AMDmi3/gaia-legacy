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

#ifndef __GAIA__GLWIDGET_H__
#define __GAIA__GLWIDGET_H__

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTime>

#include "EarthView.h"
#include "Layer.h"

namespace gaia {

/**
 * Widget with earth view
 */
class GLWidget: public QGLWidget {
	Q_OBJECT
public:
	GLWidget(QWidget* parent = 0);
	~GLWidget();

public:
	/* methods to call from ControlWidget */
	void SetFlatEarthView();
	void SetGlobeEarthView();
	void ActivateLayer(LayerMeta *meta);
	void DeactivateLayer(LayerMeta *meta);
	void MoveToPosition(double x, double y);

protected:
	/* GL stuff */
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	/* events */
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

private:
	EarthView	*m_EarthView;		///< EarthView to render data with
	int		m_MouseDownMask;	///< Bitmask containing info on which mouse buttons are currently down
	QPoint		m_MouseLeftAnchor;	///< Where left mouse button was pressed
	QPoint		m_MouseMidAnchor;	///< Where middle mouse button was pressed
	QPoint		m_MouseRightAnchor;	///< Where right mouse button was pressed

	QTime		m_LastRender;
};

} /* namespace gaia */

#endif
