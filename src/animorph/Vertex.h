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
 *  File: Vertex.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Color.h"
#include <glm/vec3.hpp>
#include <cmath>
#include <string>
#include <vector>

namespace Animorph
{

/*! \brief Represents a vertex by its coordinates and a normal vector.
 */
class Vertex
{
public:
	/// vertex coordinate
	glm::vec3 pos;

	Vertex()
	        : pos(0.f)
	{
	}

	Vertex(float x, float y, float z)
	        : pos(x, y, z)
	{
	}
};

class VertexMeta {
protected:
	std::vector<int> shared_faces_vector;
public:
	/*!
	 * \param shared_face add the index number to which face this Vertex belongs
	 */
	void addSharedFace(int shared_face);
	
	/*!
	 * \return get a vector with all faces this Vertex belongs to
	 */
	const std::vector<int> & getSharedFaces() const;
};

} // namespace Animorph
