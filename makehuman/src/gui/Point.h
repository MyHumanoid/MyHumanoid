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
 *  File   : Point.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POINT_H
#define POINT_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <string>

using std::string;

namespace mhgui {

/// Represents a point
class Point
{
public:
     Point (int32_t inX, int32_t inY);
     Point (const Point& inPoint);
    ~Point ();

    Point& operator= (const Point& inPoint)
        { x = inPoint.x; y = inPoint.y; return *this;}

    bool operator== (const Point& inPoint) const;

    int32_t getX () const {return x;}
    int32_t getY () const {return y;}

    void moveBy (const Point& inPoint);

    const string dump() const;

private:
    int32_t    x, y;
}; // class Point

inline Point::Point (int32_t inX, int32_t inY)
: x(inX), y(inY)
{
}

inline Point::Point (const Point& inPoint)
: x(inPoint.x), y(inPoint.y)
{
}

inline Point::~Point ()
{
}

inline bool Point::operator== (const Point& inPoint) const
{
    if (this == &inPoint)  // The same object?
        return true;

    return (x == inPoint.x && y == inPoint.y);
}

inline void Point::moveBy (const Point& inPoint)
{
    x += inPoint.x; y += inPoint.y;
}

} //namespace mhgui

#endif // POINT_H
