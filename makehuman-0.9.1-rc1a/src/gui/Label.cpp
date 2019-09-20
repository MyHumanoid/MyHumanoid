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
 *  File   : Label.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Label.h"
#include "gui/Tooltip.h"
#include "gui/GLUTWrapper.h"

#include <iostream>

using namespace std;

using std::cerr;
using std::endl;

namespace mhgui {

//constructor
Label::Label (uint32_t      inId,
              const Rect&   inGeometry)
    : Widget(inId, inGeometry),
    backColor (1, 1, 1, 1),
    textColor (0, 0, 0, 1),
    font (GLUT_BITMAP_HELVETICA_12)
{
}

Label::~Label()
{
}

void Label::show ()
{
  setVisible(true);
}

void Label::hide ()
{
  setVisible(false);
}

void Label::drawOverlay()
{
}

void Label::setText (const string& text)
{
  this->text = text;

  if (autosize)
    calcAutoSize ();
}

void Label::setAutoSize (bool autosize)
{
  this->autosize = autosize;

  if (autosize)
    calcAutoSize ();
}

void Label::calcAutoSize ()
{
  Size newSize (cgutils::getFontLength (font, text),
                cgutils::getFontWidth (font));

  setSize (newSize);
}

//draw function
void Label::draw ()
{
  if (isVisible())
  {
    cgutils::enableBlend ();

    //cgutils::enableScissor (getAbsoluteRect ());

    Point pos_rel (0, cgutils::getFontWidth (font));
    Point pos (getAbsolutePosition());
    pos.moveBy (pos_rel);

    cgutils::drawString (pos, font, text, textColor);

    //cgutils::disableScissor ();

    cgutils::disableBlend ();
  }
}

} // namespace mhgui

