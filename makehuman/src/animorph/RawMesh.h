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
 *  File: RawMesh.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef RAWMESH_H
#define RAWMESH_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "FaceGroup.h"
#include "VertexVector.h"

namespace Animorph
{

/*! \brief A Mesh that is not poseable or morphable anymore
 */
class RawMesh
{
private:
	FaceVector   facevector;
	VertexVector vertexvector;
	FaceGroup    bodyfaces;
	// FaceGroup         facegroup;

	string name;

public:
	RawMesh();
	~RawMesh();

public:
	/*!
	 * \return a reference to the morphed VertexVector of this Mesh
	 */
	VertexVector & getVertexVectorRef() { return vertexvector; }

	/*!
	 * \return a reference to the FaceVector of this Mesh
	 */
	FaceVector & getFaceVectorRef() { return facevector; }

	/*!
	 * \return a reference to the BodyFaces of the Mesh
	 */
	FaceGroup & getBodyFacesRef() { return bodyfaces; }

	/**** set Reference API ****/
	/***************************/

	void setVertexVector(const VertexVector & inVertexVector) { vertexvector = inVertexVector; }

	void setFaceVector(const FaceVector & inFaceVector) { facevector = inFaceVector; }

	void setBodyFaces(const FaceGroup & inBodyFaces) { bodyfaces = inBodyFaces; }

	void           setName(const string & inName) { name = inName; }
	const string & getName() { return name; }
};

} // namespace Animorph

#endif // RAWMESH_H
