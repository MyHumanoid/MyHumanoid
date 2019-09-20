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
 *  File: Vector2.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef VECTOR2_H
#define VECTOR2_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace Animorph {

template <typename T>
class Vector2
{
private:

public:
  T x, y;

  Vector2 () {}

  /// copy constructor
  Vector2 (const Vector2<T> &v) : x (v.x), y (v.y) {}

  /// construct given two values
  /*!
   * \param _x the x component to the Vector2
   * \param _y the y component to the Vector2
   */
  Vector2 (const T& _x, const T& _y) : x (_x), y (_y) {}

  virtual ~Vector2() {}

  /// set all components to zero
  void zero () {x = y = 0;}

  /// copy values
  Vector2<T>& operator = (const Vector2<T>& v1);

  /// \return inverted vector
  Vector2<T> operator - ();

  /// vector_2 += vector_1
  Vector2<T>& operator += (const Vector2<T>& v1);

  /// vector_2 -= vector_1
  Vector2<T>& operator -= (const Vector2<T>& v1);

  /// vector_1 *= scalar
  Vector2<T>& operator *= (const T scalar);
};

template <typename T>
class Vector2decimal : public Vector2<T>
{
public:
  Vector2decimal ()                    : Vector2<T>  () {}
  Vector2decimal (T _x, T _y)          : Vector2<T>  (_x, _y) {}
  Vector2decimal (const Vector2<T> &v) : Vector2<T>  (v) {}

  virtual ~Vector2decimal() {}

  // no special functions for decimal vectors
};

template <typename T>
class Vector2real : public Vector2<T>
{
public:
  Vector2real ()                    : Vector2<T>  () {}
  Vector2real (T _x, T _y)          : Vector2<T>  (_x, _y) {}
  Vector2real (const Vector2<T> &v) : Vector2<T>  (v) {}

  virtual ~Vector2real() {}

    /// normalize it
  void normalize ();

  /// return the magnitude
  virtual T getMagnitude () const = 0;

  /// vector_1 /= scalar
  Vector2real<T>& operator /= (const T scalar);
};

class Vector2f : public Vector2real<float>
{
public:
  Vector2f ()                        : Vector2real<float> () {}
  Vector2f (float _x, float _y)      : Vector2real<float> (_x, _y) {}
  Vector2f (const Vector2<float> &v) : Vector2real<float> (v) {}

  virtual ~Vector2f() {}

  virtual float getMagnitude () const
  {
    // usage of sqrtf is faster than sqrt for float!
    return sqrtf (x * x + y * y);
  }
};

class Vector2d : public Vector2real<double>
{
public:
  Vector2d ()                         : Vector2real<double> () {}
  Vector2d (double _x, double _y)     : Vector2real<double> (_x, _y) {}
  Vector2d (const Vector2<double> &v) : Vector2real<double> (v) {}

  virtual ~Vector2d() {}

  virtual double getMagnitude () const
  {
    return sqrt (x * x + y * y);
  }
};

typedef Vector2decimal<int> Vector2i;

/*****************************/
/* Member Template functions */
/*****************************/

template <typename T>
void Vector2real<T>::normalize ()
{
  T magnitude = getMagnitude ();

  if (magnitude > 0.0)
  {
    T one_mag = 1.0 / magnitude;
    *this *= one_mag;
  }
}

/********************/
/* Member-Operators */
/********************/

template <typename T>
Vector2<T>& Vector2<T>::operator = (const Vector2<T>& v1)
{
  x = v1.x;
  y = v1.y;

  return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator - ()
{
  Vector2<T> v;
  v.x = -x;
  v.y = -y;

  return v;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator += (const Vector2<T>& v1)
{
  x += v1.x;
  y += v1.y;

  return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator -= (const Vector2<T>& v1)
{
  x -= v1.x;
  y -= v1.y;

  return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator *= (const T scalar)
{
  x *= scalar;
  y *= scalar;

  return *this;
}

template <typename T>
inline Vector2real<T>& Vector2real<T>::operator /= (const T scalar)
{
  Vector2<T>::x /= scalar;
  Vector2<T>::y /= scalar;

  return *this;
}

/**************************************/
/* Non-Member operators and functions */
/**************************************/

/// test if two Vector2 have the same values
template <typename T>
inline bool operator == (const Vector2<T>& v1, const Vector2<T>& v2)
{
  if ((v1.x == v2.x) &&
      (v1.y == v2.y))
  {
    return true;
  }

  return false;
}

/// vector_2 = vector_1 * scalar
template <typename T>
inline Vector2<T> operator * (const Vector2<T>& v1, const T scalar)
{
  Vector2<T> v3 = v1;

  return v3 *= scalar;
}

/// vector_2 = vector_1 / scalar
template <typename T>
inline Vector2real<T> operator / (const Vector2<T>& v1, const T scalar)
{
  Vector2real<T> v3 = v1;

  return v3 /= scalar;
}

/// vector_3 = vector_1 - vector_2
template <typename T>
inline Vector2<T> operator - (const Vector2<T>& v1, const Vector2<T>& v2)
{
  Vector2<T> v3 = v1;

  return v3 -= v2;
}

/// vector_3 = vector_1 + vector_2
template <typename T>
inline Vector2<T> operator + (const Vector2<T>& v1, const Vector2<T>& v2)
{
  Vector2<T> v3 = v1;

  return v3 += v2;
}

/// calculate the distance between two Vector2 (float)
inline float vectorDistance (const Vector2f &v1, const Vector2f &v2)
{
  Vector2f dist = v1 - v2;

  return dist.getMagnitude ();
}

/// calculate the distance between two Vector2 (double)
inline double vectorDistance (const Vector2d &v1, const Vector2d &v2)
{
  Vector2d dist = v1 - v2;

  return dist.getMagnitude ();
}

/// << operator for output
template <typename T>
std::ostream &operator << (std::ostream &s, const Vector2<T> &v)
{
  s << "[";
  s << "x: " << v.x << " y: " << v.y;
  s << "]";

  return s;
}

} // end namespace Animorph

#endif	// VECTOR2_H
