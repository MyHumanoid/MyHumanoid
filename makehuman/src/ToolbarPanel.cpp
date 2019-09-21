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
 *  File   : ToolbarPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "ToolbarPanel.h"
#include "ComponentID.h"
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>

using namespace std;
using namespace Animorph;

ToolbarPanel::ToolbarPanel()
    : Panel(kComponentID_ToolbarPanel, Rect(192, 16, 1248, 32))
{
}

ToolbarPanel::~ToolbarPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
}

void ToolbarPanel::createWidgets()
{
	Window &mainWindow = *g_mainWindow;
	Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color color_red(1.0, 0.0, 0.0);
	const float alpha = 1.0;
	const Point kTooltipPos(70, 12);

	Image *image;

	// -------------------------------------------------------------------------
	image =
	    new Image(kComponentID_ImageToolbarPanel_LoadBodysettings,
	              searchPixmapFile("ui/toolbar_load.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Load bodysetting", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_ExportWavefrontObj,
	                  searchPixmapFile("ui/toolbar_exp.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Export wavefront obj", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_ExportColladaObj,
	                  searchPixmapFile("ui/toolbar_exp_collada.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Export Collada file", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_CharacterSettings,
	                  searchPixmapFile("ui/toolbar_charac_sett.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Character setting (somatotypes, shapes, age, etc...)",
	            kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_BodyDetailsRealistic,
	                  searchPixmapFile("ui/toolbar_body_det_real.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(Tooltip("Body details (realistic morphings)", kTooltipPos,
	                          color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);
	/*
	  // -------------------------------------------------------------------------
	  image = new Image(kComponentID_ImageToolbarPanel_Clothes,
	                    searchPixmapFile ("ui/toolbar_07.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Clothes", kTooltipPos, color_red, tooltipPanel));
	  image->setAlpha (alpha);
	  imageVector.push_back (image);
	  addWidget (image);
	*/
	// -------------------------------------------------------------------------
	image =
	    new Image(kComponentID_ImageToolbarPanel_Poses,
	              searchPixmapFile("ui/toolbar_poses.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(Tooltip("Poses", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_BodySettings,
	                  searchPixmapFile("ui/toolbar_body_settings.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Characters Library", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_PoseBodySettings,
	                  searchPixmapFile("ui/toolbar_pose_body_settings.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Poses Library", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
#ifdef _ANIMATIONS
	image =
	    new Image(kComponentID_ImageToolbarPanel_Animation,
	              searchPixmapFile("ui/toolbar_util.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(Tooltip("Utilities", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);
#endif
	/*
	  // -------------------------------------------------------------------------
	  image = new Image(kComponentID_ImageToolbarPanel_Lights,
	                    searchPixmapFile ("ui/toolbar_09.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Lights", kTooltipPos, color_red, tooltipPanel));
	  image->setAlpha (alpha);
	  imageVector.push_back (image);
	  addWidget (image);

	  // -------------------------------------------------------------------------
	  image = new Image(kComponentID_ImageToolbarPanel_Expressions,
	                    searchPixmapFile ("ui/toolbar_10.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Expressions", kTooltipPos, color_red,
	  tooltipPanel)); image->setAlpha (alpha); imageVector.push_back (image);
	  addWidget (image);
	*/
	// -------------------------------------------------------------------------
	/*  image = new Image(kComponentID_ImageToolbarPanel_Asymmetry,
	                    searchPixmapFile ("ui/toolbar_11.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Asymmetry", kTooltipPos, color_red,
	  tooltipPanel)); image->setAlpha (alpha); image->setEnabled(false);
	  imageVector.push_back (image);
	  addWidget (image);

	  // -------------------------------------------------------------------------
	  image = new Image(kComponentID_ImageToolbarPanel_Hairs,
	                    searchPixmapFile ("ui/toolbar_12.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Hairs", kTooltipPos, color_red, tooltipPanel));
	  image->setAlpha (alpha);
	  image->setEnabled(false);
	  imageVector.push_back (image);
	  addWidget (image);
	*/
	/*
	  // -------------------------------------------------------------------------
	  image = new Image(kComponentID_ImageToolbarPanel_Materials,
	                    searchPixmapFile ("ui/toolbar_13.png"),
	                    Rect(0,0,32,32));
	  image->setListener(&imgListener1);
	  image->setTooltip(Tooltip("Materials", kTooltipPos, color_red,
	  tooltipPanel)); image->setAlpha (alpha); imageVector.push_back (image);
	  addWidget (image);
	*/
	// -------------------------------------------------------------------------
	image =
	    new Image(kComponentID_ImageToolbarPanel_Rendering,
	              searchPixmapFile("ui/toolbar_rend.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(Tooltip("Rendering", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(1.0);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_RenderingPreview,
	                  searchPixmapFile("ui/toolbar_rend_preview.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Rendering preview", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(1.0);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_MorphingList,
	                  searchPixmapFile("ui/toolbar_morph_list.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Used morphing list", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image =
	    new Image(kComponentID_ImageToolbarPanel_Reset,
	              searchPixmapFile("ui/toolbar_reset.png"), Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Reset Mesh", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_ImageToolbarPanel_LoadBackground,
	                  searchPixmapFile("ui/toolbar_load_bckgnd.png"),
	                  Rect(0, 0, 32, 32));
	image->setListener(&imgListener1);
	image->setTooltip(
	    Tooltip("Load Background", kTooltipPos, color_red, tooltipPanel));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/frames_06.png"),
	                  Rect(0, 0, 32, 32));
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);

	// -------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/frames_01.png"),
#ifdef _ANIMATIONS
	                  Rect(0, 0, 736, 16)); // w = 1280 - 32x17
#else
	                  Rect(0, 0, 768, 16)); // w = 1280 - 32x17
#endif
	image->setAlpha(alpha);
	imageVector.push_back(image);
	addWidget(image);
}
