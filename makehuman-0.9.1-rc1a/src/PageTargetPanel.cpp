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
 *  File   : PageTargetPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "PageTargetPanel.h"
#include "ComponentID.h"
#include "Global.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/ImageSlider.h>
#include <mhgui/Tooltip.h>
#include "Global.h"

using namespace std;
using namespace Animorph;

#include "ComponentID.h"


PageTargetPanel::PageTargetPanel (const string& category, const Rect& rect)
    : Panel (kComponentID_TargetPanel, rect),
    category (category)
{
  setRowSpacing(3);
}


PageTargetPanel::~PageTargetPanel ()
{
  for_each (targetVector.begin (), targetVector.end (), deleteFunctor <TargetSlider*> ());
}
void PageTargetPanel::resetTargetValues(const string currentTargetName )
{
  for (vector <TargetSlider*>::const_iterator targetvector_it = targetVector.begin ();
         targetvector_it != targetVector.end ();
         targetvector_it++)
  {
    if((*targetvector_it)->getTargetName() != currentTargetName)
    {
      (*targetvector_it)->setSliderValue(0);
    }
  }
}
