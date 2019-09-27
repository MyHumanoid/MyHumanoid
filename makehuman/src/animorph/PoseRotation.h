/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
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
 *  File: PoseRotation.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POSEROTATION_H
#define POSEROTATION_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

#include "FileReader.h"
#include "Matrix.h"
#include "util.h"

using std::set;
using std::string;
using std::vector;

namespace Animorph
{

struct PoseTargetData {
	int vertex_number;
	float rotation;
};

typedef vector<int> UnsortedUsedVertex;

/*! \brief Represents the rotational data of a PoseTarget

This class can be loaded from files.

The format of PoseRotation file:
\verbatim
<int>,<float>
...
\endverbatim

Each line is read into a PoseTargetData struct and consists of a vertex_number
and a rotation angle in radians.

The filenames of these files end in ".rot".

The format of PoseRotation info file:
\verbatim
<int>,<int>,<int>,...
[X,Y,Z]
<float>,<float>
\endverbatim

The first line contains the centerVertexNumbers.
The second line indicates the axis.
The third line indicates minAngle and maxAngle.
Further lines are ignored.

The filenames of these files end in ".rot.info".

*/
class PoseRotation : public vector<PoseTargetData>
{
private:
	/// Used to calculate center
	vector<int> centerVertexNumbers;
	/// Will be used as parameter for Matrix::setRoation()
	RotateAxis axis;
	/// Set of all indices in the ".rot" file
	UnsortedUsedVertex modVertex;
	/// Currently unused
	bool hasCenter;
	/// Minimum angle in radians
	float minAngle;
	/// Maximum angle in radians
	float maxAngle;
	/// will be set to true by PoseTarget::calcNormalizations() if our
	/// (min|max)Angle != PoseTarget::(min|max)Angle
	bool normalize;
	// string inFilename;
	/* \brief "Category"? Currently always "00"
	 * First two characters of the filename, eg . "00" for "00_Z_LIMB_SPINE1.rot"
	 */
	string cat;

	/* \brief Centroid of centerVertexNumbers
	 * Is initialized by PoseTarget::calcRotationsCenteroids()
	 */
	glm::vec3 center;
	/// Limb rotation Type
	bool mbLimb;

public:
	PoseRotation();
	/*!
	 * \param filename the file with PoseTarget data to load
	 * \return true if file is found
	 * \return false if file isn't found
	 */
	bool load(const string &filename);

	const vector<int> &getCenterVertexNumbers() { return centerVertexNumbers; }
	UnsortedUsedVertex &getModVertex() { return modVertex; }
	const glm::vec3 &getCenter() const { return center; }
	void setCenter(const glm::vec3 &c) { center = c; }

	const RotateAxis &getAxis() const { return axis; }
	const bool getHasCenter() const { return hasCenter; }
	void setHasCenter(bool c) { hasCenter = c; }

	const float getMinAngle() const { return minAngle; }
	const float getMaxAngle() const { return maxAngle; }
	const bool getNormalize() const { return normalize; }
	void setNormalize(bool inNormalize) { normalize = inNormalize; }

	// const string &getFilename() const {return inFilename;}
	const string &getCat() const { return cat; }
	void setCat(string inCat) { cat = inCat; }

	void setLimb(bool limb) { mbLimb = limb; }
	bool getLimb() { return mbLimb; }
};

} // namespace Animorph

#endif // POSEROTATION_H
