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

#ifndef __LIBGAIA__LAYERMANAGER_H__
#define __LIBGAIA__LAYERMANAGER_H__

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <libgaia/LayerMeta.h>
#include <libgaia/Layer.h>

using std::map;
using std::runtime_error;
using std::string;
using std::vector;

namespace gaia {

class LayerManager {
public:
	/// Constructor
	LayerManager();

	/// Destructor
	virtual ~LayerManager();

	/// Loads layer from specified dynamic library file
	static void LoadLayer(const string &filename);

	// TODO(amdmi3): add methods for GUI and Renderer interaction

private:
	typedef map<string, LayerMeta> LayerMetaMap;
	typedef vector<Layer*> LayerMap;

	Layer *SpawnLayerByName(const string &name);

	/// All loaded layers
	static LayerMetaMap layer_metas_;

	/// All layer objects for current LayerManager instance
	LayerMap layers_;
};

} // namespace gaia

#endif
