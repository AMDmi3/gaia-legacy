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

#include <libgaia/MetaLayer.h>
#include <libgaia/Layer.h>

namespace gaia {

/// Base class for layer managers.
///
/// Handled layer loading, layer object spawning, render order etc.
/// This should be subclassed by the frontend to implement specific
/// behavior
class LayerManager {
public:
	/// Constructor
	LayerManager();

	/// Destructor
	virtual ~LayerManager();

	/// Loads layer from specified dynamic library file
	static void LoadLayer(const std::string &filename);

	/// Renders one specific region of earth surface
	virtual void RenderRegion(Region *rgn) = 0;

protected:
	typedef std::map<std::string, MetaLayer> MetaLayerMap;

	/// All loaded layers
	static MetaLayerMap metalayers_;
};

} // namespace gaia

#endif
