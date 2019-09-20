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
 *  File: Quaternion.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef QUATERNION_H
#define QUATERNION_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <cmath>
#include "Euler.h"
#include "Vector3.h"
#include "MathUtil.h"

namespace Animorph {

/*! \brief Represents a quaternion
 */
class Quaternion
{
public:
  float w, x, y, z;

  enum RotateAxis
  {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
  };

  Quaternion () : w (1.0), x (0.0), y (0.0), z (0.0) {}
  Quaternion (float w, float x, float y, float z) : w (w), x (x), y (y), z (z) {};

  void identity () {w = 1.0; x = y = z = 0.0;}
  void conjugate ();

  /// as PI
  void setRotation (float theta, RotateAxis axis);
  void fromEuler (Euler &e);
};

/**************************************/
/* Non-Member operators and functions */
/**************************************/

/// Quaternion multiplication (cross product)
Quaternion operator * (const Quaternion &q1, const Quaternion &q2);

// << operator for output
std::ostream &operator << (std::ostream &s, const Quaternion &q);

}

#endif  // QUATERNION_H
