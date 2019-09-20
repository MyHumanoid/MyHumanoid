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
 *  File   : Widget.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef WIDGET_H
#define WIDGET_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include "Component.h"
#include "Panel.h"

namespace mhgui {

class Tooltip;

class Widget : public Component
{
public:
  Widget (uint32_t inId, const Rect& inGeometry);

  virtual ~Widget ();

  virtual void draw() = 0;
  virtual void drawOverlay() = 0;
  virtual void show () = 0;
  virtual void hide () = 0;

  Panel* getParentPanel () const {return parentPanel;}
  void   setTooltip     (const Tooltip& inTooltip);

private: // intentionally not implemented
  Widget             (const Widget&);
  Widget& operator = (const Widget&);

private:
  Panel       *parentPanel;
  Tooltip     *tooltip;

  // Special Draw method which will be called from panel ONLY!
  // This call takes care to paint the tooltip and call draw()
  // afterwards
  void draw_wrapper();

  // TODO: do we really need this, I think friend isn't very good design
  friend class Panel; // grant panel access to the private members & methods
  friend class MultiPanel; // grant multipanel access to the private members & methods
};

} // namespace mhgui

#endif //WIDGET_H
