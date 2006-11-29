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

#include "Hud.h"

Hud *Hud::m_Instance = 0;

Hud *Hud::Instance() {
	if (!m_Instance)
		m_Instance = new Hud;

	return m_Instance;
}

Hud::Hud() {
}

Hud::~Hud() {
	for (std::vector<HudObject*>::iterator i = m_Objects.begin(); i < m_Objects.end(); i++)
		delete (*i);
}

void Hud::AddObject(HudObject *o) {
	m_Objects.push_back(o);
}

void Hud::Render(int width, int height) {
	if (m_Objects.size() == 0)
		return;

	glMatrixMode(GL_PROJECTION);
	double m[16] = {
		2.0/width,	0,		0,	0,
		0,		2.0/height,	0,	0,
		0,		0,		1.0,	0,
		-1.0,		-1.0,		0,	1.0,
	};
	glLoadMatrixd(m);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0, 0.0, 0.0);

	qsort(&m_Objects[0], m_Objects.size(), sizeof(HudObject*), HudObject_Compare);

	for (std::vector<HudObject*>::iterator i = m_Objects.begin(); i < m_Objects.end(); i++) {
		if ((*i)->CheckBounds(width, height))
			(*i)->Render();
		delete (*i);
	}

	m_Objects.clear();
}
