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

#ifndef __GAIA__MASTERLAYER_H__
#define __GAIA__MASTERLAYER_H__

#include <GL/gl.h>
#include <vector>

#include "Region.h"
#include "Layer.h"
#include "SlaveLayer.h"
#include "Math.h"

/**
 * Base class for core geographical data handlers
 *
 * Master layers hangle base earth data, such as imaginery and
 * topography. There is only one master layer, but multiple slave
 * layers may be bound to it to display additional information,
 * such as grid or GPS position.
 */
class MasterLayer: public Layer {
public:
	/**
	 * Constructor
	 */
	MasterLayer();

	/**
	 * Destructor
	 */
	virtual ~MasterLayer();

	/**
	 * Render one specific region of earth surface
	 *
	 * @param rgn region to render
	 */
	virtual void RenderRegion(Region *rgn) = 0;

	/**
	 * Bind slave layer to this master layer
	 *
	 * @param layer pointer to SlaveLayer to bind.
	 */
	void BindSlaveLayer(SlaveLayer *layer);

	/**
	 * Unbind all slave layers
	 */
	void ClearSlaveLayers();

protected:
	/**
	 * Pointers to all slave layers currently bound
	 */
	std::vector<SlaveLayer*>	m_SlaveLayers;
};

#endif
