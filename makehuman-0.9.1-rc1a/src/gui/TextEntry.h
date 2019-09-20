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

#ifndef TEXTENTRY_H
#define TEXTENTRY_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include "animorph/Color.h"
#include "Widget.h"
#include "Panel.h"
#include "TextEntrySysListener.h"
//#include "ImageData.h"

using Animorph::Color;
using std::string;

namespace mhgui {

class Tooltip;


/** \brief Single line input field widget
 */
class TextEntry : public Widget
{
private:
  Color overlay;
  Color backColor;
  Color textColor;
  Color borderColor;
  TextEntrySysListener *textEntrySysListener;
  bool overlayEffect;
  string text;

  // intentionally not implemented
  TextEntry           (const TextEntry&);
  TextEntry& operator=(const TextEntry&);

public:
  TextEntry (uint32_t inId, const Rect& inGeometry);
  virtual ~TextEntry();

  void  setBackgroundColor (const Color& c) {backColor = c;}
  const Color& getBackgroundColor () const {return backColor;}

  void setTextColor (const Color& c) {textColor = c;}
  const Color& getTextColor () const {return textColor;}

  void setBorderColor (const Color& c) {borderColor = c;}
  const Color& getBorderColor () const {return borderColor;}

  /*!
   * Set the Color for the overlay rectangle. The overlay is enabled at this time.
   * @param c The color for the overlay. Transparency is set by the alpha component.
   */
  void setOverlayRectangle (const Color& c);

  void setOverlayRectangle (bool overlayEffect);

  void setText (const string& text) {this->text = text;}
  const string& getText () const {return text;}

  void addChar (const char c);
  void removeChar ();

  virtual void show ();
  virtual void hide ();
  virtual void draw ();
  virtual void drawOverlay ();
};

} // namespace mhgui

#endif //TEXTENTRY_H
