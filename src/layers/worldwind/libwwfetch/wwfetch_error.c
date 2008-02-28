/* 
 * Copyright (C) 2006-2008 Dmitry Marakasov
 *
 * This file is part of Gaia.
 *
 * Gaia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gaia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gaia.  If not, see <http://www.gnu.org/licenses/>.
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
