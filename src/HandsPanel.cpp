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
 *  File   : HandsPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "HandsPanel.h"
#include "ComponentID.h"
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

HandsPanel::HandsPanel()
        : Panel(kComponentID_HandsPanel, Rect(64, 208, 128, 96))
{
}

HandsPanel::~HandsPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
}

void HandsPanel::createWidgets()
{
	Window & mainWindow = *g_mainWindow;

	Panel * tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color color_red(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);
	const float alpha = 1.0;
	Image *     body;

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_HandsPanel_ForefingerLeftParams,
	                 searchPixmapFile("ui/hands_01.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Forefinger parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_HandsPanel_MiddlefingerLeftParams,
	                 searchPixmapFile("ui/hands_02.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Middlefinger parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_HandsPanel_RingfingerLeftParams,
	                 searchPixmapFile("ui/hands_03.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Ringfinger parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_04.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_HandsPanel_PollexLeftParams,
	                 searchPixmapFile("ui/hands_05.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Pollex parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_06.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_HandsPanel_LittlefingerLeftParams,
	                 searchPixmapFile("ui/hands_07.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Littlefinger parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_08.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_09.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_10.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_11.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/hands_12.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);
}
