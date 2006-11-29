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

#include "Messages.h"

#ifdef DEBUG
void debug(const char *fmt, ...) {
	struct timeval tv;
	gettimeofday(&tv, 0);

	fprintf(stderr, "%ld.%06ld ", tv.tv_sec, tv.tv_usec);

	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}
#endif

void warning(const char *fmt, ...) {
	struct timeval tv;
	gettimeofday(&tv, 0);

	fprintf(stderr, "%ld.%06ld ", tv.tv_sec, tv.tv_usec);

	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);
}

void fatal(const char *fmt, ...) {
	struct timeval tv;
	gettimeofday(&tv, 0);

	fprintf(stderr, "%ld.%06ld ", tv.tv_sec, tv.tv_usec);

	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	va_end(arglist);

	exit(1);
}
