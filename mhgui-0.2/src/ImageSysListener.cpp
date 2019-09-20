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
 *  File   : ImageSysListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/mhgui/ImageSysListener.h"
#include "../include/mhgui/CGUtilities.h"
#include "../include/mhgui/Image.h"
#include "../include/mhgui/GLUTWrapper.h"

#include <cstdio>
#include <cassert>
#include <iostream>

using namespace Animorph;
using namespace std;

namespace mhgui {

ImageSysListener::ImageSysListener()
    : AbstractListener()
{
}

ImageSysListener::~ImageSysListener()
{
}

bool ImageSysListener::mouseOver (const Point& inMousePos, Component *source)
{
  cgutils::redisplay();

  return false;
}

bool ImageSysListener::mouseOut (const Point& inMousePos, Component *source)
{
  cgutils::redisplay();

  return false;
}

bool ImageSysListener::mouseDragged (const Point& inMousePos, Component *source)
{
  return false;
}

bool ImageSysListener::mouseWheel    (const Point& inMousePos, int inButton, Component *source )
{
  return false;
}

bool ImageSysListener::mousePressed(const Point& inMousePos, int button, Component *source)
{
  Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
  assert(imgSource); // Check if this is really an Image object?

  imgSource->setActive(true);
  imgSource->setClickConsumed(false);

  return false;
}

bool ImageSysListener::mouseReleased (const Point& inMousePos, int button, Component *source)
{
  Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
  assert(imgSource); // Check if this is really an Image object?

  imgSource->setActive(false);
  imgSource->setClickConsumed(false);

  return false;
}

bool ImageSysListener::keyType (unsigned char key, Component *source)
{
  return false;
}
} // namespace mhgui
