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

#include <libgaia/LayerManager.h>

#include <dlfcn.h>

namespace gaia {

LayerManager::MetaLayerMap LayerManager::metalayers_;

LayerManager::LayerManager() {
}

LayerManager::~LayerManager() {
}

void LayerManager::LoadLayer(const std::string &filename) {
	void *dl;

	// TODO(amdmi3): add proper logging/exception handling here
	// TODO(amdmi3): use portable path separator
	if ((dl = dlopen(filename.c_str(), RTLD_NOW)) == NULL)
		throw std::runtime_error("Cannot dlopen layer file");

	MetaLayer::GetMetaLayerFunc getmeta = (MetaLayer::GetMetaLayerFunc)dlfunc(dl, "GetMetaLayer");

	if (getmeta == NULL) {
		dlclose(dl);
		throw std::runtime_error("Cannot load layer");
	}

	// TODO(amdmi3): add version check here

	MetaLayer meta = getmeta();

	// No duplicate layers allowed
	if (metalayers_.find(meta.GetName()) != metalayers_.end()) {
		dlclose(dl);
		throw std::runtime_error("Layer already loaded");
	}

	metalayers_.insert(std::make_pair(meta.GetName(), meta));
}

} // namespace gaia
