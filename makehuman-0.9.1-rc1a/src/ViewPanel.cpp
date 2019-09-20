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
 *  File   : ViewPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "ViewPanel.h"
#include "ImageListener.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Image.h>
#include <mhgui/Tooltip.h>

using namespace std;
using namespace Animorph;

#include "ComponentID.h"
#include <mhgui/Tooltip.h>

ViewPanel::ViewPanel (const int winHeight)
    : Panel (kComponentID_ViewPanel, Rect(0,winHeight - 32,400,32))
{
  setBottomAnchor (true);
}

ViewPanel::~ViewPanel ()
{
  delete view1;
  delete view2;
}

void ViewPanel::createWidgets ()
{
  const float alpha = 0.8;

  view2 = new Image(kComponentID_Dummy,
                    searchPixmapFile ("ui/frames_01.png"),
                    Rect(0,0,368,32));
  view2->setAlpha (alpha);
  addWidget (view2);

  view1 = new Image(kComponentID_Dummy,
                    searchPixmapFile ("ui/frames_08.png"),
                    Rect(0,0,32,32));
  view1->setAlpha (alpha);
  addWidget (view1);
}
