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

#include "IndexStorage.h"
#include <tiledb.h>

IndexStorage::IndexStorage(std::string root) {
	char buf[1024];
	snprintf((char*)&buf, 1024, "%s/../gaia_storage_table", root.c_str());
	tiledb_open(&db_handle, (char*)&buf, CREATE_IF_NOT_EXISTS);
}

IndexStorage::~IndexStorage() {
	tiledb_close(&db_handle);
}

void IndexStorage::Process(TilePtr tile) {
	if (!tile->IsLoaded()) { /* loading */
		try {
			if (tiledb_get(&db_handle, tile->GetX(), tile->GetY(), tile->GetLevel()) != TILEDB_OK) {
				throw std::exception();
			}

			RawBuffer *buf = new RawBuffer(tiledb_get_data_ptr(&db_handle), tiledb_get_data_size(&db_handle));

			try {
				tile->Load(buf, m_pSaveStorage != 0);
			} catch (...) {
				delete buf;
				throw;
			}
		} catch(std::exception &e) {
			/* unable to load current tile, i.e. tile is not in a storage
			 * THIS is not an error */
		}
	} else if (tile->IsSaveable()) { /* saving */
		if (!tile->IsNull()) {
			RawBuffer *buf = tile->ReleaseRawData();
			tiledb_put(&db_handle, tile->GetX(), tile->GetY(), tile->GetLevel(), (unsigned char*)buf->Data(), buf->Size());
			return;
		}
		throw Exception("shouldn't get here");
	}
}
