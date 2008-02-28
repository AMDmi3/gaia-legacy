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

#include "GPSSourceNMEA.h"

#include "Exceptions.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

namespace gaia {

GPSSourceNMEA::GPSSourceNMEA(char *path) {
	if ((m_FD = open(path, O_RDONLY)) == -1)
		throw Exception("open() failed for NMEA GPS source path");
}

GPSSourceNMEA::~GPSSourceNMEA() {
	close(m_FD);
}

int GPSSourceNMEA::Poll(GPSData *target) {
	char sentence[MAX_SENTENCE_LENGTH];
	int len = 0;

	char c;
	while (read(m_FD, &c, 1) == 1) {
		if (c != 0x0d && c != 0x0a) {
			/* append char */
			sentence[len++] = c;

			/* overflow */
			if (len+1 >= MAX_SENTENCE_LENGTH)
				len = 0;	/* or return 0 */
		} else {
			/* newline found, process sentence */
			sentence[len] = '\0';
			if (ParseSentence(sentence, target))
				return 1;
			len = 0;
		}
	}

	/* I'm not sure of what may cause read above to not return 1,
	 * so sleep for safety */
	sleep(1);

	return 0;
}

int GPSSourceNMEA::ParseGPRMC(char **fields, int nfields, GPSData *target) {
	/* need at least 6 fields */
	if (nfields < 6)
		return 0;

	/* drop reports marked as invalid */
	if (fields[1][0] == 'V')
		return 0;

	unsigned int i;
	double lat, lon, d;

	/* latitude */
	lat = (double)((fields[2][0]-'0')*10 + (fields[2][1]-'0'))*60.0;
	lat += (double)((fields[2][2]-'0')*10 + (fields[2][3]-'0'));

	for (i = 5, d = 0.1; i < strlen(fields[2]); i++, d /= 10.0)
		lat += (fields[2][i]-'0') * d;

	lat /= 360.0*60.0;

	if (fields[3][0] == 'S')
		lat *= -1.0;

	/* longitude */
	lon = (double)((fields[4][0]-'0')*100 + (fields[4][1]-'0')*10 + (fields[4][2]-'0'))*60.0;
	lon += (double)((fields[4][3]-'0')*10 + (fields[4][4]-'0'));

	for (i = 6, d = 0.1; i < strlen(fields[4]); i++, d /= 10.0)
		lon += (fields[4][i]-'0') * d;

	lon /= 360.0*60.0;

	if (fields[5][0] == 'W')
		lon *= -1.0;

	/* check */
	if (lat < -0.25 || lat > 0.25 || lon < -0.5 || lon > 0.5)
		return 0;

	target->longitude = lon;
	target->latitude = lat;
	return 1;
}

int GPSSourceNMEA::ParseSentence(char *sentence, GPSData *target) {
	/* get rid of $ */
	if (*sentence != '\0')
		sentence++;

	/* get provided checksum */
	char *checksum = strrchr(sentence, '*');
	if (!checksum)
		return 0;
	*checksum++ = '\0';

	/* test checksum */
	if (!TestChecksum(sentence, checksum))
		return 0;

	/* split to fields */
	int nfields = 0;
	char *fields[MAX_SENTENCE_FIELDS];

	while (nfields < MAX_SENTENCE_FIELDS) {
		char *comma = strchr(sentence, ',');
		if (!comma)
			break;
		*comma = 0;
		fields[nfields++] = sentence;
		sentence = comma+1;
	}

	/* need at least 1 field */
	if (nfields == 0)
		return 0;

	/* parse variuos sentences */
	if (strcmp(fields[0], "GPRMC") == 0)
		return ParseGPRMC(fields+1, nfields-1, target);

	return 0;
}

int GPSSourceNMEA::TestChecksum(char *sentence, char *checksum) {
	unsigned char sum = 0;
	char c;

	/* provided checksum should be 2 chars length */
	if (checksum[0] == '\0' || checksum[1] == '\0')
		return 0;

	/* caculate our own checksum */
	while ((c = *sentence++) != '\0')
		sum ^= c;

	/* convert it to text */
	char mysum[3];
	snprintf(mysum, sizeof(mysum), "%02X", sum);

	/* compare */
	return (toupper(checksum[0]) == toupper(mysum[0]) && toupper(checksum[1]) == toupper(mysum[1]));
}

} /* namespace gaia */
