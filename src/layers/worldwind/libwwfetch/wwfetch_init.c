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
 * Initialize wwfetch library
 */
wwfetch_error wwfetch_init(wwfetch *handle) {
	memset(handle, 0, sizeof(wwfetch));

	/* init certain fields */
	if ((handle->curl = curl_easy_init()) == 0) {
		wwfetch_cleanup(handle);
		return WWFETCH_CURL_INIT_FAILED;
	}

	return WWFETCH_OK;
}

/**
 * Destroy wwfetch handle and free all data used by library
 */
void wwfetch_cleanup(wwfetch *handle) {
	/* first call curl cleanup, to teardown all connections
	 * and be sure not data freed below is used by anyone */
	if (handle->curl)
		curl_easy_cleanup(handle->curl);

	if (handle->curlheaders)
		curl_slist_free_all(handle->curlheaders);

	if (handle->currentdata)
		free(handle->currentdata);
}
