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

#ifndef __WWFETCH_INTERNAL_H__
#define __WWFETCH_INTERNAL_H__

#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "wwfetch.h"

enum {
	METHOD_POST,
	METHOD_GET,
};

/* http request forming and handling (wwfetch_http.c) */
wwfetch_error wwfetch_do_http_request(wwfetch *handle, int method, const char *url, void *postdata, size_t postsize, long *code);

/* high level routine to fetch generic data (wwfetch_fetch.h) */
wwfetch_error wwfetch_fetch(wwfetch *handle, char *url);

/* metadata cache cleaning (wwfetch_meta.h) */
void wwfetch_remove_extra_meta(wwfetch *handle, size_t extraspace);

/* authentication (wwfetch_auth.c) */
wwfetch_error wwfetch_authenticate(wwfetch *handle);

/* decryption (wwfetch_crypt.c) */
wwfetch_error wwfetch_decipher_current_file(wwfetch *handle);

#endif
