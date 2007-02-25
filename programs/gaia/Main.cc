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
#include "ControlWidget.h"

#include <QApplication>
#include <QGLFormat>
#include <QSplitter>

using namespace gaia;

int main(int argc, char **argv) {
	QApplication app(argc,argv);

	if (!QGLFormat::hasOpenGL()) {
		qWarning( "This system has no OpenGL support. Exiting." );
		return 1;
	}

	QSplitter splitter;

	GLWidget *gl = new GLWidget(&splitter);
	splitter.insertWidget(0, new ControlWidget(gl, &splitter));
	splitter.setStretchFactor(1, 1);
	splitter.resize(800, 600);
	splitter.show();

	return app.exec();
}
