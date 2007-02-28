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

#ifndef __GAIA__CONTROLWIDGET_H__
#define __GAIA__CONTROLWIDGET_H__

#include <QTreeWidgetItem>

#include "ui_ControlWidget.h"

#include "GLWidget.h"

#include "Layer.h"

/* type to distinguish LayerTreeItem's from other QTreeWidgetItem's */
#define LayerTreeItemType	QTreeWidgetItem::UserType+1

namespace gaia {

/**
 * Single item of layer list
 *
 * There object of this class, derived from QCheckListItem for every
 * Layer available, so user can toggle (and later change order of) layers
 */
class LayerTreeItem: public QTreeWidgetItem {
public:
	LayerTreeItem(QTreeWidget *parent, QTreeWidgetItem *preceding, LayerMeta *meta);
	~LayerTreeItem();

	LayerMeta	*GetMeta();

protected:
	LayerMeta	*m_Meta;	///< Layer metadata associated with this item
};

/**
 * Widget gaia's left panel
 *
 * Contains layer list, view mode selector and other stuff
 */
class ControlWidget: public QWidget, Ui::ControlWidget {
	Q_OBJECT
public:
	ControlWidget(GLWidget* target, QWidget* parent = 0);

	virtual QSize sizeHint() const;

private slots:
	void ToggleLayerTreeItem(QTreeWidgetItem *item);
	void SetFlatEarthView();
	void SetGlobeEarthView();
	void MoveToPosition();
	void UpdateLayers();

private:
	GLWidget	*m_GLWidget;		///< GLWidget to control
};

} /* namespace gaia */

#endif
