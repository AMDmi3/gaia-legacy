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
 *
 * Author: Andreas Grau
 */

#ifndef __INDEXSTORAGE_H__
#define __INDEXSTORAGE_H__

#include <tiledb.h>
#include <string>
#include "SimpleTileStorage.h"

#define MAX_IMAGE_SIZE 100*1024

/**
 * Fast index based storage for tiles
 */
class IndexStorage: public SimpleTileStorage {
public:
	/**
	 * Constructor.
	 *
	 * @param root path to root directory of the storage
	 */
	IndexStorage(std::string root);

	/**
	 * Destructor.
	 */
	virtual ~IndexStorage();

protected:
	/**
	 * Load/save tile
	 */
	void Process(TilePtr tile);
private:
	DB_Handle *db_handle;
};

#endif
