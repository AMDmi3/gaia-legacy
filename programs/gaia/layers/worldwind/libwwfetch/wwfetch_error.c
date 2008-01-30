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

const char *wwfetch_strerror(int err) {
	switch (err) {

	case WWFETCH_OK:
		return "successfull";
	case WWFETCH_MALLOC_FAILED:
		return "malloc failed";
	case WWFETCH_SMALL_BUFFER:
		return "internal buffer is too small (may be bug in libwwfetch)";
	case WWFETCH_CURL_INIT_FAILED:
		return "libcurl initialization failed";
	case WWFETCH_CURL_ERROR:
		return "libcurl error";	/* see wwfetch_getlastcurlerror() */
	case WWFETCH_CORRUPT_DATA:
		return "unexpected or corrupt data received";
	case WWFETCH_FETCH_FAILED:
		return "error fetching object";
	case WWFETCH_NOT_FOUND:
		return "object not found";
	default:
		return "unknown error";
	}
}

CURLcode wwfetch_getlastcurlerror(wwfetch *handle) {
	return handle->lastcurlerror;
}
