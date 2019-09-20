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
 *  File   : PageBsPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "PageBsPanel.h"
#include "ComponentID.h"
#include "Global.h"

#include <gui/Window.h>
#include <gui/Rect.h>
#include <gui/ImageSlider.h>
#include <gui/Tooltip.h>
#include "Global.h"

using namespace std;
using namespace Animorph;

#include "ComponentID.h"


PageBsPanel::PageBsPanel (const string& category, const Rect& rect)
    : Panel (kComponentID_TargetPanel, rect),
    category (category)
{

}

PageBsPanel::~PageBsPanel ()
{
  for_each (imageButtonVector.begin (), imageButtonVector.end (), deleteFunctor <ImageButton*> ());
}

