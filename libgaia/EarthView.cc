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

#include "EarthView.h"

#include "Layer.h"

namespace gaia {

EarthView::EarthView(EarthView *ancestor) {
	m_ViewportWidth = m_ViewportHeight = 0;

	/* new class inherits all valuable data stored in ancestor
	 * (layers, coords), which is stripped of everything */
	/* XXX: do we need protected interface for this ? */
	if (ancestor) {
		m_Layers = ancestor->m_Layers;
		ancestor->m_Layers.clear();
	}
}

EarthView::~EarthView() {
	for (std::vector<Layer*>::iterator i = m_Layers.begin(); i < m_Layers.end(); i++)
		delete (*i);
}

int EarthView::ActivateLayer(LayerMeta *meta) {
	/* turn layer on: create layer object and position
	 * it in layer list corresponding to it's meta
	 * position in meta list
	 */
	if (m_Layers.empty()) {
		/* empty list -> just add */
		m_Layers.push_back(meta->spawn());
		return 1;
	}

	std::vector<Layer*>::iterator i = m_Layers.begin();
	for (LayerMeta *curmeta = LayerMeta::first; curmeta; curmeta = curmeta->next) {
		if ((*i)->GetMeta() == curmeta) {
			if (curmeta == meta)
				return 0;	/* layer is already active */
			i++;
			if (i == m_Layers.end() ) {
				/* gone past last active layer -> add new layer to the end of the list*/
				m_Layers.push_back(meta->spawn());
				return 1;
			}
		} else if (curmeta == meta) {
			/* bingo */
			m_Layers.insert(i, meta->spawn());
			return 1;
		}
	}

	m_Layers.insert(m_Layers.begin(), meta->spawn());
	return 1;
}

int EarthView::DeactivateLayer(LayerMeta *meta) {
	/* turn layer off: find in in layer list by meta,
	 * delete layer itself and remove pointer to it
	 * from the list */
	for (std::vector<Layer*>::iterator i = m_Layers.begin(); i < m_Layers.end(); i++)
		if ((*i)->GetMeta() == meta) {
			delete (*i);
			m_Layers.erase(i);
			return 1;
		}
	return 0;
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

/* movement methods */
void EarthView::MoveToPosition(double x, double y) {
	m_Eye.x = x;
	m_Eye.y = y;
}

} /* namespace gaia */
