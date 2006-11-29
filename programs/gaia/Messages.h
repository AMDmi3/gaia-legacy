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

#ifndef __GAIA__STDOUT_H__
#define __GAIA__STDOUT_H__

#include "Timer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef DEBUG
void debug(const char *fmt, ...);
#else
#define debug(format, args...)
#endif

void warning(const char *fmt, ...);
void fatal(const char *fmt, ...);

#endif
