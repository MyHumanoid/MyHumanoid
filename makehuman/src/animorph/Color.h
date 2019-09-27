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
 *  File: Color.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef COLOR_H
#define COLOR_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace Animorph
{

/*! \brief Represents a colour

  This class just represents a colour and has some convenient access functions.

  */
class Color
{
private:
	float r, g, b, a;

public:
	/// constructor initalizes Color with 1.0,1.0,1.0
	Color()
	    : r(1.0)
	    , g(1.0)
	    , b(1.0)
	    , a(1.0)
	{
	}

	// constructor initializes Color with rgb values and alpha = 1.0
	Color(float r, float g, float b, float a = 1.0)
	    : r(r)
	    , g(g)
	    , b(b)
	    , a(a)
	{
	}

	Color(const Color &inColor);

	~Color() {}

	Color &operator=(const Color &inRHS);

	/// \param r red component of a color
	void red(float r) { this->r = r; }

	/// \param g green component of a color
	void green(float g) { this->g = g; }

	/// \param b blue component a color
	void blue(float b) { this->b = b; }

	/// \param a alpha component a color
	void alpha(float a) { this->a = a; }

	/*!
	 * \param r red component of a color
	 * \param g green component of a color
	 * \param b blue component a color
	 * \param a alpha component a color
	 */
	void rgba(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	/*!
	 * \param r red component of a color
	 * \param g green component of a color
	 * \param b blue component a color
	 * <br><br>the alpha component is set to 1.0
	 */
	void rgb(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1.0;
	}

	/// \return red component of a color
	float red(void) const { return r; }

	/// \return green component of a color
	float green(void) const { return g; }

	/// \return blue component of a color
	float blue(void) const { return b; }

	/// \return alpha component of a color
	float alpha(void) const { return a; }

	const float *getAsOpenGLVector() const { return &r; }
};

} // namespace Animorph

#endif // COLOR_H
