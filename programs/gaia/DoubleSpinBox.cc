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

#include "DoubleSpinBox.h"
#include <qvalidator.h>

DoubleSpinBox::DoubleSpinBox(QWidget* parent, const char* name): QSpinBox(parent, name) {
	setValue(0);
	setValidator(new QDoubleValidator(parent, name));
}

DoubleSpinBox::DoubleSpinBox(int minValue, int maxValue, int step, QWidget *parent, const char *name)
	: QSpinBox(minValue*100000, maxValue*100000, step, parent, name) {
	setValue(0);
	setValidator(new QDoubleValidator(minValue*100000, maxValue*100000, 5, parent, name));
}

QString DoubleSpinBox::mapValueToText( int value ) {
	return QString( "%1" ).arg(value/100000.0, 0, 'F', 5);
}

int DoubleSpinBox::mapTextToValue( bool *ok ) {
	return (int)(100000*text().toDouble());
}

double DoubleSpinBox::GetValue() {
	return text().toDouble();
}
