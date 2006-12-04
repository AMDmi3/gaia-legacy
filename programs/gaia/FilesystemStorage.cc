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

#include "FilesystemStorage.h"

FilesystemStorage::FilesystemStorage(std::string root) {
	m_StorageRoot = root;
}

FilesystemStorage::~FilesystemStorage() {
}

void FilesystemStorage::Process(TilePtr tile) {
	if (!tile->IsLoaded()) { /* loading */
		std::string path = m_StorageRoot + PathFromCoords(tile->GetX(), tile->GetY(), tile->GetLevel(), tile->GetType());
		try {
			int f;

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
		} catch(ErrnoException &e) {
			/* unable to load current tile, i.e. tile is not in a storage
			 * THIS is not an error */
		} catch(std::exception &e) {
			/* unable to load current tile, i.e. tile is not in a storage
			 * THIS is not an error */
		}
	} else if (tile->IsSaveable()) { /* saving */
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
}

std::string FilesystemStorage::PathFromCoords(int x, int y, int level, int type) {
	std::string path;
	std::string name;

	int i;
	char *ext = "";
	ext = ".jpg";

	for (i = 0; i <= level; i++) {
		int bit = 1 << (level-i);

		if (x & bit) {
			if (y & bit)    name += "2";
			else            name += "1";
		} else {
			if (y & bit)    name += "3";
			else            name += "0";
		}

		if (i < (level+1 & ~0x3)) {
			if (x & bit) {
				if (y & bit)    path += "2";
				else            path += "1";
			} else {
				if (y & bit)    path += "3";
				else            path += "0";
			}

			if (i % 4 == 3)
			path += "/";
		}
	}

	return std::string("/") + path + name + ext;
}
