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

#include <libgaia/EarthRenderer.h>

#include <libgaia/Layer.h>

namespace gaia {

EarthRenderer::EarthRenderer(EarthRenderer *ancestor) {
	viewport_width_ = viewport_height_ = 0;

	// new class inherits all valuable data stored in ancestor
	// (layers, coords), which is stripped of everything */
	// TODO(amdmi3): do we need protected interface for this ?
	if (ancestor) {
		layers_ = ancestor->layers_;
		ancestor->layers_.clear();
	}
}

EarthRenderer::~EarthRenderer() {
	for (std::vector<Layer*>::iterator i = layers_.begin(); i < layers_.end(); i++)
		delete (*i);
}

/*int EarthRenderer::ActivateLayer(LayerMeta *meta) {
	// turn layer on: create layer object and position
	// it in layer list corresponding to it's meta
	// position in meta list
	if (layers_.empty()) {
		// empty list -> just add
		layers_.push_back(meta->spawn());
		return 1;
	}

	std::vector<Layer*>::iterator i = layers_.begin();
	for (LayerMeta *curmeta = LayerMeta::first; curmeta; curmeta = curmeta->next) {
		if ((*i)->GetMeta() == curmeta) {
			if (curmeta == meta)
				return 0;	// layer is already active
			i++;
			if (i == layers_.end() ) {
				// gone past last active layer -> add new layer to the end of the list
				layers_.push_back(meta->spawn());
				return 1;
			}
		} else if (curmeta == meta) {
			// bingo
			layers_.insert(i, meta->spawn());
			return 1;
		}
	}

	layers_.insert(layers_.begin(), meta->spawn());
	return 1;
}

int EarthRenderer::DeactivateLayer(LayerMeta *meta) {
	// turn layer off: find in in layer list by meta,
	// delete layer itself and remove pointer to it
	// from the list
	for (std::vector<Layer*>::iterator i = layers_.begin(); i < layers_.end(); i++)
		if ((*i)->GetMeta() == meta) {
			delete (*i);
			layers_.erase(i);
			return 1;
		}
	return 0;
}*/

void EarthRenderer::Resize(int width, int height) {
	viewport_width_ = width;
	viewport_height_ = height;
}

// mouse movements
int EarthRenderer::StartDrag(int x, int y, int flags) {
	return 0;
}

int EarthRenderer::Drag(int fromx, int fromy, int x, int y, int flags) {
	return 0;
}

int EarthRenderer::Click(int x, int y, int flags) {
	return 0;
}

// keyboard movements
int EarthRenderer::StartMovement(int flags) {
	return 0;
}

int EarthRenderer::StopMovement(int flags) {
	return 0;
}

int EarthRenderer::SingleMovement(int flags) {
	return 0;
}

// movement methods
void EarthRenderer::MoveToPosition(double x, double y) {
	eye_.x = x;
	eye_.y = y;
}

} // namespace gaia
