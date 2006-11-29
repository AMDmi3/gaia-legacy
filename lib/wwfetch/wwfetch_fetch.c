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
 * Internal function to fetch data via HTTP
 *
 * Given full URL, does all required HTTP requests to server to
 * get requested data. Automatically fetches dbRoot and does
 * authentication if needed. Also decrypts data.
 *
 * @param URL full URL for request
 */
wwfetch_error wwfetch_fetch(wwfetch *handle, char *url) {
	wwfetch_error result;
	long code;

	/* try to get a file */
	if ((result = wwfetch_do_http_request(handle, METHOD_GET, url, 0, 0, &code)) != WWFETCH_OK)
		return result;

	/* success */
	if (code == 200)
		return WWFETCH_OK;

	/* not found */
	if (code == 404)
		return WWFETCH_NOT_FOUND;

	/* other result codes */
	return WWFETCH_FETCH_FAILED;
}
