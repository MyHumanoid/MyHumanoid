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
 *  File   : SelectorListener.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef SELECTORLISTENER_H
#define SELECTORLISTENER_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <mhgui/AbstractListener.h>
#include <mhgui/Component.h>
#include <mhgui/Point.h>
#include <mhgui/Selector.h>
#include <vector>

using namespace mhgui;

class SelectorListener : public AbstractListener
{
private:

  std::vector<string> ageLabels;
  std::vector<string> muscleSizeLabels;
  std::vector<string> breastLabels;
  std::vector<string> shapeLabels;

  void calcWidgetTargets(Selector &selectorSource);

  Point oldPos;

public:
  SelectorListener();
  virtual ~SelectorListener();

  virtual bool mouseOver     (const Point& inMousePos,               Component *source );
  virtual bool mouseOut      (const Point& inMousePos,               Component *source );
  virtual bool mousePressed  (const Point& inMousePos, int inButton, Component *source );
  virtual bool mouseReleased (const Point& inMousePos, int inButton, Component *source );
  virtual bool mouseDragged  (const Point& inMousePos,               Component *source );
  virtual bool mouseWheel    (const Point& inMousePos, int inButton, Component *source );
  virtual bool keyType       (unsigned char inKey,                   Component *source );

  std::vector<float> ageDists;
  std::vector<float> muscleSizeDists;
  std::vector<float> breastDists;
  std::vector<float> shapeDists;
};

#endif //SELECTORLISTENER_H
