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
 *  File   : TextEntry.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef LABEL_H
#define LABEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <iostream>
#include <animorph/Color.h>
#include "Widget.h"
#include "Panel.h"
#include "CGUtilities.h"

using Animorph::Color;
using std::string;

namespace mhgui {

class Tooltip;

/// Fixed text display widget
class Label : public Widget
{
private:
  Color backColor;
  Color textColor;
  string text;
  FontType font;
  bool autosize;

  // intentionally not implemented
  Label           (const Label&);
  Label& operator=(const Label&);

  void calcAutoSize ();

public:
  Label (uint32_t inId, const Rect& inGeometry);
  virtual ~Label();

  // TODO: Background color not yet working!
  void  setBackgroundColor (const Color& c) {backColor = c;}
  const Color& getBackgroundColor () {return backColor;}

  void setTextColor (const Color& c) {textColor = c;}
  const Color& getTextColor () {return textColor;}

  void setText (const string& text);
  const string& getText () const {return text;}

  void setAutoSize (bool autosize);
  bool getAutoSize () const {return autosize;}

  /*!
   * @param font define a GLUT font
   */
  void setFont (FontType font) {this->font = font;}

  FontType getFont () const {return font;}

  virtual void show ();
  virtual void hide ();
  virtual void draw ();
  virtual void drawOverlay ();
};

} // namespace mhgui

#endif // LABEL_H
