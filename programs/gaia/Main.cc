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

#include <SDL.h>
#include <getopt.h>

#include "PreloadedTextureManager.h"
#include "Hud.h"
#include "FlatEarthView.h"
#include "SphereEarthView.h"

#include "TestLayer.h"
#include "WorldWindLayer.h"
#include "GridLayer.h"
#include "GPSLayer.h"

#include "Data.h"

#include "Messages.h"

#define GAIA_TITLE "Gaia " GAIA_VERSION

Layer			*g_Layers[256];
int			g_nLayers = 0;

EarthView		*g_EarthView = 0;
//Hud			*g_Hud = 0;

int			g_WindowWidth = 800;
int			g_WindowHeight = 600;

int			g_MouseLeftDownX;
int			g_MouseLeftDownY;
int			g_MouseMiddleDownX;
int			g_MouseMiddleDownY;
int			g_MouseRightDownX;
int			g_MouseRightDownY;
int			g_MouseDownMask = 0;

static struct option longopts[] = {
	{ "world",	required_argument,	0,	'w' },
	{ "test",	no_argument,		0,	't' },
	{ "grid",	no_argument,		0,	'g' },
	{ "gps",	required_argument,	0,	'p' },
	{ "help",	no_argument,		0,	'h' },
};

void usage() {
	printf("gaia %s\n", GAIA_VERSION);
	printf("\nOptions:\n");
	printf("  -w, --world=MODEL   Select world model\n");
	printf("  -t, --test          Add test layer\n");
	printf("  -g, --grid          Add grid layer\n");
	printf("  -p, --gps=SOURCE    Add GPS layer\n");
	printf("\nKeys:\n");
	printf("  Arrow keys    Pan view\n");
	printf("  =/-           Zoom in/out\n");
	printf("  Q, Esc        Quit\n");
	printf("\nMouse:\n");
	printf("  Drag with left button    Pan view\n");
	printf("  Drag with right button   Zoom in/out\n");
	printf("  Wheel                    Zoom in/out\n");
	printf("\n");
}

void cleanup() {
	debug("Destroying objects\n");

	delete g_EarthView;
//	delete g_Hud;

	int i;
	for (i = 0; i < g_nLayers; i++)
		delete g_Layers[i];

	PreloadedTextureManager::Instance()->Clear();

	debug("Shutting down SDL\n");
	SDL_Quit();
}

int loop() {
	SDL_Event event;

	while (SDL_PollEvent(&event))
		switch(event.type) {
		case SDL_QUIT:
			return 0;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				g_MouseLeftDownX = event.button.x;
				g_MouseLeftDownY = event.button.y;
				g_MouseDownMask |= SDL_BUTTON_LMASK;
				g_EarthView->StartDrag(event.motion.x, event.motion.y, NAV_DRAG_PAN);
				break;
			case SDL_BUTTON_MIDDLE:
				g_MouseMiddleDownX = event.button.x;
				g_MouseMiddleDownY = event.button.y;
				g_MouseDownMask |= SDL_BUTTON_MMASK;
				break;
			case SDL_BUTTON_RIGHT:
				g_MouseRightDownX = event.button.x;
				g_MouseRightDownY = event.button.y;
				g_MouseDownMask |= SDL_BUTTON_RMASK;
				g_EarthView->StartDrag(event.motion.x, event.motion.y, NAV_DRAG_ZOOM);
				break;
			case SDL_BUTTON_WHEELUP:
				g_EarthView->SingleMovement(NAV_ZOOM_IN);
				break;
			case SDL_BUTTON_WHEELDOWN:
				g_EarthView->SingleMovement(NAV_ZOOM_OUT);
				break;
			default:
				break;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				g_MouseDownMask &= ~SDL_BUTTON_LMASK;
				break;
			case SDL_BUTTON_MIDDLE:
				g_MouseDownMask &= ~SDL_BUTTON_MMASK;
				break;
			case SDL_BUTTON_RIGHT:
				g_MouseDownMask &= ~SDL_BUTTON_RMASK;
				break;
			default:
				break;
			}
			break;

		case SDL_MOUSEMOTION:
			if (g_MouseDownMask & SDL_BUTTON_LMASK)
				g_EarthView->Drag(g_MouseLeftDownX, g_MouseLeftDownY, event.motion.x, event.motion.y, NAV_DRAG_PAN);
			if (g_MouseDownMask & SDL_BUTTON_RMASK)
				g_EarthView->Drag(g_MouseRightDownX, g_MouseRightDownY, event.motion.x, event.motion.y, NAV_DRAG_ZOOM);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				return 0;
			case SDLK_LEFT:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_PAN_LEFT) : g_EarthView->StopMovement(NAV_PAN_LEFT);
				break;
			case SDLK_RIGHT:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_PAN_RIGHT) : g_EarthView->StopMovement(NAV_PAN_RIGHT);
				break;
			case SDLK_DOWN:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_PAN_DOWN) : g_EarthView->StopMovement(NAV_PAN_DOWN);
				break;
			case SDLK_UP:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_PAN_UP) : g_EarthView->StopMovement(NAV_PAN_UP);
				break;
			case SDLK_PAGEUP:
			case SDLK_EQUALS:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_ZOOM_IN) : g_EarthView->StopMovement(NAV_ZOOM_IN);
				break;
			case SDLK_PAGEDOWN:
			case SDLK_MINUS:
				event.type == SDL_KEYDOWN ? g_EarthView->StartMovement(NAV_ZOOM_OUT) : g_EarthView->StopMovement(NAV_ZOOM_OUT);
				break;
			default:
				break;
			}
			break;

		case SDL_VIDEORESIZE:
			g_WindowWidth = event.resize.w;
			g_WindowHeight = event.resize.h;
			if (SDL_SetVideoMode(g_WindowWidth, g_WindowHeight, 32, SDL_OPENGL | SDL_RESIZABLE) == NULL) {
				warning("Couldn't change GL mode: %s\n", SDL_GetError());
				return 0;
			}
			glViewport(0, 0, g_WindowWidth, g_WindowHeight);
			g_EarthView->Resize(g_WindowWidth, g_WindowHeight);
			break;

		default:
			break;
		}

	return 1;
}

