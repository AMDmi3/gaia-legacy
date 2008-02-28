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

#ifndef __GAIA__GPSSOURCETEST_H__
#define __GAIA__GPSSOURCETEST_H__

#include "GPSSource.h"

namespace gaia {

/**
 * Test GPS source.
 *
 * Provides fake GPS data for testing purposes (moves from (0,0)
 * in north direction at ~20 km/h)
 */
class GPSSourceTest: public GPSSource {
public:
	/**
	 * Constructor
	 */
	GPSSourceTest();

	/**
	 * Destructor
	 */
	virtual ~GPSSourceTest();

	virtual int Poll(GPSData *target);

private:
	int	m_LastPoll;
	double	m_Length;
};

} /* namespace gaia */

#endif
