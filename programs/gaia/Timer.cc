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

#include "Timer.h"

Timer *Timer::m_Instance = 0;

Timer *Timer::Instance() {
	if (!m_Instance)
		m_Instance = new Timer;

	return m_Instance;
}

Timer::Timer() {
	m_Ticks = 0;
	m_UnsafeTicks = 0;
	m_Frame = 0;
}

void Timer::Initialize(ticks_t ticks) {
	m_Ticks = ticks;
	m_UnsafeTicks = (unsafe_ticks_t)ticks;
	m_Frame = 0;
}

unsafe_ticks_t Timer::Update(unsafe_ticks_t ticks) {
	unsafe_ticks_t delta;

	if (ticks < m_UnsafeTicks) {
		delta = (unsafe_ticks_t)((ticks_t)ticks + (ticks_t)UNSAFE_TICKS_LENGTH - (ticks_t)m_UnsafeTicks);
	} else {
		delta = ticks - m_UnsafeTicks;
	}

	m_UnsafeTicks = ticks;
	m_Ticks += delta;

	m_Frame++;

	return delta;
}

ticks_t Timer::GetTime() {
	return m_Ticks;
}

ticks_t Timer::GetFrame() {
	return m_Frame;
}
