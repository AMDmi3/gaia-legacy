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

#include "WorldWindTileManager.h"

namespace gaia {

WorldWindTileManager::WorldWindTileManager(TileStorage *ts): m_TextureRoot(new TextureTile(0, 0, 0, TextureTilePtr(0))) {
	m_FirstTileStorage = ts;
	m_nTextureTiles = 1;
}

WorldWindTileManager::~WorldWindTileManager() {
}

TextureTilePtr WorldWindTileManager::GetTexture(int x, int y, int level) {
	TextureTilePtr cur = m_TextureRoot;

	for (int curlevel = 0; curlevel <= level; curlevel++) {
		int cx = (x >> (level - curlevel));
		int cy = (y >> (level - curlevel));

		int dx = cx & 1;
		int dy = cy & 1;

		if (cur->GetChild(dx, dy) == 0) {
			TextureTilePtr child = new TextureTile(cx, cy, curlevel, cur);

			cur->SetChild(dx, dy, child);
			m_nTextureTiles++;
			m_FirstTileStorage->Enqueue(TilePtr((Tile*)child.GetPtr()));	// XXX: unsafe. double check
			cur = child;
		} else
			cur = cur->GetChild(dx, dy);

		cur->Touch();
	}

	return cur;
}

TextureTilePtr WorldWindTileManager::FindTextureToDrop(TextureTilePtr cur, TextureTilePtr best) {
	if (cur == 0)
		return best;

	if (cur->GetParent() != 0 && cur->IsLeaf() && cur->IsOld()) {
		if (best == 0 || cur->GetLevel() > best->GetLevel())
			best = cur;
		else if (best == 0 || cur->GetAge() > best->GetAge())
			best = cur;
	}

	for (int i = 0; i < 4; i++)
		best = FindTextureToDrop(cur->GetChild(i), best);

	return best;
}

int WorldWindTileManager::Cleanup() {
	/*
	 * How cache cleanup works:
	 *
	 * 0. Tiles with children are not dropped
	 * 0. Tiles recently used (!IsOld) are not dropped
	 * 0. Root is not dropped
	 * 1. Tile with highest level is dropped first.
	 * 2. Of same level, Tile with higher age is dropped first.
	 *
	 * This ensures:
	 * - Minimal number of textures loaded
	 * - All visible tiles are always in memory
	 * - No reloading tiles on zoomout
	 */

	if (m_nTextureTiles <= DEFAULT_MAX_TEXTURES)
		return 0;

	TextureTilePtr victim = FindTextureToDrop(m_TextureRoot, 0);

	if (victim == 0)
		return 0;

	for (int i = 0; i < 4; i++)
		if (victim->GetParent()->GetChild(i) == victim) {
			victim->GetParent()->SetChild(i, 0);	/* after this, victim is doomed */
			victim->Unload();			/* be sure no opengl will be touched in threads XXX: review this */
			m_nTextureTiles--;
			return 1;
		}

	return 0;
}

} /* namespace gaia */
