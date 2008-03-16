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

EarthRenderer::EarthRenderer() :
	layer_manager_(NULL), viewport_width_(0), viewport_height_(0) {
}

EarthRenderer::~EarthRenderer() {
}

void EarthRenderer::SetLayerManager(LayerManager *manager) {
	layer_manager_ = manager;
}

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
