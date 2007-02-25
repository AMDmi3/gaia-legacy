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

#include "ControlWidget.h"
#include "GLWidget.h"

namespace gaia {

/********************************************************************\
 * LayerTreeItem                                                    *
\********************************************************************/
LayerTreeItem::LayerTreeItem(QTreeWidget *parent, QTreeWidgetItem *preceding, LayerMeta *meta) :
		QTreeWidgetItem(parent, preceding, LayerTreeItemType), m_Meta(meta) {
	setCheckState(0, meta->initiallyactive ? Qt::Checked : Qt::Unchecked);
	setText(0, meta->name);
}

LayerTreeItem::~LayerTreeItem() {
}

LayerMeta *LayerTreeItem::GetMeta() {
	return m_Meta;
}

/********************************************************************\
 * ControlWidget                                                    *
\********************************************************************/
ControlWidget::ControlWidget(GLWidget* target, QWidget* parent): QWidget(parent), m_GLWidget(target) {
	setupUi(this);

	/* view model selector */
	QObject::connect(m_FlatWorldRadio, SIGNAL(clicked()), this, SLOT(SetFlatEarthView()));
	QObject::connect(m_GlobeWorldRadio, SIGNAL(clicked()), this, SLOT(SetGlobeEarthView()));

	/* layer list */
	LayerTreeItem *prev = 0;
	for (LayerMeta *meta = LayerMeta::first; meta; meta = meta->next)
		prev = new LayerTreeItem(m_LayerTree, prev, meta);

	QObject::connect(m_LayerTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(ToggleLayerTreeItem(QTreeWidgetItem *)));

	/* position input */
	QObject::connect(m_XPosSpinbox, SIGNAL(valueChanged(double)), this, SLOT(MoveToPosition()));
	QObject::connect(m_YPosSpinbox, SIGNAL(valueChanged(double)), this, SLOT(MoveToPosition()));

	/* init GLWidget to default state */
	/* XXX: ?! */
	SetFlatEarthView();
}

void ControlWidget::ToggleLayerTreeItem(QTreeWidgetItem *item) {
	if (item->type() != LayerTreeItemType)
		return;

	/* XXX: do we need dynamic_cast/reinterpret_cast here instead?
	 * I'm not good in those :/ */
	LayerTreeItem *litem = (LayerTreeItem*)item;
	if (litem->checkState(0) == Qt::Checked)
		m_GLWidget->ActivateLayer(litem->GetMeta());
	if (litem->checkState(0) == Qt::Unchecked)
		m_GLWidget->DeactivateLayer(litem->GetMeta());
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
	double x = m_XPosSpinbox->value() / 360.0;
	double y = m_YPosSpinbox->value() / 360.0;

	m_GLWidget->MoveToPosition(x, y);
}

} /* namespace gaia */
