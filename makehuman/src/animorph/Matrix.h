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
 *  File: Matrix.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef MATRIX_H
#define MATRIX_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Euler.h"
#include <glm/vec3.hpp>
#include "util.h"
#include <assert.h> // define NDEBUG for non debug modus!!
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace Animorph
{

/*! \brief Represents a 4x4 matrix

 \verbatim
  This is the format of the matrix data: (OpenGL notation)

   ******************
   *  0  4  8   12  *
   *  1  5  9   13  *
   *  2  6  10  14  *
   *  3  7  11  15  *
   ******************

   *******************
   * Xx  Yx  Zx  Tx  *
   * Xy  Yy  Zy  Ty  *
   * Xz  Yz  Zz  Tz  *
   * 0   0   0   1   *
   *******************
     ^   ^   ^   ^--- Translation Vector
     |   |   |------- Z Axis Vector
     |   |----------- Y Axis Vector
     |--------------- X Axis Vector

  -> Column-major matrix ordering
  \endverbatim
*/

class Matrix
{
public:
	/// the Matrix is saved in this array
	float data[16];

	/// construct Matrix with identity
	Matrix() { identity(); }

	/// set Matrix identity
	inline void identity()
	{
		// set values for identity matrix
		data[0] = 1.0;
		data[4] = 0.0;
		data[8] = 0.0;
		data[12] = 0.0;
		data[1] = 0.0;
		data[5] = 1.0;
		data[9] = 0.0;
		data[13] = 0.0;
		data[2] = 0.0;
		data[6] = 0.0;
		data[10] = 1.0;
		data[14] = 0.0;
		data[3] = 0.0;
		data[7] = 0.0;
		data[11] = 0.0;
		data[15] = 1.0;
	}

	// get Euler representation of matrix rotation
	void fromEuler(Euler &e);

	/*!
	 * \param theta rotation around an axis in PI
	 * \param axis the axis to rotate about
	 */
	void setRotation(float theta, RotateAxis axis);

	void setRotation(float theta, const glm::vec3 &axis);

	/*!
	 * \param s a Vector3 to scale the matrix
	 */
	void setScale(const glm::vec3 &s);

	/*!
	 * \param x the x value of the scale vector
	 * \param y the y value of the scale vector
	 * \param z the z value of the scale vector
	 */
	void setScale(float x, float y, float z);

	/*!
	 * \param t a Vector3 to translate the matrix
	 */
	void setTranslation(const glm::vec3 &t);

	/*!
	 * \param x the x value of the translation vector
	 * \param y the y value of the translation vector
	 * \param z the z value of the translation vector
	 */
	void setTranslation(float x, float y, float z);

	/*!
	 * \return get the translation part of the matrix
	 */
	glm::vec3 getTranslation();
};

/// Notation for matrix multiplication is from left to right.
/// Use [M] = [T] * [M] to transform a matrix.
Matrix operator*(const Matrix &a, const Matrix &b);

/// multiply vector with Matrix
glm::vec3 operator*(const glm::vec3 &v, const Matrix &m);

/// write the Matrix elements into a stream
std::ostream &operator<<(std::ostream &s, const Matrix &m);

} // namespace Animorph

#endif // MATRIX_H
