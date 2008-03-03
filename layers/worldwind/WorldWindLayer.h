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

#ifndef __GAIA__WORLDWINDLAYER_H__
#define __GAIA__WORLDWINDLAYER_H__

#include "Layer.h"

#include "WorldWindTileManager.h"
#include "WorldWindFetcher.h"
#include "FilesystemStorage.h"

#define MIN_TEXTURE_DISTANCE 192.0

namespace gaia {

/**
 * NASA World Wind map layer
 */
class WorldWindLayer: public Layer {
	DECLARE_GAIA_LAYER(WorldWindLayer);
public:
	WorldWindLayer();
	virtual ~WorldWindLayer();

	void RenderRegion(Region *rgn);

	int GetSplitLevel(double wlen, double plen);

protected:
	WorldWindTileManager	*m_TileManager;
	WorldWindFetcher        *m_ImageFetcher;
	FilesystemStorage       *m_DiskCache;
};

} /* namespace gaia */

#endif
