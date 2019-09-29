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
 *  File   : SplashPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "SplashPanel.h"
#include "ComponentID.h"

#include <gui/Image.h>
#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>

using namespace std;
using namespace Animorph;
using namespace mhgui;

#include "ComponentID.h"

SplashPanel::SplashPanel(const Size & winSize)
        //: Panel (kComponentID_SplashPanel, Rect(220,70,360,460))
        : Panel(kComponentID_SplashPanel,
                Rect((winSize.getWidth() / 2 - 240), (winSize.getHeight() / 2 - 180), 480, 360))
{
}

SplashPanel::~SplashPanel() { delete frame1; }

void SplashPanel::createWidgets()
{
	frame1 = new Image(kComponentID_Dummy, searchPixmapFile("ui/splash_01.png"),
	                   Rect(0, 0, 480, 360));

	addWidget(frame1);
}
