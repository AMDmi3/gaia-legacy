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

#include "GeoCoordSpinbox.h"

#include <QMenu>
#include <QContextMenuEvent>

namespace gaia {

GeoCoordSpinbox::GeoCoordSpinbox(double limit, QChar upletter, QChar downletter, QWidget *parent):
	QDoubleSpinBox(parent), m_UpLetter(upletter), m_DownLetter(downletter) {
	m_CoordFormat = CoordFormatDegrees;
	setRange(-limit, limit);
	setSingleStep(1.0);	/* XXX: will we be able to make it depend on parent->glwidget->earthview->eye->span? */
}

QString GeoCoordSpinbox::textFromValue(double value) const {
	QString degrees;
	degrees.setNum(fabs(value), 'f', 6);

	return QString(value >= 0 ? m_UpLetter : m_DownLetter) + degrees + QChar(0xB0);
}

double GeoCoordSpinbox::valueFromText(const QString &text) const {
	double tmp = text.toDouble(0);

	return tmp;
}

/*void GeoCoordSpinbox::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu(this);
	menu.addAction(QString("hDDD.dd") + QChar(0xB0));
	menu.addAction(QString("hDDD") + QChar(0xB0) + QString("MM.mm\'"));
	menu.addAction(QString("hDDD") + QChar(0xB0) + QString("MM\'SS.ss\""));
	menu.exec(event->globalPos());
}*/

} /* namespace gaia */
