// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright (C) 2005  Andreas Volz
// SPDX-FileCopyrightText: Copyright (C) 2006-2007  MakeHuman Project

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <assert.h>

namespace Animorph
{

/*! \brief Represents a quad or triangle and its properties
 */
struct Face
{
	int vertices[4];
	int size;
	int m_materialIndex;

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
	int getMaterialIndex() const { return m_materialIndex; }

	/*!
	 * \param material_index index of the material in MaterialVector
	 */
	void setMaterialIndex(int material_index) { this->m_materialIndex = material_index; }

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
