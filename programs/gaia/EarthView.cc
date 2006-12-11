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

#include "EarthView.h"

namespace gaia {

EarthView::EarthView() {
	m_ViewportWidth = m_ViewportHeight = 0;
}

EarthView::~EarthView() {
}

void EarthView::BindLayer(Layer *layer) {
	m_Layers.push_back(layer);
}

void EarthView::Resize(int width, int height) {
	m_ViewportWidth = width;
	m_ViewportHeight = height;
}

/* mouse movements */
int EarthView::StartDrag(int x, int y, int flags) {
	return 0;
}

int EarthView::Drag(int fromx, int fromy, int x, int y, int flags) {
	return 0;
}

int EarthView::Click(int x, int y, int flags) {
	return 0;
}

/* keyboard movements */
int EarthView::StartMovement(int flags) {
	return 0;
}

int EarthView::StopMovement(int flags) {
	return 0;
}

int EarthView::SingleMovement(int flags) {
	return 0;
}

} /* namespace gaia */
