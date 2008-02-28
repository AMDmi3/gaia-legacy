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

	/* other result codes */
	return WWFETCH_FETCH_FAILED;
}
