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

#ifndef __WWFETCH_H__
#define __WWFETCH_H__

#include <curl/curl.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* error codes */
typedef enum {
	/* no error */
	WWFETCH_OK = 0,

	/* malloc failed */
	WWFETCH_MALLOC_FAILED,

	/* internal buffer too small */
	WWFETCH_SMALL_BUFFER,

	/* curl error */
	WWFETCH_CURL_ERROR,

	/* curl init failed */
	WWFETCH_CURL_INIT_FAILED,

	/* unexpected data returned */
	WWFETCH_CORRUPT_DATA,

	/* unable to fetch data requested */
	WWFETCH_FETCH_FAILED,

	/* data requested does not exist */
	WWFETCH_NOT_FOUND,
} wwfetch_error;

/* wwfetch handle */
typedef struct wwfetch {
	/* cURL handle */
	CURL		*curl;

	/* cURL error code */
	CURLcode	lastcurlerror;

	/* headers for curl */
	struct curl_slist *curlheaders;

	/* buffer for file being downloaded */
	size_t		currentsize;
	unsigned char	*currentdata;
} wwfetch;

/* init/cleanup */
wwfetch_error	wwfetch_init(wwfetch *handle);
void		wwfetch_cleanup(wwfetch *handle);

/* errors */
const char	*wwfetch_strerror(int err);
CURLcode	wwfetch_getlastcurlerror(wwfetch *handle);

/* fetch specific types of data */
wwfetch_error	wwfetch_fetch_image(wwfetch *handle, int x, int y, int level);

/* retrieve data from library */
size_t		wwfetch_get_data_size(wwfetch *handle);
void		*wwfetch_get_data_ptr(wwfetch *handle);

#ifdef __cplusplus
}
#endif

#endif
