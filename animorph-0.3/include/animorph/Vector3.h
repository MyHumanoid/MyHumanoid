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
 *  File: Vector3.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef VECTOR3_H
#define VECTOR3_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include <cassert>

namespace Animorph {

template <typename T>
class Vector3
{
private:

public:
  T x, y, z;

  Vector3 () {}

  /// copy constructor
  Vector3 (const Vector3<T> &v) : x (v.x), y (v.y), z (v.z) {}

  /// construct given three values
  /*!
   * \param _x the x component for the Vector3
   * \param _y the y component for the Vector3
   * \param _z the z component for the Vector3
   */
  Vector3 (const T& _x, const T& _y, const T& _z)
    : x (_x), y (_y), z (_z) {}

  virtual ~Vector3 () {}

  /// set all components to zero
  void zero () {x = y = z = 0;}

  /// Vector dot product
  T operator * (const Vector3<T> &v);

  /// copy values
  Vector3<T>& operator = (const Vector3<T>& v1);

  /*! \return inverted vector */
  Vector3<T> operator - ();

  /// vector_2 += vector_1
  Vector3<T>& operator += (const Vector3<T>& v1);

  /// vector_2 -= vector_1
  Vector3<T>& operator -= (const Vector3<T>& v1);

  /// vector_1 *= scalar
  Vector3<T>& operator *= (const T& scalar);

  const T* getAsOpenGLVector() const {return &x;}
};

template <typename T>
class Vector3decimal : public Vector3<T>
{
public:
  Vector3decimal ()
    : Vector3<T>  () {}
  Vector3decimal (const T& _x, const T& _y, const T& _z)
    : Vector3<T>  (_x, _y, _z) {}
  Vector3decimal (const Vector3<T> &v) : Vector3<T>  (v) {}

  virtual ~Vector3decimal() {}

  // no special functions for decimal vectors
};

template <typename T>
class Vector3real : public Vector3<T>
{
public:
  Vector3real ()                    : Vector3<T>  () {}
  Vector3real (const T& _x, const T& _y, const T& _z)
    : Vector3<T>  (_x, _y, _z) {}
  Vector3real (const Vector3<T> &v) : Vector3<T>  (v) {}

  virtual ~Vector3real() {}

  /// normalize it
  void normalize ();

  /// return the magnitude
  virtual T getMagnitude () const = 0;

    /// vector_1 /= scalar
  Vector3real<T>& operator /= (const T& scalar);
};

class Vector3f : public Vector3real<float>
{
public:
  Vector3f ()                             : Vector3real<float> () {}
  Vector3f (float _x, float _y, float _z) : Vector3real<float> (_x, _y, _z) {}
  Vector3f (const Vector3<float> &v)      : Vector3real<float> (v) {}

  virtual ~Vector3f() {}

  float getMagnitude () const
  {
    return sqrtf (x * x + y * y + z * z);
  }
};

class Vector3d : public Vector3real<double>
{
public:
  Vector3d ()                                : Vector3real<double> () {}
  Vector3d (const double& _x, const double& _y, const double& _z)
    : Vector3real<double> (_x, _y, _z) {}
  Vector3d (const Vector3<double> &v)        : Vector3real<double> (v) {}

  virtual ~Vector3d() {}

  double getMagnitude () const
  {
    return sqrt (x * x + y * y + z * z);
  }
};

typedef Vector3 <int> Vector3i;

/*****************************/
/* Member Template functions */
/*****************************/

template <typename T>
void Vector3real<T>::normalize ()
{
  T magnitude = getMagnitude ();

  if (magnitude > 0.0)
  {
    T one_mag = 1.0 / magnitude;
    *this *= one_mag;
  }
}

template <typename T>
T Vector3<T>::operator * (const Vector3<T>& v)
{
  return x * v.x + y * v.y + z * v.z;
}

/********************/
/* Member-Operators */
/********************/

template <typename T>
Vector3<T>& Vector3<T>::operator = (const Vector3<T>& v1)
{
  x = v1.x;
  y = v1.y;
  z = v1.z;

  return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator - ()
{
  return Vector3<T>(-x,-y,-z);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator += (const Vector3<T>& v1)
{
  x += v1.x;
  y += v1.y;
  z += v1.z;

  return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator -= (const Vector3<T>& v1)
{
  x -= v1.x;
  y -= v1.y;
  z -= v1.z;

  return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator *= (const T& scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}

template <typename T>
inline Vector3real<T>& Vector3real<T>::operator /= (const T& scalar)
{
  assert(scalar != 0); // Check against Division by zero

  Vector3<T>::x /= scalar;
  Vector3<T>::y /= scalar;
  Vector3<T>::z /= scalar;

  return *this;
}

/**************************************/
/* Non-Member operators and functions */
/**************************************/

/// test two Vector3 if they have the same values
template <typename T>
inline bool operator == (const Vector3<T>& v1, const Vector3<T>& v2)
{
  return ((&v1  == &v2) || // Object identity ?
         ((v1.x == v2.x) &&
          (v1.y == v2.y) &&
          (v1.z == v2.z)));
}

/// vector_2 = vector_1 * scalar
template <typename T>
inline Vector3<T> operator * (const Vector3<T>& v1, const T scalar)
{
  Vector3<T>v3(v1);
  return v3 *= scalar;
}

/// vector_2 = vector_1 / scalar
template <typename T>
inline Vector3real<T> operator / (const Vector3<T>& v1, const T scalar)
{
  assert(scalar != 0); // Check against Division by zero

  Vector3<T>v3(v1);
  return v3 /= scalar;
}

/// vector_3 = vector_1 - vector_2
template <typename T>
inline Vector3<T> operator - (const Vector3<T>& v1, const Vector3<T>& v2)
{
  Vector3<T>v3(v1);
  return v3 -= v2;
}

/// vector_3 = vector_1 + vector_2
template <typename T>
inline Vector3<T> operator + (const Vector3<T>& v1, const Vector3<T>& v2)
{
  Vector3<T>v3(v1);
  return v3 += v2;
}

/// calculate the cross pruduct of two Vector3
template <typename T>
inline Vector3<T> crossProduct (const Vector3<T> &v1, const Vector3<T> &v2)
{
  return Vector3<T> (
           v1.y * v2.z - v1.z * v2.y,
           v1.z * v2.x - v1.x * v2.z,
           v1.x * v2.y - v1.y * v2.x
         );
}

/// calculate the distance between two Vector3 (float)
//template <typename T>
inline float vectorDistance (const Vector3f &v1, const Vector3f &v2)
{
  return Vector3f(v1 - v2).getMagnitude();
}

/// calculate the distance between two Vector3 (double)
//template <typename T>
inline double vectorDistance (const Vector3d &v1, const Vector3d &v2)
{
  return Vector3d(v1 - v2).getMagnitude();
}


/// << operator for output
template <typename T>
std::ostream &operator << (std::ostream &s, const Vector3<T> &v)
{
  s << "[";
  s << "x: " << v.x << " y: " << v.y << " z: " << v.z;
  s << "]";

  return s;
}

} // end namespace Animorph

#endif	// VECTOR3_H
