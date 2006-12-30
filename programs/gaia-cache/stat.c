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

#include "gaia-cache.h"

void rec(int x, int y, int level, int *count) {
	tiledb_error result = tiledb_exists(&g_tiledb, x, y, level);
	if (result == TILEDB_OK) {
		//printf("%d %d %d=>ok\n", x, y, level);
		count[level]++;
	}

	if ((result == TILEDB_OK || result == TILEDB_NOT_FOUND) && level < 20) {
		rec((x<<1)+0, (y<<1)+0, level+1, count);
		rec((x<<1)+1, (y<<1)+0, level+1, count);
		rec((x<<1)+0, (y<<1)+1, level+1, count);
		rec((x<<1)+1, (y<<1)+1, level+1, count);
	}
	if (level == 3) printf(".");fflush(stdout);
}


int action_stat() {
	int count[31];
	int i;
	int total = 0;
	for (i=0; i<31; i++) {
		count[i] = 0;
	}

	if (g_lazylockingflag) tiledb_enable_lazylock(&g_tiledb);
	rec(0, 0, 0, (int *)&count);
	if (g_lazylockingflag) tiledb_disable_lazylock(&g_tiledb);
	printf("\n");

	for (i=0; i<31; i++) {
		printf("%d: %d/%d tiles found\n", i, count[i], (1<<i)*(1<<i));
		total += count[i];
	}
	printf("total: %d tiles found\n", total);
	return 1;
}
