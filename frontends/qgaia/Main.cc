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

#include <QApplication>
#include <QGLFormat>
#include <QSplitter>

#include "GLWidget.h"
#include "SidebarWidget.h"

using namespace qgaia;

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	// OpenGL is essential
	if (!QGLFormat::hasOpenGL()) {
		qWarning( "This system has no OpenGL support. Exiting." );
		return 1;
	}

	QSplitter *splitter = new QSplitter;
	GLWidget *glwidget = new GLWidget;
	SidebarWidget *sidebarwidget = new SidebarWidget(glwidget);

	splitter->addWidget(sidebarwidget);
	splitter->addWidget(glwidget);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 1);
	splitter->resize(800, 600);
	splitter->setWindowTitle("Gaia " GAIA_VERSION);

	glwidget->setFocus(Qt::OtherFocusReason);

	splitter->show();

	return app.exec();
}
