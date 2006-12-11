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

#include "RawBuffer.h"

namespace gaia {

RawBuffer::RawBuffer(void *data, size_t size) {
	m_Data = new unsigned char [size];
	memcpy(m_Data, data, size);
	m_Size = size;
}

RawBuffer::RawBuffer(int f) {
	struct stat st;
	if(fstat(f, &st) == -1)
		throw ErrnoException("fstat() failed", errno);

	m_Data = new unsigned char [st.st_size];
	if(read(f, m_Data, st.st_size) != st.st_size) {
		delete[] m_Data;
		throw ErrnoException("read() failed", errno);
	}
	m_Size = st.st_size;
}

RawBuffer::~RawBuffer() {
	delete[] m_Data;
}

int RawBuffer::Size() {
	return m_Size;
}

void *RawBuffer::Data() {
	return m_Data;
}

} /* namespace gaia */
