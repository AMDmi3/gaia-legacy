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

#include "gaia-cache.h"

int	g_hasregion = 0;
double	g_region_x1 = 0.0;
double	g_region_y1 = 0.0;
double	g_region_x2 = 0.0;
double	g_region_y2 = 0.0;

char	*g_cachepath = 0;
char	*g_dumpfile = 0;
char	*g_filesdir = 0;
int	g_typemask = 0;

int	g_verboseflag = 0;
int	g_lazylockingflag = 0;

DB_Handle	g_tiledb;

char	cachepathbuffer[1024];

static struct option longopts[] = {
	{ "list",	no_argument,		0,	'L' },
	{ "remove",	no_argument,		0,	'R' },
	{ "import",	no_argument,		0,	'I' },
	{ "export",	no_argument,		0,	'W' },
	{ "dump",	no_argument,		0,	'D' },
	{ "undump",	no_argument,		0,	'U' },
	{ "stat",	no_argument,		0,	'S' },
	{ "help",	no_argument,		0,	'h' },

	{ "cache-path",	required_argument,	0,	'c' },
	{ "dump-file",	required_argument,	0,	'f' },
	{ "files-dir",	required_argument,	0,	'd' },
	{ "type",	required_argument,	0,	't' },
	{ "point",	required_argument,	0,	'p' },
	{ "verbose",	no_argument,		0,	'v' },
	{ "lazylocking",	no_argument,		0,	'l' },
};

void cleanup() {
	tiledb_close(&g_tiledb);
}

int main(int argc, char **argv) {
	opterr = 0;

	int action = 0;

	int ch;

	/* arguments parsing */
	while ((ch = getopt_long(argc, argv, "LRIEDUShc:f:d:t:p:vl", longopts, NULL)) != -1)
		switch (ch) {
		case 'L':
		case 'R':
		case 'I':
		case 'E':
		case 'D':
		case 'U':
		case 'S':
			if (action != 0)
				errx(1, "only one action may be specified");
			action = ch;
			break;
		case 'c':
			g_cachepath = optarg;
			break;
		case 'f':
			g_dumpfile = optarg;
			break;
		case 'd':
			g_filesdir = optarg;
			break;
		case 't':
			if (strcmp(optarg, "image") == 0) {
				g_typemask |= TYPEMASK_IMAGE;
			} else {
				errx(1, "bad data type `%s'", optarg);
			}
			break;
		case 'p':
			//expand_region(optarg);
			errx(1, "-p unsupported ATM");
			break;
		case 'v':
			g_verboseflag = 1;
			break;
		case 'l':
			g_lazylockingflag = 1;
			break;
		case 'h':
			usage();
			return 0;
		default:
			/* getopt displays error on invalid option here, and then we exit */
			errx(1, "unrecognized option `%s'", argv[optind-1]);
			return 1;
		}

	if (g_verboseflag && g_lazylockingflag)
		fprintf(stderr, "using lazy locking\n");

	/* check/fix options */
	if (action == 0)
		errx(1, "no action specified");

	if (!g_typemask)
		g_typemask = TYPEMASK_ALL;

	if (g_cachepath == 0) {
		/* path not specified => create and use default */
		if (snprintf(cachepathbuffer, sizeof(cachepathbuffer), "%s/.gaia", getenv("HOME")) >= sizeof(cachepathbuffer))
			errx(1, "cache path too long");

		if (mkdir(cachepathbuffer, 0777) != 0 && errno != EEXIST)
			err(1, "cannot create ~/.gaia");

		if (snprintf(cachepathbuffer, sizeof(cachepathbuffer), "%s/.gaia/cache", getenv("HOME")) >= sizeof(cachepathbuffer))
			errx(1, "cache path too long");

		if (mkdir(cachepathbuffer, 0777) != 0 && errno != EEXIST)
			err(1, "cannot create ~/.gaia/cache");

		g_cachepath = cachepathbuffer;
	} else {
		/* if path was explicitly specified, it must exist */
		struct stat st;
		if (stat(g_cachepath, &st) == -1)
			err(1, "cannot stat() specified cache directory (%s)", g_cachepath);

		if (!S_ISDIR(st.st_mode))
			errx(1, "please specify an existing directory in cache path");
	}

	/* import/export need directory specified (I think that
	 * using currentdir by default is not the best idea */
	if (action == 'I' || action == 'E') {
		if (g_filesdir == 0)
			errx(1, "no %s directory specified", action == 'I' ? "import" : "export");

		struct stat st;
		if (stat(g_filesdir, &st) == -1)
			err(1, "cannot stat() specified %s directory", action == 'I' ? "import" : "export");

		if (!S_ISDIR(st.st_mode))
			errx(1, "please specify an existing directory for %s", action == 'I' ? "import" : "export");
	}

	/* dump action works with stdin/stdout by default
	 * fear not! when dumping, stdout is checked with isatty */
	if (g_dumpfile == 0)
		g_dumpfile = "-";

	/* open cache */
	int res;

	char cache_name[1024];
	snprintf((char*)&cache_name, 1024, "%s/%s", g_cachepath, "gaia_storage_table");
	if ((res = tiledb_open(&g_tiledb, (char*)&cache_name, CREATE_IF_NOT_EXISTS)) != TILEDB_OK) {
		errx(1, "cannot open gaia cache: %s (code %d)\n", tiledb_strerror(res), res);
	}

	atexit(cleanup);

	/* perform actions */
	if (action == 'L') {
		return action_list();
	} else if (action == 'R') {
		return action_remove();
	} else if (action == 'I') {
		return action_import();
	} else if (action == 'E') {
		return action_export();
	} else if (action == 'D') {
		return action_dump();
	} else if (action == 'U') {
		return action_undump();
	} else if (action == 'S') {
		return action_stat();
	} else {
		errx(1, "bad action");
	}

	return 0;
}

