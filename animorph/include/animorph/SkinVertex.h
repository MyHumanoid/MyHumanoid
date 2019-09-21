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
 *  File: Color.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef SKINVERTEX_H
#define SKINVERTEX_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <vector>

#include "Vector3.h"

using std::set;
using std::vector;

namespace Animorph
{

/*! \brief Represents skin properties of a vertex.

The vertex is identified by its index.

 */
class SkinVertex
{
private:
	int skinVertex;
	vector<int> linkedMuscles;
	set<int> linkedMusclesSet;
	float originalDist;
	// Vector3f oldCenteroid;

public:
	SkinVertex(int _skinVertex, float _originalDist, const vector<int> &tokens);

	vector<int> &getLinkedMuscles() { return linkedMuscles; }
	set<int> &getLinkedMusclesSet() { return linkedMusclesSet; }
	float getOriginalDist() { return originalDist; }
	int getSkinVertex() { return skinVertex; }

	void setOriginalDist(const float inOriginalDist)
	{
		originalDist = inOriginalDist;
	}
	// Vector3f &getOldCenteroid () {return oldCenteroid;}
	// void setOldCenteroid (const Vector3f &inCenteroid) {oldCenteroid =
	// inCenteroid;}
};

} // namespace Animorph

#endif // SKINVERTEX_H
