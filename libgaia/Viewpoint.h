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

#ifndef __LIBGAIA__VIEWPOINT_H__
#define __LIBGAIA__VIEWPOINT_H__

#include <math.h>

#define DEFAULT_TRANSLATION_TIME	1.0

namespace gaia {

/**
 * Coordinates of virtual `eye'.
 */
class Eye {
public:
	/**
	 * Default constructor.
	 * Eye is located in 0 long, 0 lat, fov is 90 degrees
	 */
	Eye() {
		x=	0.0;
		y=	0.0;
		h=	1.0;
		orient=	0.0;
		pitch=	0.0;
		fov=	M_PI_2;
	};

	/**
	 * Constructor with given FOV.
	 */
	Eye(double nh, double nfov) {
		x=	0.0;
		y=	0.0;
		h=	nh;
		orient=	0.0;
		pitch=	0.0;
		fov=	nfov;
	};

	/**
	 * Constructor with all coordinates given.
	 */
	Eye(double nx, double ny, double nh, double norient, double npitch, double nfov) {
		x = nx;
		y = ny;
		h = nh;
		orient = norient;
		pitch = npitch;
		fov = nfov;
	};

	/**
	 * Addition operator.
	 * There's no sense in adding two eyes, but this method is used in interpolation.
	 */
	inline Eye operator + (const Eye &e) const {
		return Eye(x+e.x, y+e.y, h+e.h, orient+e.orient, pitch+e.pitch, fov+e.fov);
	};

	/**
	 * Multiplication by number operator.
	 * There's no sense in multiplying eye on a number, but this is used in interpolation.
	 */
	inline Eye operator * (const double k) const {
		return Eye(x*k, y*k, h*k, orient*k, pitch*k, fov*k);
	};

	/**
	 * Calculates span of earth surface viewed (x axis).
	 */
	double xspan(double aspect) {
		return aspect * 2.0 * tan(fov/2.0) * h / sqrt(1.0 + aspect * aspect);
	};

	/**
	 * Calculates span of earth surface viewed (y axis).
	 */
	double yspan(double aspect) {
		return 2.0 * tan(fov/2.0) * h / sqrt(1.0 + aspect * aspect);
	};

	double span() {
		return 2.0 * tan(fov/2.0) * h;
	};

	double xfov(double aspect) {
		return atan2(xspan(aspect) / 2.0, h) * 2.0;
	};

	double yfov(double aspect) {
		return atan2(yspan(aspect) / 2.0, h) * 2.0;
	};

public:
	double x;	///< longitude [-0.5..0.5]
	double y;	///< latitude [-0.25..0.25]
	double h;	///< height above the surface - in same units as x and y
	double orient;	///< orientation [0..2*PI] where 0 is north
	double pitch;	///< pitch [0..PI/2] where 0 is for straight top-down view
	double fov;	///< field-of-view angle
};

/**
 * View point representation.
 * Class representing observer - contains current position and animation
 * parameters
 *
 * @deprecated no longer used anywhere - EarthView's fully control `eye' movements on their own
 */
class Viewpoint {
public:
	Viewpoint();
	Viewpoint(Eye &eye);
	virtual ~Viewpoint();

	void SetCurrentCoordinates(Eye &eye);
	void SetTargetCoordinates(Eye &eye);
	void Animate(double delta);

	Eye GetEye();

protected:
	double TranslationFunction(double percent);

protected:
	Eye m_CurrentEye;
	Eye m_SourceEye;
	Eye m_TargetEye;
	double m_Translation;
	double m_TranslationTime;
};

} /* namespace gaia */

#endif
