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
 *  File: PoseTarget.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POSETARGET_H
#define POSETARGET_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "DirectoryList.h"
#include "FileReader.h"
#include "PoseRotation.h"
#include "PoseTranslation.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace Animorph
{

typedef std::set<int>              UsedVertex;
typedef std::list<PoseTranslation> PoseTranslationVector;
typedef std::list<PoseRotation>    PoseRotationVector;

/*! \brief Represents the final position and deformation of the body parts
 * attached to a joint
 *
 * The base mesh is linearly transformed to a state inbetween the base position
 * and the final position by applying a set of translations and rotations.
 * This calculation is done by the function Mesh::doPose(const string&
 * target_name, float morph_value, const UsedVertex &modVertex)
 *
 * Can be loaded from several files.
 */
class PoseTarget
{
	// normally these vectors contain only one element
	/// Translations to be applied on Mesh::doPose()
	PoseTranslationVector positiveTranslations;
	/// Translations to be applied on Mesh::doPose() with morph_value < 0
	PoseTranslationVector negativeTranslations;
	/// Rotations to be applied on Mesh::doPose()
	PoseRotationVector positiveRotations;
	/// Rotations to be applied on Mesh::doPose() with morph_value < 0
	PoseRotationVector negativeRotations;

	/// flags whether wehave special transformations for Mesh::doPose() with
	/// morph_value < 0
	bool negative = false;
	bool positive = false;
	/// flag for calcNormalizations()
	bool         normalizationInited = false;
	UsedVertex   modVertex;
	float        minAngle = 0.0f;
	float        maxAngle = 0.0f;

	/// Initializes the center of all rotations with the controid of their
	/// centerVertexNumbers
	void calcRotationsCenteroids(const VertexVector & vertexvector,
	                             PoseRotationVector & rotations);
	void calcTranslationsFormFactors(const VertexVector &    vertexvector,
	                                 PoseTranslationVector & translations);

public:
	/// Initializes the center of all rotations with the controid of their
	/// centerVertexNumbers
	void            calcRotationsCenteroids(const VertexVector & vertexvector);
	void            calcTranslationsFormFactors(const VertexVector & vertexvector);
	const glm::vec3 getFirstRotationCenteroid();

	/// Determines, whether normalizations of the rotations or translations are
	/// required
	void                    calcNormalizations();
	bool                    hasNegative() const { return negative; }
	bool                    hasPositive() const { return positive; }
	PoseTranslationVector & getPositiveTranslations() { return positiveTranslations; }
	PoseTranslationVector & getNegativeTranslations() { return negativeTranslations; }
	PoseRotationVector &    getPositiveRotations() { return positiveRotations; }
	PoseRotationVector &    getNegativeRotations() { return negativeRotations; }

	UsedVertex & getModVertex() { return modVertex; }
	float        getMinAngle() const { return minAngle; }
	float        getMaxAngle() const { return maxAngle; }

	bool load(const std::string &fullPath);
};

} // namespace Animorph

#endif // POSETARGET_H
