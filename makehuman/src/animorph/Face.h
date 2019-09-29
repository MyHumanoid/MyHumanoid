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
 *  File: Face.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef FACE_H
#define FACE_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Color.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Vertex.h"
#include <assert.h>
#include <list>
#include <vector>

namespace Animorph
{

/*! \brief Represents a quad or triangle and its properties
 */
class Face
{
private:
	int vertices[4];
	int size;
	int material_index;

public:
	/// the normal vector of this face
	glm::vec3 no;

	/// construct a quad
	Face(int v0, int v1, int v2, int v3);

	/// construct a triangle
	Face(int v0, int v1, int v2);

	/*!
	 * \return index of the material in MaterialVector
	 */
	int getMaterialIndex() const { return material_index; }

	/*!
	 * \param material_index index of the material in MaterialVector
	 */
	void setMaterialIndex(int material_index) { this->material_index = material_index; }

	/*!
	 * \return a vector with all vertex numbers for this face
	 */
	size_t getSize() const { return size; }

	size_t getVertexAtIndex(int inIndex) const
	{
		assert(inIndex < size);
		return vertices[inIndex];
	}
};

} // namespace Animorph

#endif // FACE_H
