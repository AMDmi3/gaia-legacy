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

#include "Layer.h"

namespace gaia {

/**
 * Single item of layer list
 *
 * There object of this class, derived from QCheckListItem for every
 * Layer available, so user can toggle (and later change order of) layers
 */
class LayerListItem: public QCheckListItem {
public:
	LayerListItem(QListView *parent, QListViewItem *after, GLWidget *glwidget, LayerMeta *meta);
	~LayerListItem();

	LayerMeta	*GetMeta();

protected:
	virtual void stateChange(bool s);

protected:
	LayerMeta	*m_Meta;	///< Layer metadata associated with this item
	GLWidget	*m_GLWidget;	///< GLWidget to control
};

/**
 * Widget gaia's left panel
 *
 * Contains layer list, view mode selector and other stuff
 */
class ControlWidget: public QWidget {
	Q_OBJECT
public:
	ControlWidget(GLWidget* target, QWidget* parent = 0, const char* name = 0);

private slots:
	void SetFlatEarthView();
	void SetGlobeEarthView();
	void UpdateLayers();

private:
	QPtrList<LayerListItem>	m_LayerItems;	///< Items of layer list
	GLWidget	*m_GLWidget;		///< GLWidget to control
};

} /* namespace gaia */

#endif
