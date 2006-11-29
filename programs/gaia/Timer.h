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

#ifndef __GAIA__TIMER_H__
#define __GAIA__TIMER_H__

#include <stdint.h>

typedef uint64_t		ticks_t;
typedef uint32_t		unsafe_ticks_t;

#define UNSAFE_TICKS_LENGTH	0x100000000ULL

/**
 * Singleton timer class.
 *
 * Can output 64bit time in milliseconds.
 * Uses terms:
 * - Unsafe ticks - milliseconds stored as 32 bit unsigned integer
 *   Those tend to overflow every ~47 days, thus considered unsafe
 * - Safe ticks - milliseconds stored as 64 bit unsigned integer
 */
class Timer {
public:
	/**
	 * Returns instance of Timer
	 */
	static Timer *Instance();

	/**
	 * Initializes timer.
	 *
	 * @param time is current unsafe ticks value.
	 */
	void Initialize(ticks_t time);

	/**
	 * Updates current time given unsae ticks value.
	 * Correctly handles overflows.
	 *
	 * @returns time delta to last call
	 */
	unsafe_ticks_t Update(unsafe_ticks_t time);

	/**
	 * Returns current safe ticks value.
	 */
	ticks_t GetTime();

	/**
	 * Returns current frame number
	 */
	ticks_t GetFrame();

protected:
	/**
	 * Protected constructor
	 */
	Timer();

private:
	static Timer	*m_Instance;

	ticks_t		m_Ticks;
	unsafe_ticks_t	m_UnsafeTicks;
	ticks_t		m_Frame;
};

#endif
