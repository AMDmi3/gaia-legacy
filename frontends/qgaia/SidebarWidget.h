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

#ifndef __QGAIA__SIDEBARWIDGET_H__
#define __QGAIA__SIDEBARWIDGET_H__

#include "GLWidget.h"

#include <QTreeWidgetItem>
#include <libgaia/Layer.h>

#include "ui_SidebarWidget.h"

namespace qgaia {

/// Sidebar widget
class SidebarWidget: public QWidget, Ui::SidebarWidget {
	Q_OBJECT
public:
	SidebarWidget(GLWidget* glwidget, QWidget* parent = 0);

	virtual QSize sizeHint() const;

private slots:
/*	void ToggleLayerTreeItem(QTreeWidgetItem *item);
	void UpdateLayers();*/
	void SetFlatEarthRenderer();
	void SetGlobeEarthRenderer();

private:
	/// Controlled GLWidget
	GLWidget *gl_widget_;
};

} // namespace qgaia

#endif
