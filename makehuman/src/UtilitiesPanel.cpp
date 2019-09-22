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
 *  File   : UtilitiesPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "UtilitiesPanel.h"
#include "ComponentID.h"
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>

using namespace std;
using namespace Animorph;

UtilitiesPanel::UtilitiesPanel()
    : Panel(kComponentID_UtilitiesPanel, Rect(0, 16, 192, 430))
{
}

UtilitiesPanel::~UtilitiesPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	for_each(targetVector.begin(), targetVector.end(),
	         deleteFunctor<TargetSlider *>());
}

void UtilitiesPanel::createWidgets()
{
	Window &mainWindow = *g_mainWindow;
	Global &global = g_global;
	Animation *animation = global.getAnimation();

	Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color c(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);
	const float alpha = 1.0;
	const int overlayMultiplier = 1;

	// -------------------------------------------------------------------------
	target_slider = new TargetSlider(kComponentID_UtilitiesPanel_Interpolations,
	                                 searchPixmapFile("ui/utilitiespanel_01.png"),
	                                 "", Rect(0, 0, 64, 74), 0.0, 10.0);

	target_slider->setListener(&utilitiesSliderListener);
	target_slider->setTooltip(
	    Tooltip("Interpolations", kTooltipPos, c, tooltipPanel));
	target_slider->setOverlayMultiplier(overlayMultiplier);
	target_slider->setAlpha(alpha);
	target_slider->setStep(1.0);
	target_slider->setSliderValue(animation->getInterpNumber());
	targetVector.push_back(target_slider);
	addWidget(target_slider);

	// -------------------------------------------------------------------------
	target_slider = new TargetSlider(kComponentID_UtilitiesPanel_FrameRate,
	                                 searchPixmapFile("ui/utilitiespanel_02.png"),
	                                 "", Rect(0, 0, 64, 74), 1.0, 100.0);

	target_slider->setListener(&utilitiesSliderListener);
	target_slider->setTooltip(
	    Tooltip("Frame rate per sec", kTooltipPos, c, tooltipPanel));
	target_slider->setOverlayMultiplier(overlayMultiplier);
	target_slider->setAlpha(alpha);
	target_slider->setStep(1.0);
	target_slider->setSliderValue(animation->getFrameRate());
	targetVector.push_back(target_slider);
	addWidget(target_slider);

	// -------------------------------------------------------------------------
	img = new Image(kComponentID_Dummy, searchPixmapFile("ui/filler_2.png"),
	                Rect(0, 0, 64, 64));
	img->setAlpha(alpha);
	imageVector.push_back(img);
	addWidget(img);

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	img = new Image(kComponentID_UtilitiesPanel_Reset,
	                searchPixmapFile("ui/utilitiespanel_04.png"),
	                Rect(0, 0, 64, 64));
	img->setListener(&imgListener);
	img->setTooltip(Tooltip("Reset animation", kTooltipPos, c, tooltipPanel));
	img->setAlpha(alpha);
	imageVector.push_back(img);
	addWidget(img);

	// -------------------------------------------------------------------------
	img = new Image(kComponentID_Dummy, searchPixmapFile("ui/filler_2.png"),
	                Rect(0, 0, 64, 64));
	img->setAlpha(alpha);
	imageVector.push_back(img);
	addWidget(img);
}

/*
void CharacterSettingPanel::resetTargetValues(const string currentTarget)
{
  for (vector <TargetSlider*>::const_iterator targetvector_it =
targetVector.begin (); targetvector_it != targetVector.end ();
         targetvector_it++)
  {
    if((*targetvector_it)->getTargetName() != currentTarget)
    {
      (*targetvector_it)->setSliderValue(0);
    }
  }
}
*/
