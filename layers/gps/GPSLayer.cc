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

#include "GPSLayer.h"

#include <string.h>

#include "Timer.h"
#include "GPSSourceNMEA.h"
#include "GPSSourceTest.h"
#ifdef WITH_GPSD
#       include "GPSSourceGPSD.h"
#endif
#include "Hud.h"
#include "HudIcon.h"
#include "Data.h"

namespace gaia {

IMPLEMENT_GAIA_LAYER(GPSLayer, "GPS", LAYERGROUP_TRANSPARENT, 0);

GPSLayer::GPSLayer() {
}

GPSLayer::~GPSLayer() {
	int *result;

	/* destroy all allocated resources for each entity*/
	for (std::vector<GPSEntity*>::iterator i = m_Entities.begin(); i < m_Entities.end(); i++) {
		/* thread */
		pthread_cancel((*i)->thread);
		pthread_join((*i)->thread, (void**)&result);

		/* mutex */
		pthread_mutex_destroy(&(*i)->datamutex);

		/* data structures */
		delete (*i)->source;
		delete (*i);
	}
}

void* GPSLayer::StartEntityThread(void* pentity) {
	GPSEntity *e = (GPSEntity*)pentity;

	while(1) {
		GPSData	newdata;

		int res = e->source->Poll(&newdata);

		if (res) {
			pthread_mutex_lock(&e->datamutex);
			/* don't save point in save coordinates */
			if (e->data.size() == 0 ||
					fabs(e->data.back().longitude - newdata.longitude) > DATA_CONST_DIST ||
					fabs(e->data.back().latitude - newdata.latitude) > DATA_CONST_DIST) {
				e->data.push_back(newdata);
			}
			e->lastupdate = Timer::Instance()->GetTime();
			pthread_mutex_unlock(&e->datamutex);
		}
	}

	return (void*)1;
}

void GPSLayer::AddEntity(char *name, char *source) {
	/* First, create corresponding source */
	GPSSource *s;
	if (strlen(source) >= 7 && memcmp(source, "gpsd://", 7) == 0) {
#ifdef WITH_GPSD
		source = source+7;
		if (strlen(source) > 1023)
			throw Exception("GPS source description too long");

		char buf[1024];
		strcpy(buf, source);

		char *delim = strchr(buf, ':');
		if (delim) {
			/* source is in gpsd://host:port form */
			*(delim++) = '\0';
			s = new GPSSourceGPSD(buf, delim);
		} else {
			s = new GPSSourceGPSD(buf, "2947");
		}
#else
		throw Exception("Compiled without GPSD support");
#endif
	} else if (strlen(source) >= 7 && memcmp(source, "nmea://", 7) == 0) {
		source = source+7;
		s = new GPSSourceNMEA(source);
	} else if (strlen(source) >= 7 && memcmp(source, "test://", 7) == 0) {
		s = new GPSSourceTest();
	} else {
		throw Exception("Unknown GPS source");
	}

	GPSEntity *e = new GPSEntity();
	strncpy(e->name, name, MAX_ENTITY_NAME);
	e->name[MAX_ENTITY_NAME-1] = '\0';
	e->lastupdate = 0;
	e->color[0] = 0;	/* TODO: take color from somewhere */
	e->color[1] = 128;
	e->color[2] = 255;
	e->source = s;

	if (pthread_mutex_init(&e->datamutex, 0) != 0) {
		delete s;
		delete e;
		throw ErrnoException("pthread_mutex_init() failed", errno);
	}

	if (pthread_create(&e->thread, 0, StartEntityThread, (void*)e) != 0) {
		pthread_mutex_destroy(&e->datamutex);
		delete s;
		delete e;
		throw ErrnoException("pthread_create() failed", errno);
	}

	m_Entities.push_back(e);
}

void GPSLayer::RenderRegion(Region *rgn) {
	double model[16];
	double proj[16];
	int view[4];

	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetIntegerv(GL_VIEWPORT, view);

	/* draw all entities */
	for (std::vector<GPSEntity*>::iterator i = m_Entities.begin(); i < m_Entities.end(); i++) {
		pthread_mutex_lock(&(*i)->datamutex);
		int valid = Timer::Instance()->GetTime() - (*i)->lastupdate < DATA_ROT_TIME;
		for (std::vector<GPSData>::iterator d = (*i)->data.begin(); d < (*i)->data.end(); d++) {
			double x = (*d).longitude;
			double y = (*d).latitude;

			if (x >= rgn->w[0].x && x < rgn->w[1].x && y >= rgn->w[0].y && y < rgn->w[1].y) {
				double xk = (x - rgn->w[0].x)/(rgn->w[1].x - rgn->w[0].x);
				double yk = (y - rgn->w[0].y)/(rgn->w[1].y - rgn->w[0].y);

				Vector3d v = rgn->v[0]*(1.0-xk)*(1.0-yk) + rgn->v[1]*xk*(1.0-yk) + rgn->v[2]*xk*yk + rgn->v[3]*(1.0-xk)*yk;
				Vector3d p;	// projected

				gluProject(v.x, v.y, v.z, model, proj, view, &p.x, &p.y, &p.z);

				p.x += 0.5;
				p.y += 0.5;

				if (d + 1 == (*i)->data.end()) {
					if (valid)	Hud::Instance()->AddObject(ICON_GPS_POSITION((int)p.x, (int)p.y, 100));
					else		Hud::Instance()->AddObject(ICON_GPS_POSITION_INVALID((int)p.x, (int)p.y, 100));
				} else
					Hud::Instance()->AddObject(ICON_GPS_POINT((int)p.x, (int)p.y, 99));
			}
		}
		pthread_mutex_unlock(&(*i)->datamutex);
	}
}

} /* namespace gaia */
