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

#ifndef __GAIA__GPSSOURCE_H__
#define __GAIA__GPSSOURCE_H__

namespace gaia {

/**
 * Structure to store single GPS state
 */
struct GPSData {
	double	longitude;
	double	latitude;
};

/**
 * Base class for all GPS data sources
 *
 * Defines abstract method for retrieving GPS data.
 */
class GPSSource {
public:
	/**
	 * Constructor
	 */
	virtual ~GPSSource() {}

	/**
	 * Retrieve current GPS data
	 *
	 * @param where to place new data 
	 * @returns 1 if data was retrieved successfully, 0 otherwise
	 */
	virtual int Poll(GPSData *target) = 0;
};

} /* namespace gaia */

#endif
