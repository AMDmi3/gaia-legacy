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

#ifndef __GAIA_CACHE_H__
#define __GAIA_CACHE_H__

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fts.h>
#include <fcntl.h>
#include <unistd.h>

#include <tiledb.h>

//#include "../gaia/Version.h"

#define TYPEMASK_IMAGE	0x01
#define TYPEMASK_ALL	0x01

extern int	g_hasregion;
extern double	g_region_x1;
extern double	g_region_y1;
extern double	g_region_x2;
extern double	g_region_y2;

extern char	*g_cachepath;
extern char	*g_dumpfile;
extern char	*g_filesdir;
extern int	g_typemask;

extern int	g_verboseflag;
extern int	g_lazylockingflag;

extern DB_Handle g_tiledb;

void usage();

int action_list();
int action_remove();
int action_import();
int action_export();
int action_defrag();
int action_dump();
int action_undump();
int action_stat();

#endif
