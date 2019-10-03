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
 *  File   : AbstractListener.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "gui/Point.h"

/*! \brief Classes to build the GUI of MakeHuman.
 */
namespace mhgui
{

class Component;

/// Abstract callback interface for mouse and keyboard events
class AbstractListener
{
public:
	AbstractListener() {}
	virtual ~AbstractListener() {}

	virtual bool mouseOver(const Point & inMousePos, Component * source)                   = 0;
	virtual bool mouseOut(const Point & inMousePos, Component * source)                    = 0;
	virtual bool mousePressed(const Point & inMousePos, int inButton, Component * source)  = 0;
	virtual bool mouseReleased(const Point & inMousePos, int inButton, Component * source) = 0;
	virtual bool mouseDragged(const Point & inMousePos, Component * source)                = 0;
	virtual bool mouseWheel(const Point & inMousePos, int inButton, Component * source)    = 0;
	virtual bool keyType(unsigned char inKey, Component * source)                          = 0;
};

} // namespace mhgui
