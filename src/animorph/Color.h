// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright (C) 2005  Andreas Volz
// SPDX-FileCopyrightText: Copyright (C) 2006-2007  MakeHuman Project
#pragma once

#include <glm/ext/vector_float4.hpp>

namespace Animorph
{

/*! \brief Represents a colour

  This class just represents a colour and has some convenient access functions.

  */
struct Color
{
	glm::vec4 m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	
	Color() = default;
	
	// constructor initializes Color with rgb values and alpha = 1.0
	Color(float r, float g, float b, float a = 1.0)
	        : m_color(r, g, b, a)
	{
	}

	Color(const Color & inColor);

	~Color() {}

	Color & operator=(const Color & inRHS);

	/// \param r red component of a color
	void red(float r) { this->m_color.r = r; }

	/// \param g green component of a color
	void green(float g) { this->m_color.g = g; }

	/// \param m_color.b blue component a color
	void blue(float b) { this->m_color.b = b; }

	/// \param a alpha component a color
	void alpha(float a) { this->m_color.a = a; }

	/*!
	 * \param r red component of a color
	 * \param g green component of a color
	 * \param m_color.b blue component a color
	 * \param a alpha component a color
	 */
	void rgba(float r, float g, float b, float a)
	{
		this->m_color.r = r;
		this->m_color.g = g;
		this->m_color.b = b;
		this->m_color.a = a;
	}

	/*!
	 * \param r red component of a color
	 * \param g green component of a color
	 * \param m_color.b blue component a color
	 * <br><br>the alpha component is set to 1.0
	 */
	void rgb(float r, float g, float b)
	{
		this->m_color.r = r;
		this->m_color.g = g;
		this->m_color.b = b;
		this->m_color.a = 1.0;
	}

	/// \return red component of a color
	float red(void) const { return m_color.r; }

	/// \return green component of a color
	float green(void) const { return m_color.g; }

	/// \return blue component of a color
	float blue(void) const { return m_color.b; }

	/// \return alpha component of a color
	float alpha(void) const { return m_color.a; }

	const float * getAsOpenGLVector() const { return &m_color.r; }
};

} // namespace Animorph
