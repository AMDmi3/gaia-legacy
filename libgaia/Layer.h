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

#ifndef __LIBGAIA__LAYER_H__
#define __LIBGAIA__LAYER_H__

// groups that specify default layer ordering
// TODO: more groups (maybe will be added with new layers)
#define LAYERGROUP_OPAQUE		0x00000000	// cover whole world
#define LAYERGROUP_PARTIALLY_OPAQUE	0x00010000	// like aerial data for single city
#define LAYERGROUP_TRANSPARENT		0x00020000	// grids, gps data

#define LAYERGROUP_BACKGROUND		-1

// macros to define layers
#define DECLARE_GAIA_LAYER(classname) \
private: \
	static Layer* _spawn() { \
		return new classname; \
	} \
	static LayerMeta _meta; \
public: \
	LayerMeta *GetMeta() { \
		return &_meta; \
	}

#define IMPLEMENT_GAIA_LAYER(classname, name, group, initiallyactive) \
	LayerMeta classname::_meta = LayerMeta(name, classname::_spawn, group, initiallyactive);

namespace gaia {

class Region;

class Layer;

/// Layer metadata
///
/// For each layer class (class, not object), LayerMeta object is formed.
/// Those objetct arrange themselves as linked list and provide some info
/// about Layers, such as name (in future, position in layer tree), group,
/// initial activity flag, as well as method to create object of specific
/// Layer class.
///
/// Thus, one can iterate LayerMeta objects to find all available Layers
/// and create objects for some of them.
///
/// Macros (DECLARE_GAIA_LAYER, IMPLEMENT_GAIA_LAYER) are provided, so
/// all metadata stuff Just Works (C)
///
/// TODO: make it actual class with protected data, better encapsulation,
/// methods to access data?
class LayerMeta {
public:
	LayerMeta(char *nname, Layer *(*nspawn)(), int ngroup, int ninitiallyactive);

public:
	/// Layer name in human-friendly format
	char	*name;

	/// Layer group (see LAYERGROUP_* above)
	int	group;

	/// Whether layer is active on app start
	int	initiallyactive;

	/// Method to create object of correspondint Layer-derived class
	Layer	*(*spawn)();


	/// Next LayerMeta object in the linked list
	LayerMeta	*next;

	/// Head of LayerMeta linked list
	static LayerMeta* first;

};

/// Base class for all geo-bound data.
class Layer {
public:
	/// Constructor.
	Layer();

	/// Destructor.
	virtual ~Layer();

	/// Renders one specific region of earth surface.
	///
	/// @param rgn region to render
	virtual void RenderRegion(Region *rgn) = 0;

	/// Returns layer metadata object
	virtual LayerMeta *GetMeta() = 0;
};

} // namespace gaia

#endif
