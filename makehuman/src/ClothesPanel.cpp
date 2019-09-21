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
 *  File   : ClothesPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "ClothesPanel.h"
#include "ComponentID.h"
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

ClothesPanel::ClothesPanel()
    : Panel(kComponentID_ClothesPanel, Rect(0, 16, 96, 384))
{
}

ClothesPanel::~ClothesPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
}

void ClothesPanel::createWidgets()
{
	Window &mainWindow = Window::instance();

	Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color color_red(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);
	const float alpha = 1.0;
	Image *clothes;

	// -------------------------------------------------------------------------
	clothes =
	    new Image(kComponentID_ImageClothes_HatParams,
	              searchPixmapFile("ui/clothes_01.png"), Rect(0, 0, 32, 32));
	clothes->setListener(&imgListener);
	clothes->setTooltip(
	    Tooltip("Hat parameters", kTooltipPos, color_red, tooltipPanel));
	clothes->setAlpha(alpha);
	imageVector.push_back(clothes);
	addWidget(clothes);
}
