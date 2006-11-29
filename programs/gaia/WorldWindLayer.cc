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

#include "WorldWindLayer.h"

WorldWindLayer::WorldWindLayer(WorldWindTileManager *tm) {
	m_TileManager = tm;
}

WorldWindLayer::~WorldWindLayer() {
}

#define EPSILON 0.00001

void WorldWindLayer::RenderRegion(Region *rgn) {
	/* select maximal tile level for this region
	 * (later every tile is checked and it's individual level may be lowered */
	int level = max4(GetSplitLevel(rgn->w[1].x - rgn->w[0].x, rgn->proj_length(0,1)),
			GetSplitLevel(rgn->w[1].x - rgn->w[0].x, rgn->proj_length(2,3)),
			GetSplitLevel(rgn->w[1].y - rgn->w[0].y, rgn->proj_length(1,2)),
			GetSplitLevel(rgn->w[1].y - rgn->w[0].y, rgn->proj_length(3,0)));

	double step = 1.0/(double)(1 << level);

	int x,y;

	/* min/max indices of tiles that fit into current region */
	int x0 = (int)((rgn->w[0].x + 0.5) / step);
	int x1 = (int)((rgn->w[1].x + 0.5) / step - EPSILON);

	int y0 = (int)((rgn->w[0].y + 0.5) / step);
	int y1 = (int)((rgn->w[1].y + 0.5) / step - EPSILON);

	/* XXX: optimize this: coords should be calculated 1 time for each
	 * vertex, not 4; use addition instead of recalculating linearly
	 * distributed value; move expressions out of inner loop */
	glColor3f(1.0, 1.0, 1.0);
	for (x = x0; x <= x1; x++) {
		for (y = y0; y <= y1; y++) {
			/* coords of sides for current tile */
			double tile_x0 = (double)x * step - 0.5;
			double tile_x1 = (double)(x+1) * step - 0.5;
			double tile_y0 = (double)y * step - 0.5;
			double tile_y1 = (double)(y+1) * step - 0.5;

			/* texture coordinates for the tile */
			double texture_x0 = 0.0;
			double texture_x1 = 1.0;
			double texture_y0 = 1.0;
			double texture_y1 = 0.0;

			/* tile corners in region-local coordinates */
			double xk0 = (tile_x0 - rgn->w[0].x)/(rgn->w[1].x - rgn->w[0].x);
			double xk1 = (tile_x1 - rgn->w[0].x)/(rgn->w[1].x - rgn->w[0].x);
			double yk0 = (tile_y0 - rgn->w[0].y)/(rgn->w[1].y - rgn->w[0].y);
			double yk1 = (tile_y1 - rgn->w[0].y)/(rgn->w[1].y - rgn->w[0].y);

			/* projected coordinates for this tile; should be placed before clipping, as it's used in LOD */
			Vector3d p0 = rgn->p[0]*(1.0-xk0)*(1.0-yk0) + rgn->p[1]*xk0*(1.0-yk0) + rgn->p[2]*xk0*yk0 + rgn->p[3]*(1.0-xk0)*yk0;
			Vector3d p1 = rgn->p[0]*(1.0-xk1)*(1.0-yk0) + rgn->p[1]*xk1*(1.0-yk0) + rgn->p[2]*xk1*yk0 + rgn->p[3]*(1.0-xk1)*yk0;
			Vector3d p2 = rgn->p[0]*(1.0-xk1)*(1.0-yk1) + rgn->p[1]*xk1*(1.0-yk1) + rgn->p[2]*xk1*yk1 + rgn->p[3]*(1.0-xk1)*yk1;
			Vector3d p3 = rgn->p[0]*(1.0-xk0)*(1.0-yk1) + rgn->p[1]*xk0*(1.0-yk1) + rgn->p[2]*xk0*yk1 + rgn->p[3]*(1.0-xk0)*yk1;

			/* projected lengths of tile sides */
			//double pl01 = (p0 - p1).length();
			//double pl12 = (p1 - p2).length();
			//double pl23 = (p2 - p3).length();
			//double pl30 = (p3 - p0).length();

			/* clip tile to region */
			if (xk0 < 0.0) texture_x0 = (-xk0)/(xk1-xk0);
			if (xk1 > 1.0) texture_x1 = 1.0-(xk1-1.0)/(xk1-xk0);
			if (yk0 < 0.0) texture_y0 = 1.0-(-yk0)/(yk1-yk0);
			if (yk1 > 1.0) texture_y1 = (yk1-1.0)/(yk1-yk0);

			if (xk0 < 0.0) xk0 = 0.0;
			if (xk1 > 1.0) xk1 = 1.0;
			if (yk0 < 0.0) yk0 = 0.0;
			if (yk1 > 1.0) yk1 = 1.0;

			/* virtual vertices for this tile */
			Vector3d v0 = rgn->v[0]*(1.0-xk0)*(1.0-yk0) + rgn->v[1]*xk0*(1.0-yk0) + rgn->v[2]*xk0*yk0 + rgn->v[3]*(1.0-xk0)*yk0;
			Vector3d v1 = rgn->v[0]*(1.0-xk1)*(1.0-yk0) + rgn->v[1]*xk1*(1.0-yk0) + rgn->v[2]*xk1*yk0 + rgn->v[3]*(1.0-xk1)*yk0;
			Vector3d v2 = rgn->v[0]*(1.0-xk1)*(1.0-yk1) + rgn->v[1]*xk1*(1.0-yk1) + rgn->v[2]*xk1*yk1 + rgn->v[3]*(1.0-xk1)*yk1;
			Vector3d v3 = rgn->v[0]*(1.0-xk0)*(1.0-yk1) + rgn->v[1]*xk0*(1.0-yk1) + rgn->v[2]*xk0*yk1 + rgn->v[3]*(1.0-xk0)*yk1;

			int reall = level;
			int realx = x;
			int realy = y;

			/* ensure we use correct level for this tile (without this
			 * we may actually have higher level, as level = max4(...) */
/*			while ( (pl01 < MIN_TEXTURE_DISTANCE || pl12 < MIN_TEXTURE_DISTANCE || pl23 < MIN_TEXTURE_DISTANCE || pl30 < MIN_TEXTURE_DISTANCE) && reall > 0) {
				texture_x0 = ((double)(realx & 1) + texture_x0)/2.0;
				texture_y0 = ((double)(1 ^ (realy & 1)) + texture_y0)/2.0;
				texture_x1 = ((double)(realx & 1) + texture_x1)/2.0;
				texture_y1 = ((double)(1 ^ (realy & 1)) + texture_y1)/2.0;

				realx /= 2;
				realy /= 2;
				reall--;

				pl01 *= 2.0;
				pl12 *= 2.0;
				pl23 *= 2.0;
				pl30 *= 2.0;
			}*/

			/* The Rendering */
			glEnable(GL_TEXTURE_2D);
			glColor3d(1.0, 1.0, 1.0);

			TextureTilePtr texture = m_TileManager->GetTexture(realx, realy, reall);

			/* try to use needed texture */
			if (texture->IsReady()) {
				texture->SetTexture();
			} else {
				/* find best available texture */
				TextureTilePtr cur;
				while (reall >= 0) {
					cur = m_TileManager->GetTexture(realx, realy, reall);
					if (cur->IsReady())
						break;
					else {
						/* change texture coordinates for lower level*/
						texture_x0 = ((double)(realx & 1) + texture_x0)/2.0;
						texture_y0 = ((double)(1 ^ (realy & 1)) + texture_y0)/2.0;
						texture_x1 = ((double)(realx & 1) + texture_x1)/2.0;
						texture_y1 = ((double)(1 ^ (realy & 1)) + texture_y1)/2.0;
					}

					realx /= 2;
					realy /= 2;
					reall--;
				}

				if (cur->IsReady()) {
					/* use this texture */
					cur->SetTexture();
				} else {
					/* we have no texture at all */
					glDisable(GL_TEXTURE_2D);
					glColor3f(0.25f, 0.25f, 0.25f);
				}
			}

			/* render tile */
			glBegin(GL_QUADS);
			glTexCoord2d(texture_x0, texture_y0);
			glVertex3d(v0.x, v0.y, v0.z);
			glTexCoord2d(texture_x1, texture_y0);
			glVertex3d(v1.x, v1.y, v1.z);
			glTexCoord2d(texture_x1, texture_y1);
			glVertex3d(v2.x, v2.y, v2.z);
			glTexCoord2d(texture_x0, texture_y1);
			glVertex3d(v3.x, v3.y, v3.z);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}

	for (std::vector<SlaveLayer*>::iterator i = m_SlaveLayers.begin(); i < m_SlaveLayers.end(); i++)
		if ((*i)->GetCap(SLAVELAYERCAP_OVERDRAW))
			(*i)->Overdraw(rgn);
}

int WorldWindLayer::GetSplitLevel(double wlen, double plen) {
	int level = 0;
	while(1) {
		if (plen/wlen/(double)(1 << (level+1)) < MIN_TEXTURE_DISTANCE)
			return level;
		level++;
	}
}
