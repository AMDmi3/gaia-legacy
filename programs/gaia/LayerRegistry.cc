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

#include "LayerRegistry.h"

namespace gaia {

LayerMeta::LayerMeta(char *name, Layer *(*spawn)()) {
	m_Name = name;
	m_Next = 0;
	m_Active = 1;
	m_SpawnFunction = spawn;
}

LayerMeta::~LayerMeta() {
}

LayerMeta *LayerMeta::Next() {
	return m_Next;
}

void LayerMeta::SetNext(LayerMeta *next) {
	m_Next = next;
}

Layer *LayerMeta::Spawn() {
	return m_SpawnFunction();
}

int LayerMeta::Toggle(int on) {
	int current = m_Active;
	m_Active = on;

	return current;
}

int LayerMeta::IsActive() {
	return m_Active;
}

} /* namespace gaia */
