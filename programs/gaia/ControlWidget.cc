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
#include <qpushbutton.h>
#include <qhbuttongroup.h>
#include <qlayout.h>

#include "ControlWidget.h"
#include "GLWidget.h"

#include "WorldWindLayer.h"
#include "GridLayer.h"
#include "GPSLayer.h"
#include "TestLayer.h"

namespace gaia {

ControlWidget::ControlWidget(GLWidget* target, QWidget* parent, const char* name): QWidget(parent, name) {
	/* target GLWidget to control */
	m_GLWidget = target;

	/* view model selector */
	QHButtonGroup *group = new QHButtonGroup("View model", this);

	QRadioButton *rflat = new QRadioButton("Flat", group);
	QRadioButton *rglobe = new QRadioButton("Globe", group);

	group->setButton(0);

	QObject::connect(rflat, SIGNAL(clicked()), this, SLOT(SetFlatEarthView()));
	QObject::connect(rglobe, SIGNAL(clicked()), this, SLOT(SetGlobeEarthView()));

	/* layer list */
	QListView* listview = new QListView(this);
	listview->header()->setClickEnabled(0);
	listview->addColumn("Layer");
	listview->setSorting(-1);

	for (LayerMeta *meta = LayerMeta::first; meta; meta = meta->next)
		m_LayerItems.append(new LayerListItem(listview, listview->lastItem(), meta));

	QObject::connect(listview, SIGNAL(clicked(QListViewItem *)), this, SLOT(UpdateLayers()));

	/* layout */
	QVBoxLayout *layout = new QVBoxLayout(this, 2, 2);
	layout->addWidget(group);
	layout->addWidget(listview);

	/* init GLWidget to default state */
	SetFlatEarthView();
}

void ControlWidget::UpdateLayers() {
	m_GLWidget->UpdateLayers();
	m_GLWidget->updateGL();
}

void ControlWidget::SetFlatEarthView() {
	m_GLWidget->SetFlatEarthView();
	m_GLWidget->updateGL();
}

void ControlWidget::SetGlobeEarthView() {
	m_GLWidget->SetGlobeEarthView();
	m_GLWidget->updateGL();
}

} /* namespace gaia */
