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

#ifndef __GAIA__GPSSOURCENMEA_H__
#define __GAIA__GPSSOURCENMEA_H__

#include <vector>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "GPSSource.h"

#define MAX_SENTENCE_LENGTH 1024
#define MAX_SENTENCE_FIELDS 64

/**
 * GPS source class that parses NMEA.
 *
 * Uses local file (most likely, device) and
 * parses NMEA input
 *
 * @todo ability to set baudrate line nmea:///dev/ttyd0@4800
 */
class GPSSourceNMEA: public GPSSource {
public:
	/**
	 * Constructor
	 *
	 * @param path path to device (or socket/fifo/file)
	 */
	GPSSourceNMEA(char *path);

	/**
	 * Destructor
	 */
	virtual ~GPSSourceNMEA();

	virtual int Poll(GPSData *target);

private:
	/**
	 * Parse GPRMC (recommended minimum data) sentence
	 *
	 * @param fields array of null-terminated strings containing sentence fields
	 * @param nfields number of fields
	 * @param target where to store coordinates in case of success
	 * @return 1 if sentence was parsed successfully (i.e. *target valid), 0 otherwise
	 */
	int ParseGPRMC(char **fields, int nfields, GPSData *target);

	/**
	 * Parse one NMEA sentence
	 *
	 * @param sentence pointer to null-terminated string containing sentence
	 * @param target where to store coordinates in case of success
	 * @return 1 if sentence was parsed successfully (i.e. *target valid), 0 otherwise
	 */
	int ParseSentence(char *sentence, GPSData *target);

	/**
	 * Check if checksum on NMEA sentence is correct
	 *
	 * @param sentence pointer to null-terminated string containing sentence (without * and checksum)
	 * @param checksum pointer to null-terminated string containing checksum
	 * @return 1 if checksum is valid, 0 otherwise
	 */
	int TestChecksum(char *sentence, char *checksum);

private:
	int	m_FD;		///< File descriptor
};

#endif
