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
 *  File   : Tooltip.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Point.h"
#include <animorph/Color.h>
#include <string>

using Animorph::Color;
using std::string;

namespace mhgui
{

class Panel;

/* ========================================================================== */
/**
 */
/* ========================================================================== */
class Tooltip
{

public:
	Tooltip(const string & inTooltipString, const Point & inPos, const Color & inColor,
	        const Panel * inAnchor = NULL);

	Tooltip(const string & inTooltipString, const Point & inPos, const Panel * inAnchor = NULL);

	Tooltip(const Tooltip & inTooltip);

	~Tooltip() {}

	Tooltip & operator=(const Tooltip & inTooltip);

	void draw() const;

private:
	string        label;
	Point         pos;
	Color         color;
	const Panel * anchorPanel;
}; // class Tooltip

} // namespace mhgui