void usage() {
	fprintf(stderr, "gaia-cache %s\n", GAIA_VERSION);
	fprintf(stderr, "\nUsage:\n");
	fprintf(stderr, "   gaia-cache [OPTIONS] ACTION [PARAMETERS]\n");
	fprintf(stderr, "\nActions:\n");
	fprintf(stderr, "  -L, --list        list cache contents\n");
	fprintf(stderr, "  -R, --remove      remove specified region data from cache\n");
	fprintf(stderr, "  -I, --import      import cache from bunch of JPEGs (gaia 0.1.0 cache)\n");
	fprintf(stderr, "  -E, --export      export part of cache as bunch of files\n");
	fprintf(stderr, "  -D, --dump        dump cache in cross-plaform binary format\n");
	fprintf(stderr, "  -U, --undump      suck in previously dumped cache\n");
	fprintf(stderr, "  -S, --stat        display cache statistics\n");
	fprintf(stderr, "  -h, --help        display this help\n");
	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "  -c, --cache-path=PATH   path to cache (default ~/.gaia/cache)\n");
	fprintf(stderr, "  -f, --dump-file=PATH    file to use in dump|undump operations (default stdout/stdin)\n");
	fprintf(stderr, "  -d, --files-dir=PATH    directory to use in import|export operations (default current dir)\n");
	fprintf(stderr, "  -t, --type=TYPE         work only with specified type(s) of data (default all)\n");
	fprintf(stderr, "  -p, --point=COORDS      specify one or more points to limit region (default whole world)\n");
	fprintf(stderr, "  -v, --verbose           increase verbosity\n");
	fprintf(stderr, "  -l, --lazylocking       used lazylocking, single process db access\n");
	fprintf(stderr, "\nData types (for -t)\n");
	fprintf(stderr, "  Only `image' is supported at the moment\n");
	fprintf(stderr, "\nPoints format (for -p)\n");
	fprintf(stderr, "  As geographical coordinates:\n");
	fprintf(stderr, "    DD,ddddd[NSWE]  DD'MM,mmmmm[NSWE] DD'MM'SS,sssss[NSWE]\n");
	fprintf(stderr, "    Period or comma is used to separate fractional part, any other non-\n");
	fprintf(stderr, "    alphanumeric symbol is used to separate degrees/minutes/seconds. Don't\n");
	fprintf(stderr, "    forget N/S W/E suffix\n");
	/* XX Add examples */
	fprintf(stderr, "  As index:\n");
	fprintf(stderr, "    X:Y:L\n");
	fprintf(stderr, "    L is detail level starting with 0 (lowest-detail, covers whole world)\n");
	fprintf(stderr, "    X,Y are coordinates in range [0..(2^level-1)] with (0,0) being lower\n");
	fprintf(stderr, "    left corner of the world\n");
}
