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

#ifndef __LIBGAIA__MATH_H__
#define __LIBGAIA__MATH_H__

#include <cmath>
#include <stdexcept>

namespace gaia {

/// Simple 2d vector class
template <typename T>
class Vector2 {
public:
	// ctors
	Vector2<T>() { x = y = 0; }
	Vector2<T>(T ix, T iy) : x(ix), y(iy) {}
	Vector2<T>(const Vector2<T> &i) : x(i.x), y(i.y) {}

	// math operators
	inline Vector2<T> operator + (const Vector2<T> &v) const {
		return Vector2<T>(x+v.x, y+v.y);
	}

	inline Vector2<T> operator - (const Vector2<T> &v) const {
		return Vector2<T>(x-v.x, y-v.y);
	}

	inline Vector2<T> operator - () const {
		return Vector2<T>(-x, -y);
	}

	inline Vector2<T> operator * (const Vector2<T> &v) const {
		return Vector2<T>(x*v.x, y*v.y);
	}

	inline Vector2<T> operator * (T v) const {
		return Vector2<T>(x*v, y*v);
	}

	inline Vector2<T> operator / (const Vector2<T> &v) const {
		return Vector2<T>(x/v.x, y/v.y);
	}

	inline Vector2<T> operator / (T v) const {
		return Vector2<T>(x/v, y/v);
	}
	inline Vector2<T> operator += (const Vector2<T> &v) {
		x += v.x; y += v.y;
		return Vector2<T>(x, y);
	}

	inline Vector2<T> operator -= (const Vector2<T> &v) {
		x -= v.x; y -= v.y;
		return Vector2<T>(x, y);
	}

	inline Vector2<T> operator *= (const Vector2<T> &v) {
		x *= v.x; y *= v.y;
		return Vector2<T>(x, y);
	}

	inline Vector2<T> operator *= (T v) {
		x *= v; y *= v;
		return Vector2<T>(x,y);
	}

	inline Vector2<T> operator /= (const Vector2<T> &v) {
		x /= v.x; y /= v.y;
		return Vector2<T>(x,y);
	}

	inline Vector2<T> operator /= (T v) {
		x /= v; y /= v;
		return Vector2<T>(x,y);
	}
	inline Vector2<T> operator = (const Vector2<T> &v) {
		return Vector2<T>(x = v.x, y = v.y);
	}

	inline bool operator == (const Vector2<T> &v) const {
		if (v.x == x && v.y == y) return true;
		return false;
	}

	inline bool operator != (const Vector2<T> &v) const {
		if (v.x != x || v.y != y) return true;
		return false;
	}

	// comparison
	inline bool operator < (const Vector2<T> &v) const {
		if (x < v.x && y < v.y) return true;
		return false;
	}
	inline bool operator > (const Vector2<T> &v) const {
		if (x > v.x && y > v.y) return true;
		return false;
	}
	inline bool operator <= (const Vector2<T> &v) const {
		if (x <= v.x && y <= v.y) return true;
		return false;
	}
	inline bool operator >= (const Vector2<T> &v) const {
		if (x >= v.x && y >= v.y) return true;
		return false;
	}

	// methods
	inline T length() const {
		return sqrt(x*x + y*y);
	}

public:
	// data
	T	x, y;
};

typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

/// Simple 3d vector class
template <typename T>
class Vector3 {
public:
	// ctors
	Vector3<T>() { x = y = z = 0; }
	Vector3<T>(T ix, T iy, T iz) : x(ix), y(iy), z(iz) {}
	Vector3<T>(const Vector3<T> &i) : x(i.x), y(i.y), z(i.z) {}

	// math operators
	inline Vector3<T> operator + (const Vector3<T> &v) const {
		return Vector3<T>(x+v.x, y+v.y, z+v.z);
	}

	inline Vector3<T> operator - (const Vector3<T> &v) const {
		return Vector3<T>(x-v.x, y-v.y, z-v.z);
	}

	inline Vector3<T> operator - () const {
		return Vector3<T>(-x, -y, -z);
	}

	inline Vector3<T> operator * (const Vector3<T> &v) const {
		return Vector3<T>(x*v.x, y*v.y, z*v.z);
	}

