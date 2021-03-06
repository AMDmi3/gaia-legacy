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

#ifndef __GAIA__RAWBUFFER_H__
#define __GAIA__RAWBUFFER_H__

#include "Exceptions.h"

namespace gaia {

/**
 * Simple in-memory buffer.
 */
class RawBuffer {
public:
	/**
	 * Construct RawBuffer object from existing data in memory.
	 *
	 * Allocates memory for a buffer and copies data from given
	 * memory location.
	 *
	 * @param data pointed to beginning of data
	 * @param size size of data in bytes
	 */
	RawBuffer(void *data, size_t size);

	/**
	 * Construct RawBuffer reading data from file.
	 *
	 * Allocates memory for a buffer and fills it with data
	 * read from file. Whole file is read.
	 *
	 * @param f source file descriptor
	 */
	RawBuffer(int f);

	/**
	 * Destructor.
	 *
	 * Frees memory used by buffer.
	 */
	virtual ~RawBuffer();

	/**
	 * Returns data size.
	 */
	int Size();

	/**
	 * Returns pointer to data.
	 */
	void *Data();

protected:
	unsigned char	*m_Data;	///< Pointer to data
	size_t		m_Size;		///< Size of data
};

} /* namespace gaia */

#endif
