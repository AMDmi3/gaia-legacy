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

#ifndef __LIBGAIA__LAYERMETA_H__
#define __LIBGAIA__LAYERMETA_H__

#include <string>

#define REGISTER_GAIA_LAYER(name,group,active) \
static Layer *Spawn() { \
	return new name; \
} \
extern "C" LayerMeta GetLayerMeta() { \
	return LayerMeta(#name, group, active, Spawn); \
} \

namespace gaia {

using std::string;

class Layer;

/// Layer metadata
class LayerMeta {
public:
	/// Layer group - specifies layer ordering (from bottom to top)
	enum LayerGroup {
		LAYERGROUP_OPAQUE,
		LAYERGROUP_PARTIALLY_OPAQUE,
		LAYERGROUP_TRANSPARENT,
	};

	/// Layer spawning function
	typedef Layer *(*SpawnLayer)(); 

	/// Constructor
	LayerMeta(const string &name, LayerGroup group, bool initially_active, SpawnLayer spawn):
		name_(name), group_(group), initially_active_(initially_active), spawn_(spawn) {
	}

	/// Spawn layer object
	Layer *Spawn() {
		return spawn_();
	}

private:
	/// Layer name
	string name_;

	/// Layer group
	LayerGroup group_;

	// TODO(amdmi3): this is to be removed in future, superseded
	// by Config class and default configs
	/// Whether layer is active on gaia start
	bool initially_active_;

	/// Method to create object of corresponding Layer class
	SpawnLayer spawn_;
};

} // namespace gaia

#endif