	inline Vector3<T> operator * (T v) const {
		return Vector3<T>(x*v, y*v, z*v);
	}

	inline Vector3<T> operator / (const Vector3<T> &v) const {
		return Vector3<T>(x/v.x, y/v.y, z/v.z);
	}

	inline Vector3<T> operator / (T v) const {
		return Vector3<T>(x/v, y/v, z/v);
	}
	inline Vector3<T> operator += (const Vector3<T> &v) {
		x += v.x; y += v.y; z += v.z;
		return Vector3<T>(x, y, z);
	}

	inline Vector3<T> operator -= (const Vector3<T> &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return Vector3<T>(x, y, z);
	}

	inline Vector3<T> operator *= (const Vector3<T> &v) {
		x *= v.x; y *= v.y; z *= v.z;
		return Vector3<T>(x, y, z);
	}

	inline Vector3<T> operator *= (T v) {
		x *= v; y *= v; z *= v;
		return Vector3<T>(x,y,z);
	}

	inline Vector3<T> operator /= (const Vector3<T> &v) {
		x /= v.x; y /= v.y; z /= v.z;
		return Vector3<T>(x,y,z);
	}

	inline Vector3<T> operator /= (T v) {
		x /= v; y /= v; z /= v;
		return Vector3<T>(x,y,z);
	}
	inline Vector3<T> operator = (const Vector3<T> &v) {
		return Vector3<T>(x = v.x, y = v.y, z = v.z);
	}

	inline bool operator == (const Vector3<T> &v) const {
		if (v.x == x && v.y == y && v.z == z) return true;
		return false;
	}

	inline bool operator != (const Vector3<T> &v) const {
		if (v.x != x || v.y != y || v.z != z) return true;
		return false;
	}

	// methods
	inline T length() const {
		return sqrt(x*x + y*y + z*z);
	}
	inline void normalize() {
		T l = length();
		if (l == 0.0) throw std::runtime_error("Attempt to normalize zero length vector");
		x/=l; y/=l; z/=l;
	}
	inline Vector3<T> normalized() const {
		T l = length();
		if (l == 0.0) throw std::runtime_error("Attempt to normalize zero length vector");
		return Vector3<T>(x/l, y/l, z/l);
	}

public:
	// data
	T	x, y, z;
};

typedef Vector3<double> Vector3d;

//
// min/max template functions for 2 and 3 arguments
//
template <typename T>
inline T min4(T a, T b, T c, T d) {
	if (a <= b && a <= c && a <= d)	return a;
	if (b <= c && b <= d)		return b;
	if (c <= d)			return c;
	return d;
}

template <typename T>
inline T max4(T a, T b, T c, T d) {
	if (a >= b && a >= c && a >= d)	return a;
	if (b >= c && b >= d)		return b;
	if (c >= d)			return c;
	return d;
}

template <typename T>
inline T min3(T a, T b, T c) {
	if (a <= b && a <= c)	return a;
	if (b <= c)		return b;
	return c;
}

template <typename T>
inline T max3(T a, T b, T c) {
	if (a >= b && a >= c)	return a;
	if (b >= c)		return b;
	return c;
}

template <typename T>
inline T min2(T a, T b) {
	if (a <= b)	return a;
	return b;
}

template <typename T>
inline T max2(T a, T b) {
	if (a >= b)	return a;
	return b;
}

//
// clamp
//
template <typename T>
inline T clamp01(T a) {
	if (a < 0.0) return 0.0;
	if (a > 1.0) return 1.0;
	return a;
}

//
// Vector ops
//
template <typename T>
inline int intersect_line_x(Vector3<T> &point1, Vector3<T> &point2, T &x, T *target) {
	if (point1.x == point2.x)
		return 0;

	double t = (x - point1.x)/(point2.x - point1.x);

	if (t < 0.0 || t > 1.0)
		return 0;

	*target = t;
	return 1;
}

template <typename T>
inline int intersect_line_y(Vector3<T> &point1, Vector3<T> &point2, T &y, T *target) {
	if (point1.y == point2.y)
		return 0;

	double t = (y - point1.y)/(point2.y - point1.y);

	if (t < 0.0 || t > 1.0)
		return 0;

	*target = t;
	return 1;
}

} // namespace gaia

#endif
