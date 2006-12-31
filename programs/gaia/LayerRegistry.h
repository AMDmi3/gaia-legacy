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

/* layer groups; by default layers are sorted in group order */
/* layer is opaque and covers whole world (like NASA WW) */
#define LAYERGROUP_OPAQUE		0x00

/* layer has data only for specific regions (for example, map of some city) */
#define LAYERGROUP_PARTIALLY_OPAQUE	0x01

/* layer is fully transparent (GPS, grid etc.) */
#define LAYERGROUP_TRANSPARENT		0x02

/* macros to define layers */
#define DECLARE_GAIA_LAYER(classname) \
private: \
	static Layer* _spawn() { \
		return new classname; \
	} \
	static LayerMeta _meta;

#define INIT_GAIA_LAYER(classname, name, group) \
	LayerMeta classname::_meta = LayerMeta(name, classname::_spawn);

namespace gaia {

class LayerRegistry;

class LayerMeta {
public:
	LayerMeta(char *name) {}
	LayerMeta(Layer *(*spawn)()) {}
	LayerMeta(char *name, Layer *(*spawn)());
	virtual ~LayerMeta();

	LayerMeta *Next();
	Layer*	Spawn();

	int Toggle(int on);
	int IsActive();

private:
	void SetNext(LayerMeta *next);

private:
	char	*m_Name;
	int	m_Active;
	Layer	*(*m_SpawnFunction)();
	LayerMeta	*m_Next;

	friend class LayerRegistry;
};

/*class LayerRegistry {
public:
	LayerRegistry();
	virtual ~LayerRegistry();

	virtual void RegisterLayer(LayerData *data, int group);

private:
	LayerData	*m_FirstLayer;
};*/

} /* namespace gaia */

#endif
