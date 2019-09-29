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
 *  File   : PosesBodyPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "PosesBodyPanel.h"
#include "ComponentID.h"
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

PosesBodyPanel::PosesBodyPanel()
        : Panel(kComponentID_BodyPanel, Rect(0, 16, 192, 448))
{
}

PosesBodyPanel::~PosesBodyPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
}

void PosesBodyPanel::createWidgets()
{
	Window & mainWindow = *g_mainWindow;

	Panel * tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color color_red(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);
	const float alpha = 1.0;
	Image *     body;

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightCollarParams,
	                 searchPixmapFile("ui/rotations_01.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Right collar parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_HeadParams,
	                 searchPixmapFile("ui/rotations_02.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Head parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftCollarParams,
	                 searchPixmapFile("ui/rotations_03.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left collar parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftRingfinger3Params,
	                 searchPixmapFile("ui/rotations_04.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left ringfinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftMiddlefinger3Params,
	                 searchPixmapFile("ui/rotations_05.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left middlefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftForefinger3Params,
	                 searchPixmapFile("ui/rotations_06.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left forefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightUpperArmParams,
	                 searchPixmapFile("ui/rotations_07.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right upper arm parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_NeckParams,
	                 searchPixmapFile("ui/rotations_08.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Neck parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftUpperArmParams,
	                 searchPixmapFile("ui/rotations_09.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left upper arm parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftRingfinger2Params,
	                 searchPixmapFile("ui/rotations_10.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left ringfinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftMiddlefinger2Params,
	                 searchPixmapFile("ui/rotations_11.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left middlefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftForefinger2Params,
	                 searchPixmapFile("ui/rotations_12.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left forefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_13.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_TorsoParams,
	                 searchPixmapFile("ui/rotations_14.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Torso parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_15.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftLittlefinger3Params,
	                 searchPixmapFile("ui/rotations_16.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left littlefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftMiddlefinger1Params,
	                 searchPixmapFile("ui/rotations_17.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left middlefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftForefinger1Params,
	                 searchPixmapFile("ui/rotations_18.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left forefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightLowerArmParams,
	                 searchPixmapFile("ui/rotations_19.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right lower arm parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_PivotTransformation,
	                 searchPixmapFile("ui/rotations_20.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Pivot transformation parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftLowerArmParams,
	                 searchPixmapFile("ui/rotations_21.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left lower arm parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftLittlefinger2Params,
	                 searchPixmapFile("ui/rotations_22.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left littlefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftRingfinger1Params,
	                 searchPixmapFile("ui/rotations_23.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left ringfinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftPollex3Params,
	                 searchPixmapFile("ui/rotations_24.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left pollex 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightHandParams,
	                 searchPixmapFile("ui/rotations_25.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Right hand parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_26.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftHandParams,
	                 searchPixmapFile("ui/rotations_27.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left hand parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftLittlefinger1Params,
	                 searchPixmapFile("ui/rotations_28.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left littlefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftPollex1Params,
	                 searchPixmapFile("ui/rotations_29.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left pollex 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftPollex2Params,
	                 searchPixmapFile("ui/rotations_30.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left pollex 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightUpperLegParams,
	                 searchPixmapFile("ui/rotations_31.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right upper leg parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_32.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftUpperLegParams,
	                 searchPixmapFile("ui/rotations_33.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left upper leg parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightForefinger3Params,
	                 searchPixmapFile("ui/rotations_34.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right forefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightMiddlefinger3Params,
	                 searchPixmapFile("ui/rotations_35.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right middlefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightRingfinger3Params,
	                 searchPixmapFile("ui/rotations_36.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right ringfinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightLowerLegParams,
	                 searchPixmapFile("ui/rotations_37.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right lower leg parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_38.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftLowerLegParams,
	                 searchPixmapFile("ui/rotations_39.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left lower leg parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightForefinger2Params,
	                 searchPixmapFile("ui/rotations_40.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right forefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightMiddlefinger2Params,
	                 searchPixmapFile("ui/rotations_41.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right middlefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightRingfinger2Params,
	                 searchPixmapFile("ui/rotations_42.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right ringfinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_43.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_44.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_45.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightForefinger1Params,
	                 searchPixmapFile("ui/rotations_46.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right forefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightMiddlefinger1Params,
	                 searchPixmapFile("ui/rotations_47.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right middlefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightLittlefinger3Params,
	                 searchPixmapFile("ui/rotations_48.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right littlefinger 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFootParams,
	                 searchPixmapFile("ui/rotations_49.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Right foot parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_50.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFootParams,
	                 searchPixmapFile("ui/rotations_51.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(Tooltip("Left foot parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightPollex3Params,
	                 searchPixmapFile("ui/rotations_52.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right pollex 3 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightRingfinger1Params,
	                 searchPixmapFile("ui/rotations_53.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right ringfinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightLittlefinger2Params,
	                 searchPixmapFile("ui/rotations_54.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right littlefinger 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_55.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_56.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_57.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightPollex2Params,
	                 searchPixmapFile("ui/rotations_58.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right pollex 2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightPollex1Params,
	                 searchPixmapFile("ui/rotations_59.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right pollex 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightLittlefinger1Params,
	                 searchPixmapFile("ui/rotations_60.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right littlefinger 1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot42Params,
	                 searchPixmapFile("ui/rotations_61.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 4_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot52Params,
	                 searchPixmapFile("ui/rotations_62.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 5_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot51Params,
	                 searchPixmapFile("ui/rotations_63.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 5_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot51Params,
	                 searchPixmapFile("ui/rotations_64.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 5_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot52Params,
	                 searchPixmapFile("ui/rotations_65.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 5_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot42Params,
	                 searchPixmapFile("ui/rotations_66.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 4_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot32Params,
	                 searchPixmapFile("ui/rotations_67.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 3_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot31Params,
	                 searchPixmapFile("ui/rotations_68.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 3_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot41Params,
	                 searchPixmapFile("ui/rotations_69.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 4_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot41Params,
	                 searchPixmapFile("ui/rotations_70.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 4_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot31Params,
	                 searchPixmapFile("ui/rotations_71.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 3_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot32Params,
	                 searchPixmapFile("ui/rotations_72.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 3_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot22Params,
	                 searchPixmapFile("ui/rotations_73.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 2_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot21Params,
	                 searchPixmapFile("ui/rotations_74.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 2_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot11Params,
	                 searchPixmapFile("ui/rotations_75.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 1_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot11Params,
	                 searchPixmapFile("ui/rotations_76.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 1_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot21Params,
	                 searchPixmapFile("ui/rotations_77.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 2_1 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot22Params,
	                 searchPixmapFile("ui/rotations_78.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 2_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_RightFoot12Params,
	                 searchPixmapFile("ui/rotations_79.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Right footfinger 1_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_80.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_81.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_82.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_Dummy, searchPixmapFile("ui/rotations_83.png"),
	                 Rect(0, 0, 32, 32));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);

	// -------------------------------------------------------------------------
	body = new Image(kComponentID_PosesImageBody_LeftFoot12Params,
	                 searchPixmapFile("ui/rotations_84.png"), Rect(0, 0, 32, 32));
	body->setListener(&imgListener);
	body->setTooltip(
	        Tooltip("Left footfinger 1_2 parameters", kTooltipPos, color_red, tooltipPanel));
	body->setAlpha(alpha);
	imageVector.push_back(body);
	addWidget(body);
}
