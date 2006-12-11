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

#ifndef __GAIA__GPSLAYER_H__
#define __GAIA__GPSLAYER_H__

#include <vector>
#include <string.h>
#include <pthread.h>

#include "Timer.h"
#include "Layer.h"
#include "GPSSourceNMEA.h"
#include "GPSSourceTest.h"
#ifdef WITH_GPSD
#	include "GPSSourceGPSD.h"
#endif
#include "Hud.h"
#include "HudIcon.h"
#include "Data.h"

#define DATA_ROT_TIME 5000
#define MAX_ENTITY_NAME 64

/* minimal distance between two points */ 
#define DATA_CONST_DIST 0.01/40000.0

namespace gaia {

/**
 * Single GPS entity
 * 
 * Represents single GPS dataset, with it's own source, distinguished
 * from others by name and color.
 */
struct GPSEntity {
	char		name[MAX_ENTITY_NAME]; 	///< User-provided name for an entity
	int		lastupdate;	///< Timestamp of last update
	std::vector<GPSData>	data;	///< Data points
	unsigned char	color[3];	///< Color to distinguish from other entities
	GPSSource	*source;	///< Data source
	pthread_t	thread;		///< Entity's own thread
	pthread_mutex_t datamutex;	///< Entity's mutex
};

/**
 * Layer for displaying GPS data
 *
 * @todo render trails
 * @todo route optimizer (i.e. for strait line consisting of many
 * points, leave only first and last)
 */
class GPSLayer: public Layer {
public:
	/**
	 * Constructor
	 */
	GPSLayer();

	/**
	 * Destructor
	 */
	virtual ~GPSLayer();

	void RenderRegion(Region *rgn);

	/**
	 * Adds one GPS entity
	 *
	 * @param name arbitary name for an entity
	 * @param source URL-like source description (like gpsd://host:port or nmea://path)
	 */
	void AddEntity(char *name, char *source);

private:
	/**
	 * Entry point for per-entity retrieval threads
	 */
	static void* StartEntityThread(void* pentity);

protected:
	/**
	 * Array of entities
	 */
	std::vector<GPSEntity*>	m_Entities;
};

} /* namespace gaia */

#endif
