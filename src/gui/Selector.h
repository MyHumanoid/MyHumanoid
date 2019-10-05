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
 *  File   : Selector.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Texture.h"
#include "Widget.h"
#include "animorph/Color.h"
#include <string>

#define CURSOR_SIZE 6
#define HALF_CURSOR_SIZE CURSOR_SIZE / 2

using Animorph::Color;

using std::string;

namespace mhgui
{

class Tooltip;
class SelectorSysListener;

class Selector : public Widget
{
public:
	Texture texture;

	string imageFilename;
	bool textureIsInited; // used by lazyLoadTexture

	glm::ivec2 cursorPos;

	int rows;
	int cols;

	float maxValue;
	float cellRatio;

	std::vector<glm::ivec2>  points; // from left-bottom point to right-up point

	Color backgroundColor;
	Color cursorColor;

	// intentionally not implemented
	Selector(const Selector &);
	Selector & operator=(const Selector &);

public:
	Selector(uint32_t inId, const string & inFilename, const Rect & inGeometry);
	virtual ~Selector();

	void setCursorPosFromMousePoint(const glm::ivec2 & inMousePoint);

	std::vector<float> calculateDists() const;

	void setPoints(int inRows, int inCols);

	virtual void draw();

protected:
	bool lazyLoadTexture();
	bool lazyLoadCursorTexture();
};

} // namespace mhgui
