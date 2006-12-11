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

#include "MainWidget.h"
#include "GLWidget.h"
#include "ControlWidget.h"

MainWidget::MainWidget(QWidget* parent, const char* name): QSplitter(Qt::Horizontal, parent, name) {
	/* create control widget (left bar) */
	ControlWidget *cw = new ControlWidget(this);

	setResizeMode(cw, QSplitter::KeepSize);

	/* create gl widget */
	GLWidget* gl = new GLWidget(this);
}
