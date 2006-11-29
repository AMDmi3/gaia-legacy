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

#include "WorldWindFetcher.h"

WorldWindFetcher::WorldWindFetcher() {
	int res;
	if ((res = wwfetch_init(&m_WWFetch)) != WWFETCH_OK)
		throw ErrnoException("wwfetch_init() failed", res);
}

WorldWindFetcher::~WorldWindFetcher() {
	wwfetch_cleanup(&m_WWFetch);
}

void WorldWindFetcher::Process(TilePtr tile) {
	int res;
	if ((res = wwfetch_fetch_image(&m_WWFetch, tile->GetX(), tile->GetY(), tile->GetLevel())) == WWFETCH_NOT_FOUND) {
		tile->Null();
		return;
	} else if (res != WWFETCH_OK) {
		sleep(1);	/* don't do a DOS in case of any problems */
		throw ErrnoException("wwfetch_fetch_image() failed", res);
	}

	RawBuffer *buf = new RawBuffer(wwfetch_get_data_ptr(&m_WWFetch), wwfetch_get_data_size(&m_WWFetch));

	try {
		tile->Load(buf, m_pSaveStorage != 0);
	} catch (...) {
		delete buf;
		throw;
	}
}
