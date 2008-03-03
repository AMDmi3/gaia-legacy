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

#include "GPSSourceTest.h"

#include "Timer.h"

#include <unistd.h>

namespace gaia {

GPSSourceTest::GPSSourceTest() {
	m_LastPoll = Timer::Instance()->GetTime();
	m_Length = 0.0;
}

GPSSourceTest::~GPSSourceTest() {
}

int GPSSourceTest::Poll(GPSData *target) {
	sleep(1);

	int time = Timer::Instance()->GetTime();
	m_Length += double(time-m_LastPoll)/(1000*3600)*20.0/40000.0;	/* ~20km/h */
	m_LastPoll = time;

	target->longitude = 0.0;
	target->latitude = m_Length;

	return 1;
}

} /* namespace gaia */
