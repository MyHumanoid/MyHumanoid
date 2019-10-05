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
 *  File   : Selector.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <iostream>

#include <glm/glm.hpp>

#include "gui/Selector.h"
#include "gui/CGUtilities.h"
#include "gui/GLUTWrapper.h"

using std::cerr;
using std::endl;

namespace mhgui
{



// constructor
Selector::Selector(uint32_t inId, const std::string & inFilename, const Rect & inGeometry)
        : Widget(inId, inGeometry)
        , texture()
        , imageFilename(inFilename)
        , textureIsInited(false)
        , cursorPos(0, 0)
        , backgroundColor(1.0, 1.0, 1.0, 1.0)
        , cursorColor(1.0, 0.0, 0.0, 1.0)
{
}

Selector::~Selector()
{
}

void Selector::setCursorPosFromMousePoint(const glm::ivec2 & inMousePoint)
{
	glm::ivec2 foo;
	
	foo = glm::clamp(inMousePoint, getAbsolutePosition(), getAbsolutePosition() + getSize());
	
	// coordinate system change !!
	cursorPos = glm::ivec2(foo.x, getAbsolutePosition().y + getSize().y - foo.y);
}

// draw function
void Selector::draw()
{
	cgutils::enableBlend();
	if(lazyLoadTexture()) {
		cgutils::drawSquareFillTexture(getAbsoluteRect(), 1.0, texture);
	} else {
		cgutils::drawSquareFill(getAbsoluteRect(), backgroundColor);
	}

	// cursor
	const glm::ivec2 & pos  = getAbsolutePosition();
	const glm::ivec2 &  size = getSize();
	const Rect    cur(cursorPos.x + pos.x - HALF_CURSOR_SIZE,
						   (size.y + pos.y) - cursorPos.y -
								   HALF_CURSOR_SIZE,
						   CURSOR_SIZE, CURSOR_SIZE);
	
	cgutils::drawSquareFill(cur, cursorColor);

	glDisable(GL_BLEND);
}

bool Selector::lazyLoadTexture()
{
	bool &    isInited = textureIsInited;
	string &  filename = imageFilename;
	Texture & text     = texture;

	if(filename.empty())
		return false;

	if(isInited) // already loaded?
		return true;

	isInited = true;

	// read the PNG file using pngLoad
	// raw data from PNG file is in image buffer
	if(text.load(filename) == false) {
		cerr << "(pngLoad) " << filename << " FAILED" << endl;
		;
		return false;
	}

	return true;
}

} // namespace mhgui
