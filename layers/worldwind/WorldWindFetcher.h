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

#ifndef __GAIA__WORLDWINDFETCHER_H__
#define __GAIA__WORLDWINDFETCHER_H__

#include <wwfetch.h>

#include "SimpleTileStorage.h"

namespace gaia {

/**
 * Fetcher for NASA World Wind maps
 *
 * Handles and encapsulates all network stuff, authentification and
 * downloading of tiles from NASA server. Of course, only handles
 * loading of tiles.
 *
 * @deprecated should be restructured
 */
class WorldWindFetcher: public SimpleTileStorage {
public:
	/**
	 * Constructor
	 */
	WorldWindFetcher();

	/**
	 * Destructor
	 */
	virtual ~WorldWindFetcher();

protected:
	/**
	 * Download single tile
	 */
	void Process(TilePtr tile);

private:
	wwfetch		m_WWFetch;
};

} /* namespace gaia */

#endif 
