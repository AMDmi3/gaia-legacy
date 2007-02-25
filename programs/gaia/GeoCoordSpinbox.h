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

#ifndef __GAIA__GEOCOORdSpinbox_H__
#define __GAIA__GEOCOORdSpinbox_H__

#include <QDoubleSpinBox>

#include <math.h>

namespace gaia {

/**
 * Spinbox that understands geographical coordinates
 */
class GeoCoordSpinbox: public QDoubleSpinBox {
	Q_OBJECT
public:
	enum CoordFormat { CoordFormatDegrees = 0, CoordFormatMinutes = 1, CoordFormatSeconds = 2 };
	GeoCoordSpinbox(double limit, QChar upletter, QChar downletter, QWidget *parent = 0);

	virtual QString textFromValue(double value) const;
	virtual double valueFromText(const QString &text) const;

protected:
//	void contextMenuEvent(QContextMenuEvent *event);

protected:
	int	m_CoordFormat;
	QChar	m_UpLetter;
	QChar	m_DownLetter;
};

/**
 * Shortcut for longitude GeoCoordSpinbox
 */
class LongitudeSpinbox: public GeoCoordSpinbox {
	Q_OBJECT
public:
	LongitudeSpinbox(QWidget *parent = 0): GeoCoordSpinbox(180.0, 'E', 'W', parent) {}
};

/**
 * Shortcut for latitude GeoCoordSpinbox
 */
class LatitudeSpinbox: public GeoCoordSpinbox {
	Q_OBJECT
public:
	LatitudeSpinbox(QWidget *parent = 0): GeoCoordSpinbox(90.0, 'N', 'S', parent) {}
};

} /* namespace gaia */

#endif
