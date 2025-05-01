/* SPDX-License-Identifier: GPL-3.0-or-later */
// SPDX-FileCopyrightText: Copyright (C) 2005-2007  MakeHuman Project
#pragma once

#include "Target.h"
#include "VertexVector.h"

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
	Target *  target;
	glm::vec3 originalSize;
	glm::vec3 formFactor;
	float     minAngle;
	float     maxAngle;
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
	bool load(const string & filename);

	UnsortedUsedVertex & getModVertex() { return target->getModVertex(); }

	/// The distance between startVertexNumbers and endVertexNumbers, divided by
	/// originalSize
	void      calcFormFactor(const VertexVector & vertexvector);
	Target &  getTarget() const { return *target; }
	glm::vec3 getFormFactor() const { return formFactor; }
	float     getMinAngle() const { return minAngle; }
	float     getMaxAngle() const { return maxAngle; }
	bool      getNormalize() const { return normalize; }
	void      setNormalize(bool inNormalize) { normalize = inNormalize; }
	// const string &getFilename() const {return inFilename;}
	const string & getCat() const { return cat; }
	void           setCat(string inCat) { cat = inCat; }

	void setLimb(bool limb) { mbLimb = limb; }
	bool getLimb() { return mbLimb; }
};

} // namespace Animorph
