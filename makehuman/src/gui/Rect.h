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
 *  File   : Rect.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef RECT_H
#define RECT_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Point.h"
#include "Size.h"
#include <iostream>
#include <stdint.h>

namespace mhgui
{

/*!
 * \brief Represents a rectangle by its position and size
 */
class Rect
{
public:
	Rect(int32_t inX, int32_t inY, int32_t inWidth, int32_t inHeight);
	/// Copy constructor
	Rect(const Rect & inRect);

	~Rect();

	/// Assigns another Rect to this one
	Rect & operator=(const Rect & inPoint);

	const Point & getPos() const { return pos; }
	const Size &  getSize() const { return size; }

	int32_t getX() const { return pos.getX(); }
	int32_t getY() const { return pos.getY(); }

	int32_t getWidth() const { return size.getWidth(); }
	int32_t getHeight() const { return size.getHeight(); }

	bool isEmpty() const { return size.isEmpty(); }

	/// Checks whether two rectangles are identical
	bool operator==(const Rect & inRect) const;

	/// Sets the position
	void moveTo(const Point & inPos);
	/** \brief Moves the rectangle
	 *
	 * Wrapper for Point::moveBy().
	 */
	void moveBy(const Point & inDeltaPos);

	/// Sets the size
	void resizeTo(const Size & inSize);
	/** \brief Changes the size
	 *
	 * Wrapper for Size::resizeBy().
	 */
	void resizeBy(const Size & inDeltaSize);

	/** \brief Makes this rectangle smaller by removing an outer border
	 *
	 * @param inInsetX vertical border size
	 * @param inInsetY horizontal border size
	 */
	void inset(int32_t inInsetX, int32_t inInsetY);

	/// Checks whether a particular Point is located in this rectangle
	bool isHitBy(const Point & inPoint) const;

private:
	Point pos;
	Size  size;

}; // class Rect

inline Rect::Rect(int32_t inX, int32_t inY, int32_t inWidth, int32_t inHeight)
        : pos(inX, inY)
        , size(inWidth, inHeight)
{
}

inline Rect::Rect(const Rect & inRect)
        : pos(inRect.pos)
        , size(inRect.size)
{
}

inline Rect::~Rect() {}

inline Rect & Rect::operator=(const Rect & inPoint)
{
	pos  = inPoint.pos;
	size = inPoint.size;
	return *this;
}

inline bool Rect::operator==(const Rect & inRect) const
{
	if(this == &inRect) // The same object?
		return true;

	return (pos == inRect.pos && size == inRect.size);
}

inline void Rect::moveTo(const Point & inPos) { pos = inPos; }

inline void Rect::moveBy(const Point & inDeltaPos) { pos.moveBy(inDeltaPos); }

inline void Rect::resizeTo(const Size & inSize) { size = inSize; }

inline void Rect::resizeBy(const Size & inDeltaSize) { size.resizeBy(inDeltaSize); }

inline void Rect::inset(int32_t inInsetX, int32_t inInsetY)
{
	moveBy(Point(inInsetX, inInsetY));
	resizeBy(Size(-2 * inInsetX, -2 * inInsetY));
}

inline bool Rect::isHitBy(const Point & inPoint) const
{
	return (((inPoint.getX() >= getX()) && (inPoint.getY() >= getY())) &&
	        ((inPoint.getX() < getX() + getWidth()) &&
	         (inPoint.getY() < getY() + getHeight())));
}

} // namespace mhgui

#endif // RECT_H
