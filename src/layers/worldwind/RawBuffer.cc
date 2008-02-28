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

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

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
