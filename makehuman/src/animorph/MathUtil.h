/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005  Andreas Volz
 *  Copyright (C) 2006-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File: MathUtil.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef MATHUTIL_H
#define MATHUTIL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

namespace Animorph
{

/// "Wrap" an angle in range -pi...pi by adding the correct multiple of 2 pi
float wrapPi(float theta);

/// convert angle from radians to degree
inline double rad2deg(double x)
{
	return x * 180.0 / M_PI;
}

/// convert angle from degree to radians
inline double deg2rad(double x)
{
	return x * M_PI / 180.0;
}

/***********************
 * some math templates *
 ***********************/

/// return true if 'value' between 'range1' and 'range2' - including bounds!
template <typename T> bool between(T value, T range1, T range2)
{
	if(((value >= range1) && (value <= range2)) || ((value >= range2) && (value <= range1))) {
		return true;
	}
	return false;
}

/// fuzzy a value (very primitive implementation!)
template <typename T, typename T2> T2 pseudoFuzzy(T total, T part, T2 fuzzyable)
{
	T2 ratio;
	T  null_value(0);

	if(part != null_value) {
		ratio = total / part;
		return fuzzyable / ratio;
	} else {
		return 0;
	}
}

/// return true if 'unexact' in 'exact' +- 'tolerance'
template <typename T> bool inTolerance(T exact, T unexact, T tolerance)
{
	return between(unexact, exact - tolerance, exact + tolerance);
}

/// compute the absolute value of a value
template <typename T> T abs(T value)
{
	T null_value(0);

	if(value < null_value)
		value = -value;

	return value;
}

/// return the distance of two values
template <typename T> T dist(T value1, T value2)
{
	T d = value1 - value2;
	T null_value(0);

	return abs<T>(d);
}

/// return the value exact between both values (something like arithmetic middle
/// (a+b)/2)
template <typename T> T middle(T value1, T value2)
{
	if(value1 < value2) {
		return value1 + (dist(value1, value2) * 0.5);
	} else if(value1 > value2) {
		return value1 - (dist(value1, value2) * 0.5);
	}

	// both have same value, return one of them
	return value1;
}

/// Clap value to bounds from limit_val (from both sides!)
template <typename T> T limit(T value, T limit_val)
{
	if(value > limit_val)
		value = limit_val;
	else if(value < -limit_val)
		value = -limit_val;

	return value;
}

/// test three values about its monotony attributes
/*!
 * \param value0 first value
 * \param value1 second value
 * \param value2 third value
 * \return 0 if strictly monotonic increasing
 * \return 1 if strictly monotonic decreasing
 * \return -1 if not monotonic
 */
template <typename T> int hasMonotonie(T value0, T value1, T value2)
{
	// strictly monotonic increasing
	if((value0 > value1) && (value1 > value2))
		return 0;

	// strictly monotonic decreasing
	if((value0 < value1) && (value1 < value2))
		return 1;

	return -1;
}

} // namespace Animorph

#endif // MATHUTIL_H
