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

#include "ControlWidget.h"
#include "GLWidget.h"

namespace gaia {

ControlWidget::ControlWidget(GLWidget* glwidget, QWidget* parent): QWidget(parent), gl_widget_(glwidget) {
	setupUi(this);

	// view model selector
	//QObject::connect(m_FlatWorldRadio, SIGNAL(clicked()), this, SLOT(SetFlatEarthView()));
	//QObject::connect(m_GlobeWorldRadio, SIGNAL(clicked()), this, SLOT(SetGlobeEarthView()));

	/* init GLWidget to default state */
	/* XXX: ?! */
	SetFlatEarthView();
}

QSize ControlWidget::sizeHint() const {
	return QSize(minimumSize().width() ? minimumSize().width() : minimumSizeHint().width(), QWidget::sizeHint().height());
}

/*void ControlWidget::ToggleLayerTreeItem(QTreeWidgetItem *item) {
	if (item->type() != LayerTreeItemType)
		return;

	// TODO(amdmi3): do we need dynamic_cast/reinterpret_cast here instead?
	// I'm not good in those :/
	LayerTreeItem *litem = (LayerTreeItem*)item;
	if (litem->checkState(0) == Qt::Checked)
		m_GLWidget->ActivateLayer(litem->GetMeta());
	if (litem->checkState(0) == Qt::Unchecked)
		m_GLWidget->DeactivateLayer(litem->GetMeta());
}

void ControlWidget::UpdateLayers() {
	// TODO(amdmi3): ?!
//	m_GLWidget->UpdateLayers();
	m_GLWidget->updateGL();
}*/

void ControlWidget::SetFlatEarthView() {
	gl_widget_->SetFlatEarthView();
	gl_widget_->updateGL();
}

void ControlWidget::SetGlobeEarthView() {
	gl_widget_->SetGlobeEarthView();
	gl_widget_->updateGL();
}

} /* namespace gaia */
