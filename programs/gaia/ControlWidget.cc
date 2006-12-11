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

#include <qheader.h>
#include <qlistview.h>
#include <qradiobutton.h>
#include <qhbuttongroup.h>
#include <qlayout.h>

#include "ControlWidget.h"
#include "GLWidget.h"

ControlWidget::ControlWidget(QWidget* parent, const char* name): QWidget(parent, name) {
	/* view model selector */
	QHButtonGroup *group = new QHButtonGroup("View model", this);

	new QRadioButton("Flat", group);
	new QRadioButton("Globe", group);

	group->setButton(0);

	/* layer list */
	QListView* listview = new QListView(this);
	listview->header()->setClickEnabled(0);
	listview->addColumn("Layer");

	/* layout */
	QVBoxLayout *layout = new QVBoxLayout(this, 2, 2);
	layout->addWidget(group);
	layout->addWidget(listview);
}
