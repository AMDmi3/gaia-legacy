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

#ifndef __GAIA__FILESYSTEMSTORAGE_H__
#define __GAIA__FILESYSTEMSTORAGE_H__

#include <string>
#include <list>
#include <db.h>

#include "SimpleTileStorage.h"

struct PSSPrimaryKey {
	unsigned int	x;
	unsigned int	y;
	unsigned short	level;

	unsigned char	type;
#define TYPE_IMAGE 0x01
};

struct PSSSecondaryKey {
	unsigned long	lastaccess;
};

struct PSSData {
	unsigned long	lastmodify;
	unsigned long	lastaccess;
	unsigned char	data[0];
};

class FilesystemStorage: public SimpleTileStorage {
public:
	/**
	 * Constructor.
	 *
	 * @param root path to root directory of the storage
	 */
	FilesystemStorage(std::string root);

	/**
	 * Destructor.
	 */
	virtual ~FilesystemStorage();

protected:
	/**
	 * Load/save tile
	 */
	void Process(TilePtr tile);

private:
	DB	*m_Cache;
	DB	*m_SecCache;
};

#endif 
