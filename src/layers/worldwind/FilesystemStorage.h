/*
 * gaia - opensource 3D interface to the planet
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

#ifndef __FILESYSTEMSTORAGE_H__
#define __FILESYSTEMSTORAGE_H__

#include <string>
#include <list>
#include <sys/stat.h>
#include <fcntl.h>

#include "SimpleTileStorage.h"

namespace gaia {

/**
 * Filesystem cache to store tiles locally.
 *
 * @deprecated This implementation wastes tons of inodes, lots of space, is
 * very slow and ineffective. Will be replaced, see TODO file.
 *
 * @deprecated should be restructured and merged to GoogleLayer
 */
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
	/**
	 * Convert tile type and coords into path
	 *
	 * @returns tile path relative to storage root
	 */
	std::string PathFromCoords(int x, int y, int level, int type);

private:
	std::string	m_StorageRoot;	///< Root directory of the storage
};

} /* namespace gaia */

#endif 
