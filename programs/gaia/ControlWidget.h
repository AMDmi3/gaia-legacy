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

#include <qwidget.h>
#include <qlistview.h>

#include "GLWidget.h"

namespace gaia {

class LayerListItem: public QCheckListItem {
public:
	LayerListItem(QListView *parent, QListViewItem *after, const QString &name, Layer *layer, int on) : QCheckListItem(parent, after, name, QCheckListItem::CheckBox), m_Layer(layer) { setOn(on); }
	~LayerListItem() { delete m_Layer; }

	Layer	*GetLayer() { return m_Layer; }

protected:
	Layer	*m_Layer;
};

class ControlWidget: public QWidget {
	Q_OBJECT
public:
	ControlWidget(GLWidget* target, QWidget* parent = 0, const char* name = 0);

private slots:
	void SetFlatEarthView();
	void SetGlobeEarthView();
	void UpdateLayers();

private:
	QPtrList<LayerListItem>	m_LayerItems;
	GLWidget	*m_GLWidget;
};

} /* namespace gaia */

#endif
