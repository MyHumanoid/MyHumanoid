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
 *  File   : SelectorSysListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/mhgui/SelectorSysListener.h"
#include "../include/mhgui/CGUtilities.h"
#include "../include/mhgui/Selector.h"
#include "../include/mhgui/GLUTWrapper.h"

#include <cstdio>
#include <cassert>
#include <iostream>

using namespace Animorph;
using namespace std;

namespace mhgui {

SelectorSysListener::SelectorSysListener()
    : AbstractListener()
{
}

SelectorSysListener::~SelectorSysListener()
{
}

bool SelectorSysListener::mouseOver (const Point& inMousePos, Component *source)
{
  cgutils::redisplay();

  return false;
}

bool SelectorSysListener::mouseOut (const Point& inMousePos, Component *source)
{
  cgutils::redisplay();

  return false;
}

bool SelectorSysListener::mouseDragged (const Point& inMousePos, Component *source)
{
  Selector *selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
  assert(selectorSource); // Check if this is really an Image object?

  selectorSource->setCursorPosFromMousePoint(inMousePos);

  cgutils::redisplay();

  return true;
}

bool SelectorSysListener::mouseWheel    (const Point& inMousePos, int inButton, Component *source )
{
  return false;
}

bool SelectorSysListener::mousePressed(const Point& inMousePos, int button, Component *source)
{
  Selector *selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
  assert(selectorSource); // Check if this is really an Image object?

  selectorSource->setCursorPosFromMousePoint(inMousePos);

  selectorSource->setActive(true);
  selectorSource->setClickConsumed(false);

  cgutils::redisplay();

  return true;
}

bool SelectorSysListener::mouseReleased (const Point& inMousePos, int button, Component *source)
{
  Selector *selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
  assert(selectorSource); // Check if this is really an Image object?

  selectorSource->setActive(false);
  selectorSource->setClickConsumed(false);

  return false;
}

bool SelectorSysListener::keyType (unsigned char key, Component *source)
{
  return false;
}
} // namespace mhgui
