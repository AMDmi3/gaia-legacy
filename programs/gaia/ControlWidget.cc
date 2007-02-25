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

//#include <q3header.h>
//#include <q3listview.h>
//#include <qradiobutton.h>
//#include <qpushbutton.h>
//#include <qhbuttongroup.h>
//#include <qvbuttongroup.h>
//#include <qlayout.h>
//Added by qt3to4:
//#include <Q3VBoxLayout>

#include "ControlWidget.h"
#include "GLWidget.h"

namespace gaia {

/********************************************************************\
 * LayerListItem                                                    *
\********************************************************************/
/*LayerListItem::LayerListItem(Q3ListView *parent, Q3ListViewItem *after, GLWidget *glwidget, LayerMeta *meta) :
		Q3CheckListItem(parent, after, meta->name, Q3CheckListItem::CheckBox),
       		m_Meta(meta), m_GLWidget(glwidget) {
	setOn(meta->initiallyactive);
}

LayerListItem::~LayerListItem() {
}

LayerMeta *LayerListItem::GetMeta() {
	return m_Meta;
}

void LayerListItem::stateChange(bool s) {
	if (state() == Q3CheckListItem::On)
		m_GLWidget->ActivateLayer(m_Meta);
	else if (state() == Q3CheckListItem::Off)
		m_GLWidget->DeactivateLayer(m_Meta);
}*/

/********************************************************************\
 * ControlWidget                                                    *
\********************************************************************/
ControlWidget::ControlWidget(QWidget* parent, GLWidget* target): QWidget(parent) {
	/* target GLWidget to control */
	m_GLWidget = target;

	/* view model selector */
//	Q3HButtonGroup *group = new Q3HButtonGroup("View model", this);

//	QRadioButton *rflat = new QRadioButton("Flat", group);
//	QRadioButton *rglobe = new QRadioButton("Globe", group);

//	group->setButton(0);

//	QObject::connect(rflat, SIGNAL(clicked()), this, SLOT(SetFlatEarthView()));
//	QObject::connect(rglobe, SIGNAL(clicked()), this, SLOT(SetGlobeEarthView()));

	/* layer list */
//	Q3ListView* listview = new Q3ListView(this);
//	listview->header()->setClickEnabled(0);
//	listview->addColumn("Layer");
//	listview->setSorting(-1);

//	for (LayerMeta *meta = LayerMeta::first; meta; meta = meta->next)
//		m_LayerItems.append(new LayerListItem(listview, listview->lastItem(), m_GLWidget, meta));

//	QObject::connect(listview, SIGNAL(clicked(Q3ListViewItem *)), this, SLOT(UpdateLayers()));

	/* position input */
//	Q3VButtonGroup *group2 = new Q3VButtonGroup("Goto Position", this);
//	m_XPosSpinbox = new DoubleSpinBox(-180, 180, 10000, group2);
//	m_YPosSpinbox = new DoubleSpinBox(-90, 90, 10000, group2);

//	QObject::connect(m_XPosSpinbox, SIGNAL(valueChanged(int)), this, SLOT(MoveToPosition()));
//	QObject::connect(m_YPosSpinbox, SIGNAL(valueChanged(int)), this, SLOT(MoveToPosition()));

	/* layout */
//	Q3VBoxLayout *layout = new Q3VBoxLayout(this, 3, 2);
//	layout->addWidget(group);
//	layout->addWidget(group2);
//	layout->addWidget(listview);

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
//	double x = m_XPosSpinbox->GetValue() / 360.0;
//	double y = m_YPosSpinbox->GetValue() / 360.0;
	double x = 0;
	double y = 0;

	m_GLWidget->MoveToPosition(x, y);
}

} /* namespace gaia */
