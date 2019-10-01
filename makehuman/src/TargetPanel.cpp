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
 *  File   : TargetPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "TargetPanel.h"
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

const static string kFilePrefixTarget(".target");
const static string kFilePrefixPNG(".png");
const static int    kMaxTargetsNumber = 44;

TargetPanel::TargetPanel(const string & category, const Rect & rect)
        : MultiPanel(kComponentID_TargetPanel, rect)
        , category(category)
{
	setAlignment(FREE);
	setRightAnchor(true);
}

TargetPanel::~TargetPanel()
{
	for_each(targetVector.begin(), targetVector.end(), deleteFunctor<TargetSlider *>());
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	for_each(begin(), end(), deleteFunctor<Panel *>());
}

PageTargetPanel * TargetPanel::newPagePanel()
{
	PageTargetPanel * page;

	page = new PageTargetPanel(category, getRect());

	return page;
}

void TargetPanel::createWidgets()
{
	int numTargetsInPage = 1;

	Window & mainWindow = *g_mainWindow;

	Panel * tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	Mesh * mesh = g_global.getMesh();
	assert(mesh);

	const TargetMap & targetmap = mesh->targets();

	const Color c(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);

	BodySettings bodyset = mesh->getBodySettings();

	TargetSlider * image_slider;

	PageTargetPanel * page = newPagePanel();

	if(category == applied_target_list) {
		Image * close_image =
		        new Image(kComponentID_CloseTargetPanel,
		                  searchPixmapFile("ui/close_panel.png"), Rect(0, 0, 32, 32));
		close_image->setListener(&imgListener);
		close_image->setTooltip(Tooltip("Close panel", kTooltipPos, c, tooltipPanel));
		imageVector.push_back(close_image);
		page->addWidget(close_image);

		for(BodySettings::iterator bodyset_it = bodyset.begin();
		    bodyset_it != bodyset.end(); bodyset_it++) {

			const string & target_name((*bodyset_it).first);
			if(target_name.find("ages", 0) != string::npos ||
			   target_name.find("breast", 0) != string::npos ||
			   target_name.find("muscleSize", 0) != string::npos ||
			   target_name.find("shapes", 0) != string::npos) {
				continue;
			}

			if(numTargetsInPage++ > kMaxTargetsNumber) {
				numTargetsInPage = 0;
				addPanel(page);
				page = newPagePanel();
			}

			float  target_value = (*bodyset_it).second;
			string target_image(target_name);
			target_image.replace(target_image.length() - kFilePrefixTarget.length(),
			                     kFilePrefixTarget.length(), kFilePrefixPNG);

			image_slider =
			        new TargetSlider(kComponentID_TargetPanel_Target,
			                         searchPixmapFile("tgimg/" + target_image),
			                         target_name, Rect(0, 0, 32, 32 + 10), 0.0, 1.0);

			image_slider->setOverlayTexture(searchPixmapFile("tgimg/" + target_image));
			image_slider->setOverlayMultiplier(4);
			image_slider->setSliderValue(target_value);
			image_slider->setListener(&imgSliderListener);
			image_slider->setTooltip(
			        Tooltip(target_name, kTooltipPos, c, tooltipPanel));
			page->targetVector.push_back(image_slider);
			page->addWidget(image_slider);
		}
	} else {
		Image * close_image =
		        new Image(kComponentID_CloseTargetPanel,
		                  searchPixmapFile("ui/close_panel.png"), Rect(0, 0, 32, 32));
		close_image->setListener(&imgListener);
		close_image->setTooltip(Tooltip("Close panel", kTooltipPos, c, tooltipPanel));
		imageVector.push_back(close_image);
		page->addWidget(close_image);

		for(TargetMap::const_iterator targetmap_it = targetmap.begin();
		    targetmap_it != targetmap.end(); targetmap_it++) {
			const string & target_name((*targetmap_it).first);

			string::size_type loc = target_name.find("/", 0);
			if(loc == string::npos)
				continue;
			else {
				string sub = target_name.substr(0, loc);

				if(sub == category) {
					if(numTargetsInPage++ > kMaxTargetsNumber) {
						numTargetsInPage = 0;
						addPanel(page);
						page = newPagePanel();
					}
					// remove ".target"
					string target_image(target_name);
					target_image.replace(
					        target_image.length() - kFilePrefixTarget.length(),
					        kFilePrefixTarget.length(), kFilePrefixPNG);

					image_slider = new TargetSlider(
					        kComponentID_TargetPanel_Target,
					        searchPixmapFile("tgimg/" + target_image),
					        target_name, Rect(0, 0, 32, 42), 0.0, 1.0);

					image_slider->setOverlayTexture(
					        searchPixmapFile("tgimg/" + target_image));
					image_slider->setOverlayMultiplier(4);
					float target_value = bodyset[target_name];
					image_slider->setSliderValue(target_value);

					image_slider->setListener(&imgSliderListener);
					image_slider->setTooltip(
					        Tooltip(target_name, kTooltipPos, c, tooltipPanel));
					page->targetVector.push_back(image_slider);
					page->addWidget(image_slider);
				}
			}
		}
	}
	addPanel(page);

	if(numPages > 1) {
		prevPage = new Image(
		        kComponentID_PrevTargetPanel, searchPixmapFile("ui/page_prev.png"),
		        Rect(getSize().getWidth() - 70, getSize().getHeight() - 32, 32, 32));
		prevPage->setListener(&imgListener);
		prevPage->setTooltip(Tooltip("Prev page", kTooltipPos, c, tooltipPanel));
		addWidget(prevPage);
		imageVector.push_back(prevPage);

		nextPage = new Image(
		        kComponentID_NextTargetPanel, searchPixmapFile("ui/page_next.png"),
		        Rect(getSize().getWidth() - 35, getSize().getHeight() - 32, 32, 32));
		nextPage->setListener(&imgListener);
		nextPage->setTooltip(Tooltip("Next page", kTooltipPos, c, tooltipPanel));
		addWidget(nextPage);
		imageVector.push_back(nextPage);

		checkControlsVisibility();
	}
}

void TargetPanel::resetTargetValues(const string currentTargetName)
{
	for(vector<Panel *>::iterator pagepanel_it = begin(); pagepanel_it != end();
	    pagepanel_it++) {

		((PageTargetPanel *)(*pagepanel_it))->resetTargetValues(currentTargetName);
	}
}
