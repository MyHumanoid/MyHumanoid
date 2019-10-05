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
 *  File   : Widget.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Widget.h"
#include "gui/AbstractListener.h"
#include "gui/GLUTWrapper.h"

#include <iostream>

namespace mhgui
{

Widget::Widget(uint32_t inId, const Rect & inGeometry)
        : Component(inId, inGeometry)
        , parentPanel(NULL)
        , tooltip(NULL)
{
}

Widget::~Widget()
{
	if(parentPanel)
		parentPanel->removeWidget(this);
}

void Widget::setTooltip(const Tooltip & inTooltip)
{
}

void Widget::draw()
{
}

void Widget::show()
{
	setVisible(true);
}

void Widget::hide()
{
	setVisible(false);
}

} // namespace mhgui
