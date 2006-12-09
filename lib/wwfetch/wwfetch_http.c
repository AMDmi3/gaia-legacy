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
 * Internal function used in curl
 *
 * Is called when chunk of data is received. Thus, appends it to
 * buffer storing curently downloaded file (handle->currentdata)
 *
 * See libCURL documentation for description.
 */
static size_t wwfetch_curl_writefunction(char *ptr, size_t size, size_t nmemb, void *stream) {
	wwfetch *handle = (wwfetch *)stream;

	size_t newsize = handle->currentsize + size*nmemb;

	unsigned char *newdata = (unsigned char*)realloc((void*)handle->currentdata, newsize);

	if (newdata == 0)
		return 0;

	handle->currentdata = newdata;

	memcpy(&handle->currentdata[handle->currentsize], ptr, size*nmemb);

	handle->currentsize = newsize;

	return size * nmemb;
}

/**
 * Internal function to form and execute single HTTP request
 *
 * @param method request method (METHOD_POST | METHOD_GET)
 * @param url full URL for request
 * @param postdata (POST only) pointer to data to send to server
 * @param postsize (POST only) length of postdata
 * @param code address of long to store HTTP response code to
 */
wwfetch_error wwfetch_do_http_request(wwfetch *handle, int method, const char *url, void *postdata, size_t postsize, long *code) {
	/* free last downloaded file, if any */
	if (handle->currentdata)
		free(handle->currentdata);

	handle->currentdata = 0;
	handle->currentsize = 0;

	/* drop all curl headers */
	if (handle->curlheaders) {
		curl_slist_free_all(handle->curlheaders);
		handle->curlheaders = 0;
	}

	/* reset curl */
	curl_easy_reset(handle->curl);

	/* init request */
	if (method == METHOD_POST) {
		if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_POSTFIELDS, postdata)) != CURLE_OK)
			return WWFETCH_CURL_ERROR;
		if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_POSTFIELDSIZE, postsize)) != CURLE_OK)
			return WWFETCH_CURL_ERROR;
	} else {
		if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_HTTPGET, 1)) != CURLE_OK)
			return WWFETCH_CURL_ERROR;
	}

	/* init headers */
	struct curl_slist *tmplist;

	/* Connection: */
	if ((tmplist = curl_slist_append(handle->curlheaders, "Connection: Keep-Alive")) == 0)
		return WWFETCH_MALLOC_FAILED;

	handle->curlheaders = tmplist;

	/* options */
	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_HTTPHEADER, handle->curlheaders)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_USERAGENT, "gaia/%s", GAIA_VERSION)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_URL, url)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_TIMEOUT, 30)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_NOSIGNAL, 1)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_WRITEFUNCTION, wwfetch_curl_writefunction)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_setopt(handle->curl, CURLOPT_WRITEDATA, handle)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	int i;
	if ((handle->lastcurlerror = curl_easy_perform(handle->curl)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	if ((handle->lastcurlerror = curl_easy_getinfo(handle->curl, CURLINFO_RESPONSE_CODE, code)) != CURLE_OK)
		return WWFETCH_CURL_ERROR;

	return WWFETCH_OK;
}
