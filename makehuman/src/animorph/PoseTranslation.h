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
 *  File: PoseTranslation.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POSETRANSLATION_H
#define POSETRANSLATION_H 1

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
#include "Target.h"
#include "util.h"

using std::set;
using std::string;
using std::vector;

namespace Animorph
{

/*! \brief Represents the translational data of a PoseTarget

This class is used like a PoseTarget to deform the base mesh before applying
PoseRotations.

This class can be loaded from files.

The format of PoseTranslation file is the same like for Target.

The filenames of these files end in ".target".

The format of PoseTranslation info file:
\verbatim
<int>
<int>
<float>
<float>,<float>
\endverbatim

The first line contains the startVertexNumbers.
The second line contains the endVertexNumbers.
The third line indicates the originalSize.
The fourth line indicates minAngle and maxAngle.

The filenames of these files end in ".target.info".

 */
class PoseTranslation
{
private:
	Target *target;
	float originalSize[3];
	glm::vec3 formFactor;
	float minAngle;
	float maxAngle;
	/// flag
	bool normalize;
	// string inFilename;
	/// Can be used to influence the order of application of rotations and
	/// translations
	string cat;

	bool mbLimb;

public:
	PoseTranslation();
	/*!
	 * \param filename the file with PoseTarget data to load
	 * \return true if file is found
	 * \return false if file isn't found
	 */
	bool load(const string &filename);

	UnsortedUsedVertex &getModVertex() { return target->getModVertex(); }

	/// The distance between startVertexNumbers and endVertexNumbers, divided by
	/// originalSize
	void calcFormFactor(const VertexVector &vertexvector);
	Target &getTarget() { return *target; }
	glm::vec3 &getFormFactor() { return formFactor; }
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

#endif // POSETRANSLATION_H
