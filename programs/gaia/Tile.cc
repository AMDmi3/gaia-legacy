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

#include "Tile.h"

Tile::Tile(int x, int y, int level) {
	if (pthread_mutex_init(&m_RefCountMutex, 0) != 0)
		throw ErrnoException("pthread_mutex_init() failed", errno);

	m_RefCount = 0;

	m_X = x;
	m_Y = y;
	m_Level = level;

	m_LastUsed = Timer::Instance()->GetFrame();

	m_RawData = 0;
	m_IsNull = 0;
}

Tile::~Tile() {
	delete m_RawData;
	pthread_mutex_destroy(&m_RefCountMutex);
}

/* coord ops */
int Tile::GetX() {
	return m_X;
}

int Tile::GetY() {
	return m_Y;
}

int Tile::GetLevel() {
	return m_Level;
}

int Tile::GetType() {
	return TILETYPE_NONE;
}

/* age ops */
void Tile::Touch() {
	m_LastUsed = Timer::Instance()->GetFrame();
}

ticks_t Tile::GetAge() {
	return Timer::Instance()->GetFrame() - m_LastUsed;
}

int Tile::IsOld() {
	return GetAge() > 2;
}

/* null ops */
void Tile::Null() {
	m_IsNull = 1;
}

int Tile::IsNull() {
	return m_IsNull;
}

/* load/save */
void Tile::Load(RawBuffer *data, int keep) {
	if (keep)
		m_RawData = data;
	else 
		delete data;
}

RawBuffer *Tile::ReleaseRawData() {
	RawBuffer *raw = m_RawData;
	m_RawData = 0;
	return raw;
}

int Tile::IsSaveable() {
	return m_RawData != 0;
}

int Tile::IsLoaded() {
	return IsNull();
}

int Tile::IsReady() {
	return !IsNull() && IsLoaded();
}

/* reference counting */
unsigned int Tile::IncRef() {
	pthread_mutex_lock(&m_RefCountMutex);
	unsigned int rc = ++m_RefCount;
	pthread_mutex_unlock(&m_RefCountMutex);
	return rc;
}

unsigned int Tile::DecRef() {
	pthread_mutex_lock(&m_RefCountMutex);
	unsigned int rc = --m_RefCount;
	pthread_mutex_unlock(&m_RefCountMutex);
	return rc;
}
