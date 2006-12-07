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

#include "wwfetch_internal.h"

/**
 * Fetch single image from nasa server
 *
 * @param x x coordinate of image [0..(2^level-1)]
 * @param y y coordinate of image [0..(2^level-1)]
 * @param level level of image [0..]
 */
wwfetch_error wwfetch_fetch_image(wwfetch *handle, int x, int y, int level) {
	int res = 1 << (level);

	double x1 = (double)(x)/(double)(res) * 360.0 - 180.0;
	double y1 = (double)(y)/(double)(res) * 360.0 - 180.0;
	double x2 = (double)(x+1)/(double)(res) * 360.0 - 180.0;
	double y2 = (double)(y+1)/(double)(res) * 360.0 - 180.0;

	/* form full url */
	char urlbuf[1024];
	if (snprintf(urlbuf, sizeof(urlbuf), "http://wms.jpl.nasa.gov/wms.cgi?request=GetMap&layers=global_mosaic_base&srs=EPSG:4326&width=256&height=256&bbox=%f,%f,%f,%f&format=image/jpeg&version=1.1.0&styles=", x1, y1, x2, y2) >= sizeof(urlbuf))
		return WWFETCH_SMALL_BUFFER;

	wwfetch_error result = wwfetch_fetch(handle, urlbuf);
	if (result == WWFETCH_OK) {
		/* ensure that we got a jpeg file, (ww sends xml error packets) */
		/* XXX: - is this a correct test for JPEG?
		 *      - what's in that xml data? introduce additional wwfetch_error's?
		 *      - libjpeg does not handle corrupt data gracefully? */
		if (handle->currentsize < 4 || handle->currentdata[0] != 0xff || handle->currentdata[1] != 0xd8 || handle->currentdata[2] != 0xff || handle->currentdata[3] != 0xe0)
			return WWFETCH_FETCH_FAILED;
	}
	return result;
}
