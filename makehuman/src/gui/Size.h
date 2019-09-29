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

#ifndef SIZE_H
#define SIZE_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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

	bool isEmpty() const { return ((mWidth == 0) && (mHeight == 0)); }

	int32_t getWidth() const { return mWidth; }
	int32_t getHeight() const { return mHeight; }

	void resizeBy(const Size & inSize);

private:
	int32_t mWidth, mHeight;

}; // class Size

inline Size::Size(int32_t inWidth, int32_t inHeight)
        : mWidth(inWidth)
        , mHeight(inHeight)
{
}

inline Size::Size(const Size & inSize)
        : mWidth(inSize.mWidth)
        , mHeight(inSize.mHeight)
{
}

inline Size & Size::operator=(const Size & inRHS)
{
	mWidth  = inRHS.mWidth;
	mHeight = inRHS.mHeight;
	return *this;
}

inline bool Size::operator==(const Size & inSize) const
{
	if(this == &inSize) // The same object?
		return true;

	return (mWidth == inSize.mWidth && mHeight == inSize.mHeight);
}

inline void Size::resizeBy(const Size & inSize)
{
	mWidth += inSize.mWidth;
	mHeight += inSize.mHeight;
}

} // namespace mhgui

#endif // SIZE_H