int main(int argc, char **argv) {
	int ch;

	int use_sphere = 0;

	int use_test = 0;
	int use_grid = 0;
	int use_gps = 0;
	char *gps_source = "";

	/* arguments parsing */
	while ((ch = getopt_long(argc, argv, "w:tgp:h", longopts, NULL)) != -1)
		switch (ch) {
		case 'w':
			if (strcmp(optarg, "3d") == 0)
				use_sphere = 1;
			else if (strcmp(optarg, "flat") == 0)
				use_sphere = 0;
			else {
				warning("Unknown world model %s\n", optarg);
				exit(1);
			}
			break;
		case 't':
			use_test = 1;
			break;
		case 'g':
			use_grid = 1;
			break;
		case 'p':
			use_gps = 1;
			gps_source = optarg;
			break;
		case 'h':
			usage();
			return 0;
		default:
			usage();
			return 1;
	}

	/* create dotdir */
	std::string dotdir = std::string(getenv("HOME")) + "/.gaia";
	if (mkdir(dotdir.c_str(), 0777) != 0 && errno != EEXIST) {
		warning("Cannot create dotdir (%s): %s\n", dotdir.c_str(), strerror(errno));
		return 1;
	}
	std::string cachedir = dotdir + "/cache";
	if (mkdir(cachedir.c_str(), 0777) != 0 && errno != EEXIST) {
		warning("Cannot create cache directory (%s): %s\n", cachedir.c_str(), strerror(errno));
		return 1;
	}

	/* init SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		warning("Couldn't initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	/* install cleanup handler */
	atexit(cleanup);

	/* set SDL video mode */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_SetVideoMode(g_WindowWidth, g_WindowHeight, 0, SDL_OPENGL | SDL_RESIZABLE) == NULL) {
		warning("Couldn't set video mode: %s\n", SDL_GetError());
		return 1;
	}

	/* set caption */
	SDL_WM_SetCaption(GAIA_TITLE, GAIA_TITLE);

	ticks_t last_time;
	ticks_t last_frame;

	/* startup */
	debug("Initializing\n");
	try {
		/* init timer */
		Timer *timer = Timer::Instance();
		timer->Initialize(SDL_GetTicks());

		last_time = timer->GetTime();
		last_frame = timer->GetFrame();

		/* preloaded textures */
		PreloadedTextureManager::Instance()->LoadPNG(TEXTURE_FONT, DATADIR "/font.png");

		/* layers */
		g_Layers[g_nLayers++] = new WorldWindLayer(cachedir);

		if (use_test)	g_Layers[g_nLayers++] = new TestLayer();
		if (use_grid)	g_Layers[g_nLayers++] = new GridLayer();
		if (use_gps)	{
			GPSLayer *l = new GPSLayer();
			l->AddEntity("Default", gps_source);
			g_Layers[g_nLayers++] = l;
		}

		/* view */
		if (use_sphere)	g_EarthView = new SphereEarthView();
		else		g_EarthView = new FlatEarthView();
		g_EarthView->Resize(g_WindowWidth, g_WindowHeight);

		int i;
		for (i = 0; i < g_nLayers; i++)
			g_EarthView->BindLayer(g_Layers[i]);

		/* hud */
//		g_Hud = new Hud();
	} catch(std::exception &e) {
		warning("Initialization failed: %s\n", e.what());
		return 1;
	}

	/* main loop */
	debug("Entering main loop\n");
	try {
		Timer *timer = Timer::Instance();
		Uint32 delta;
		while (loop()) {
			if ((delta = timer->Update(SDL_GetTicks())) > 0) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				g_EarthView->Animate((double)delta / 1000.0);
				g_EarthView->Render();
				//g_Hud->Render(g_WindowWidth, g_WindowHeight);
				Hud::Instance()->Render(g_WindowWidth, g_WindowHeight);

				SDL_GL_SwapBuffers();

				SDL_Delay(1);
			}
			if (timer->GetTime()-last_time > 5000) {
				warning("FPS: %f, frame time %f ms\n",
						1000.0*(double)(timer->GetFrame() - last_frame)/(double)(timer->GetTime() - last_time),
						(double)(timer->GetTime() - last_time)/(double)(timer->GetFrame() - last_frame));
				last_time = timer->GetTime();
				last_frame = timer->GetFrame();
			}
		}
	} catch(std::exception &e) {
		warning("Exception caught: %s\n", e.what());
		return 1;
	}

	return 0;
}
