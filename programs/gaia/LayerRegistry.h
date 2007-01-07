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

#ifndef __GAIA__LAYERREGISTRY_H__
#define __GAIA__LAYERREGISTRY_H__

#include "Layer.h"

/* groups that specify default layer ordering */
/* TODO: more groups (maybe will be added with new layers) */
#define LAYERGROUP_OPAQUE		0x00000000	/* cover whole world */
#define LAYERGROUP_PARTIALLY_OPAQUE	0x00010000	/* like aerial data for single city */
#define LAYERGROUP_TRANSPARENT		0x00020000	/* grids, gps data */

#define LAYERGROUP_BACKGROUND		-1

/* macros to define layers */
#define DECLARE_GAIA_LAYER(classname) \
private: \
	static Layer* _spawn() { \
		return new classname; \
	} \
	static LayerMeta _meta;

#define IMPLEMENT_GAIA_LAYER(classname, name, group, active) \
	LayerMeta classname::_meta = LayerMeta(name, classname::_spawn, group, active);

namespace gaia {

class LayerRegistry;

class LayerMeta {
public:
	LayerMeta(char *nname, Layer *(*nspawn)(), int ngroup, int nactive);

public:
	char	*name;
	int	group;
	int	active;
	Layer	*(*spawn)();

	LayerMeta	*next;
	static LayerMeta* first;
};

} /* namespace gaia */

#endif
