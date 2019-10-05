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
 *  File   : Size.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include <stdint.h>

namespace mhgui
{

/// Represents a size of width and height
class Size
{
public:
	Size(int32_t inWidth, int32_t inHeight);
	Size(const Size & inSize);

	~Size() {}

	Size & operator=(const Size & inRHS);

	bool operator==(const Size & inSize) const;

	bool isEmpty() const { return ((x == 0) && (y == 0)); }

	void resizeBy(const Size & inSize);

	int32_t x, y;

}; // class Size

inline Size::Size(int32_t inWidth, int32_t inHeight)
        : x(inWidth)
        , y(inHeight)
{
}

inline Size::Size(const Size & inSize)
        : x(inSize.x)
        , y(inSize.y)
{
}

inline Size & Size::operator=(const Size & inRHS)
{
	x  = inRHS.x;
	y = inRHS.y;
	return *this;
}

inline bool Size::operator==(const Size & inSize) const
{
	if(this == &inSize) // The same object?
		return true;

	return (x == inSize.x && y == inSize.y);
}

inline void Size::resizeBy(const Size & inSize)
{
	x += inSize.x;
	y += inSize.y;
}

} // namespace mhgui
