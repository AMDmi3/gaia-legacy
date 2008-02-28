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

#ifdef WITH_GPSD

#include "GPSSourceGPSD.h"

#include "Exceptions.h"

#include <string.h>
#include <fcntl.h>

namespace gaia {

GPSSourceGPSD::GPSSourceGPSD(char *server, char *port) {
	if ((m_GPS = gps_open(server, port)) == 0)
		throw Exception("gps_open() failed");

	if (gps_query(m_GPS, "w+x\n") != 0)
		throw Exception("initial gps_query() failed");
}

GPSSourceGPSD::~GPSSourceGPSD() {
	gps_close(m_GPS);
}

int GPSSourceGPSD::Poll(GPSData *target) {
	if (gps_poll(m_GPS) != 0)
		return 0;

	/* XXX: warning: comparing floating point with == or != is unsafe */
	/* use (fpclassify(m_GPS->online) == FP_ZERO)? is it portable? */
	if (!m_GPS->online)
		return 0;

	if (m_GPS->status == STATUS_NO_FIX)
		return 0;

	if (m_GPS->fix.mode < MODE_2D)
		return 0;

	target->longitude = m_GPS->fix.longitude/360.0;
	target->latitude = m_GPS->fix.latitude/360.0;

	return 1;
}

} /* namespace gaia */

#endif
