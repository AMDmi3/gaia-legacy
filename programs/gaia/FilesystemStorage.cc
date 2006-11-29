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

#include "FilesystemStorage.h"

FilesystemStorage::FilesystemStorage(std::string root) {
#if 0
	int ret;

	m_Cache = 0;
	m_SecCache = 0;

	try {
		if ((ret = db_create(&m_Cache, 0, 0)) != 0)
			throw Exception("db_create() failed for primary BDB");

		if ((ret = db_create(&m_SecCache, 0, 0)) != 0)
			throw Exception("db_create() failed for secondary BDB");

		if ((ret = m_Cache->open(m_Cache, 0, (root + "cache.db").c_str(), 0, DB_BTREE, DB_CREATE, 0)) != 0)
			throw Exception("open() failed for primary BDB");

		if ((ret = m_SecCache->set_flags(m_SecCache, DB_DUPSORT)) != 0)
			throw Exception("set_flags() failed for secondary BDB");

		if ((ret = m_SecCache->open(m_SecCache, 0, (root + "cache.sec.db").c_str(), 0, DB_BTREE, DB_CREATE, 0)) != 0)
			throw Exception("open() failed for secondary BDB");

		if ((ret = m_Cache->associate(m_Cache, 0, m_SecCache, sec_key_creation, 0)) != 0)
			throw Exception("associate() failed");
	} catch (...) {
		if (m_Cache)
			m_Cache->close(m_Cache, 0);
		if (m_SecCache)
			m_SecCache->close(m_SecCache, 0);
		throw;
	}
#endif
}

FilesystemStorage::~FilesystemStorage() {
#if 0
	m_Cache->close(m_Cache, 0);
	m_SecCache->close(m_SecCache, 0);
#endif
}

void FilesystemStorage::Process(TilePtr tile) {
#if 0
	if (!tile->IsLoaded()) { /* loading */
		try {
			PSSPrimaryKey k;

			DBT key, data;

			key.data = &k;
			key.size = sizeof(k);

			if ((f = open(path.c_str(), O_RDONLY)) == -1)
				throw ErrnoException("cannot open file in filesystem storage", errno);

			RawBuffer *buf = new RawBuffer(f);

			try {
				tile->Load(buf, m_pSaveStorage != 0);
			} catch (...) {
				close(f);
				delete buf;
				throw;
			}

			close(f);
		} catch(ErrnoException &) {
			/* unable to load current tile, i.e. tile is not in a storage
			 * THIS is not an error */
		} catch(std::exception &) {
			/* unable to load current tile, i.e. tile is not in a storage
			 * THIS is not an error */
		}
	} else if (tile->IsSaveable()) { /* saving */
		PSSPrimaryKey k;
		PSSData d;

		/* compose PSSPrimaryKey */
		/* compose PSSData */
		/* store */

		std::string subpath = PathFromCoords(tile->GetX(), tile->GetY(), tile->GetLevel(), tile->GetType());

		/* store file, creating needed directories */
		std::string::size_type pos = 0;
		while (pos != std::string::npos) {
			if ((pos = subpath.find('/', pos+1)) == std::string::npos) {
				std::string path = m_StorageRoot + subpath;
				int f;
				if ((f = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
					throw ErrnoException("cannot open file in filesystem storage", errno);
				}

				/* for null tile, just create corresponding file */
				if (!tile->IsNull()) {
					RawBuffer *buf = tile->ReleaseRawData();
					if (write(f, buf->Data(), buf->Size()) != buf->Size()) {
						unlink(path.c_str());	/* ensure no broken files get into cache */
						delete buf;
						close(f);
						throw ErrnoException("cannot write file into filesystem storage", errno);
					}

					delete buf;
				}

				close(f);
				return;
			} else {
				std::string path = m_StorageRoot + subpath.substr(0, pos);
				if (mkdir(path.c_str(), 0777) != 0 && errno != EEXIST)
					throw ErrnoException("cannot mkdir in filesystem storage", errno);
			}
		}
		throw Exception("shouldn't get here");
	}
#endif
}
