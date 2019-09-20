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
 *  File   : TeethPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "TeethPanel.h"
#include "ImageListener.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Tooltip.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

TeethPanel::TeethPanel ()
    : Panel (kComponentID_TeethPanel, Rect(0,304,192,32))
{
}

TeethPanel::~TeethPanel ()
{
  for_each (imageVector.begin (), imageVector.end (), deleteFunctor <Image*> ());
}

void TeethPanel::createWidgets ()
{
  Window &mainWindow = Window::instance ();

  Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
  assert(tooltipPanel);

  const Color color_red (1.0, 0.0, 0.0);
  const Point kTooltipPos(70, 12);
  const float alpha = 1.0;
  Image *body;

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_TeethPanel_TongueParams,
                   searchPixmapFile ("ui/teeth_01.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Tongue parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_TeethPanel_TeethParams,
                   searchPixmapFile ("ui/teeth_02.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Teeth parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/teeth_03.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/teeth_04.png"),
                   Rect(0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget (body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/teeth_05.png"),
                   Rect(0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget (body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/teeth_06.png"),
                   Rect(0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget (body);
}
