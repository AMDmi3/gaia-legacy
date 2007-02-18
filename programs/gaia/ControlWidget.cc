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
#include <qvbuttongroup.h>
#include <qlayout.h>

#include "ControlWidget.h"
#include "GLWidget.h"

namespace gaia {

/********************************************************************\
 * LayerListItem                                                    *
\********************************************************************/
LayerListItem::LayerListItem(QListView *parent, QListViewItem *after, GLWidget *glwidget, LayerMeta *meta) :
		QCheckListItem(parent, after, meta->name, QCheckListItem::CheckBox),
       		m_Meta(meta), m_GLWidget(glwidget) {
	setOn(meta->initiallyactive);
}

LayerListItem::~LayerListItem() {
}

LayerMeta *LayerListItem::GetMeta() {
	return m_Meta;
}

void LayerListItem::stateChange(bool s) {
	if (state() == QCheckListItem::On)
		m_GLWidget->ActivateLayer(m_Meta);
	else if (state() == QCheckListItem::Off)
		m_GLWidget->DeactivateLayer(m_Meta);
}

/********************************************************************\
 * ControlWidget                                                    *
\********************************************************************/
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
		m_LayerItems.append(new LayerListItem(listview, listview->lastItem(), m_GLWidget, meta));

	QObject::connect(listview, SIGNAL(clicked(QListViewItem *)), this, SLOT(UpdateLayers()));

	/* position input */
	QVButtonGroup *group2 = new QVButtonGroup("Goto Position", this);
	m_XPosSpinbox = new DoubleSpinBox(-180, 180, 10000, group2);
	m_YPosSpinbox = new DoubleSpinBox(-90, 90, 10000, group2);

	QObject::connect(m_XPosSpinbox, SIGNAL(valueChanged(int)), this, SLOT(MoveToPosition()));
	QObject::connect(m_YPosSpinbox, SIGNAL(valueChanged(int)), this, SLOT(MoveToPosition()));

	/* layout */
	QVBoxLayout *layout = new QVBoxLayout(this, 3, 2);
	layout->addWidget(group);
	layout->addWidget(group2);
	layout->addWidget(listview);

	/* init GLWidget to default state */
	/* XXX: ?! */
	SetFlatEarthView();
}

void ControlWidget::UpdateLayers() {
	/* XXX: ?! */
//	m_GLWidget->UpdateLayers();
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

void ControlWidget::MoveToPosition() {
	double x = m_XPosSpinbox->GetValue() / 360.0;
	double y = m_YPosSpinbox->GetValue() / 360.0;

	m_GLWidget->MoveToPosition(x, y);
}

} /* namespace gaia */
