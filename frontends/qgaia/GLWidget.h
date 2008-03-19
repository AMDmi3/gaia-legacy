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

#ifndef __QGAIA__GLWIDGET_H__
#define __QGAIA__GLWIDGET_H__

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTime>

#include <libgaia/EarthRenderer.h>

namespace qgaia {

/// Widget with earth view.
class GLWidget: public QGLWidget {
	Q_OBJECT
public:
	GLWidget(QWidget* parent = 0);
	~GLWidget();

public:
	// methods to call from ControlWidget
	void SetFlatEarthRenderer();
	void SetGlobeEarthRenderer();
//	void ActivateLayer(LayerMeta *meta);
//	void DeactivateLayer(LayerMeta *meta);
//	void MoveToPosition(double x, double y);

private:
	// GL stuff
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	// events
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

	/// EarthRenderer to render data with
	gaia::EarthRenderer	*earth_view_;

	/// Bitmask containing info on which mouse buttons are currently down
	int		mouse_down_mask_;

	/// Where left mouse button was pressed
	QPoint		mouse_left_anchor_;

	/// Where middle mouse button was pressed
	QPoint		mouse_mid_anchor_;

	/// Where right mouse button was pressed
	QPoint		mouse_right_anchor_;

	/// Time of previous render
	QTime		last_render_;
};

} // namespace qgaia

#endif
