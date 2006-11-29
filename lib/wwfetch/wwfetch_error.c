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
		return "No error";
	case WWFETCH_MALLOC_FAILED:
		return "Malloc failed";
	case WWFETCH_SMALL_BUFFER:
		return "Internal buffer is too small (may be bug in library)";
	case WWFETCH_CURL_INIT_FAILED:
		return "Libcurl initialization failed";
	case WWFETCH_CURL_ERROR:
		return "Libcurl error";	/* see wwfetch_getlastcurlerror() */
	case WWFETCH_CORRUPT_DATA:
		return "Unexpected or corrupt data encountered";
	case WWFETCH_FETCH_FAILED:
		return "Data fetch failed";
	case WWFETCH_NOT_FOUND:
		return "Data not found";
	default:
		return "Unknown error";
	}
}

int wwfetch_getlastcurlerror(wwfetch *handle) {
	return handle->lastcurlerror;
}
