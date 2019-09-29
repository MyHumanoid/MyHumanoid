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
 *  File   : PoseTargetPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "PoseTargetPanel.h"
#include "ComponentID.h"
#include "Global.h"

#include "Global.h"
#include <gui/ImageSlider.h>
#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>

using namespace std;
using namespace Animorph;

#include "ComponentID.h"

const static string       kFilePrefixTarget(".target");
const static string       kFilePrefixPNG(".png");
const static unsigned int kMaxTargetsNumberStep1 = 17;
const static unsigned int kMaxTargetsNumberStep2 = 65;

PoseTargetPanel::PoseTargetPanel(const string & category, const Rect & rect)
        : Panel(kComponentID_TargetPanel, rect)
        , category(category)
{
	setRightAnchor(true);
	setRowSpacing(5);
	setColumnSpacing(3);
}

PoseTargetPanel::~PoseTargetPanel()
{
	for_each(targetVector.begin(), targetVector.end(), deleteFunctor<TargetSlider *>());
	delete close_image;
}

void PoseTargetPanel::createWidgets()
{
	Window & mainWindow = *g_mainWindow;

	Panel * tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	Mesh * mesh = g_global.getMesh();
	assert(mesh);

	PoseMap & posemap = mesh->getPoseMapRef();

	const Color c(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);

	const BodySettings & bodyset(mesh->getPoses());

	TargetSlider * image_slider;

	if(category == applied_target_list) {
		int targetSize;
		int overlayZoom;

		if(bodyset.size() > kMaxTargetsNumberStep2) {
			targetSize  = 24;
			overlayZoom = 2;
		} else if(bodyset.size() > kMaxTargetsNumberStep1) {
			targetSize  = 32;
			overlayZoom = 2;
		} else {
			targetSize  = 64;
			overlayZoom = 1;
		}

		close_image = new Image(kComponentID_CloseTargetPanel,
		                        searchPixmapFile("ui/close_panel.png"),
		                        Rect(0, 0, targetSize, targetSize));
		close_image->setListener(&imgListener);
		close_image->setTooltip(Tooltip("Close panel", kTooltipPos, c, tooltipPanel));
		addWidget(close_image);

		for(BodySettings::const_iterator bodyset_it = bodyset.begin();
		    bodyset_it != bodyset.end(); bodyset_it++) {
			const string & target_name(bodyset_it->first);
			float          target_value = bodyset_it->second;

			string target_image(target_name);
			target_image.replace(target_image.length() - kFilePrefixTarget.length(),
			                     kFilePrefixTarget.length(), kFilePrefixPNG);

			image_slider = new TargetSlider(
			        kComponentID_TargetPanel_Target,
			        searchPixmapFile("rotimg/" + target_name + kFilePrefixPNG),
			        target_name, Rect(0, 0, targetSize, targetSize + 10), 0.0, 1.0);

			PoseTarget * poseTarget = mesh->getPoseTargetForName(target_name);
			assert(poseTarget);
			/*
			      if(!poseTarget->hasNegative()) image_slider->setMinValue(0);
			      else                           image_slider->setMinValue(-1.0);
			*/
			image_slider->setMinValue(poseTarget->getMinAngle());
			image_slider->setMaxValue(poseTarget->getMaxAngle());

			image_slider->setSliderValue(target_value);
			image_slider->setListener(&imgSliderListener);
			image_slider->setOverlayMultiplier(overlayZoom);
			image_slider->setStep(1.0);
			image_slider->setTooltip(
			        Tooltip(target_name, kTooltipPos, c, tooltipPanel));
			targetVector.push_back(image_slider);
			addWidget(image_slider);
		}
	} else {
		close_image = new Image(kComponentID_CloseTargetPanel,
		                        searchPixmapFile("ui/close_panel.png"), Rect(0, 0, 64, 64));
		close_image->setListener(&imgListener);
		close_image->setTooltip(Tooltip("Close panel", kTooltipPos, c, tooltipPanel));
		addWidget(close_image);

		for(PoseMap::const_iterator posemap_it = posemap.begin();
		    posemap_it != posemap.end(); posemap_it++) {
			const string & target_name(posemap_it->first);

			string::size_type loc = target_name.find("/", 0);
			if(loc == string::npos)
				continue;
			else {
				string sub = target_name.substr(0, loc);

				if(sub == category) {
					string target_image(target_name);

					//          target_image.replace (target_image.length () -
					//          kFilePrefixTarget.length (),
					//                                kFilePrefixTarget.length
					//                                (), kFilePrefixPNG);
					//          std::cout << target_name << endl;
					image_slider = new TargetSlider(
					        kComponentID_TargetPanel_Target,
					        searchPixmapFile("rotimg/" + target_name +
					                         kFilePrefixPNG),
					        target_name, Rect(0, 0, 64, 74), 0.0, 1.0);

					PoseTarget * poseTarget =
					        mesh->getPoseTargetForName(target_name);
					assert(poseTarget);

					//          if(!poseTarget->hasNegative())
					//          image_slider->setMinValue(0); else
					//          image_slider->setMinValue(-1.0);

					image_slider->setMinValue(poseTarget->getMinAngle());
					image_slider->setMaxValue(poseTarget->getMaxAngle());

					image_slider->setOverlayMultiplier(1);
					image_slider->setStep(1.0);

					BodySettings::const_iterator bodyset_it =
					        bodyset.find(target_name);

					// FIX: Make sure that a bodyset with the given name really
					// exists!
					float target_value = (bodyset_it != bodyset.end())
					                             ? bodyset_it->second
					                             : 0.0f;

					image_slider->setSliderValue(target_value);
					image_slider->setListener(&imgSliderListener);
					image_slider->setTooltip(Tooltip(
					        target_name.substr(4, target_name.length() - 4),
					        kTooltipPos, c, tooltipPanel));
					targetVector.push_back(image_slider);
					addWidget(image_slider);
				}
			}
		}
	}
}

void PoseTargetPanel::resetTargetValues(const string currentTarget)
{
	for(vector<TargetSlider *>::const_iterator targetvector_it = targetVector.begin();
	    targetvector_it != targetVector.end(); targetvector_it++) {
		if((*targetvector_it)->getTargetName() != currentTarget) {
			(*targetvector_it)->setSliderValue(0);
		}
	}
}
