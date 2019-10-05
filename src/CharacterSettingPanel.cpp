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
 *  File   : CharacterSettingPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "CharacterSettingPanel.h"
#include "ComponentID.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Window.h>

#include "Global.h"
#include "log/log.h"

using mhgui::Rect;
using mhgui::Image;
using mhgui::Selector;

CharacterSettingPanel::CharacterSettingPanel()
        : Panel(kComponentID_CharacterSettingPanel, Rect(0, 16, 192, 580))
{
	setAlignment(VERTICAL);
}

CharacterSettingPanel::~CharacterSettingPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	for_each(selectorVector.begin(), selectorVector.end(), deleteFunctor<Selector *>());
}

void CharacterSettingPanel::createWidgets()
{
	const Color c(1.0, 0.0, 0.0);
	// --------------------------------------------------------------------------
	Image * image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                          Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// --------------------------------------------------------------------------

	selector = new Selector(kAge,
	                        searchPixmapFile("ui/age_selector.png"), Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/age_cursor.png"));
	selector->setPoints(2, 5);
	selector->setListener(&selectorListener);
	selector->setShowLines(false);
	selectorVector.push_back(selector);
	addWidget(selector);
	
	{
		auto & r = selector->getRect();
		log_info("kAge: {} {} {} {}", r.getX(), r.getY(), r.getWidth(), r.getHeight());
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector =
	        new Selector(kMuscleSize,
	                     searchPixmapFile("ui/muscle_size_selector.png"), Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/muscle_size_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kMuscleSize: {} {} {} {}", r.getX(), r.getY(), r.getWidth(), r.getHeight());
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector = new Selector(kBreast,
	                        searchPixmapFile("ui/breast_selector.png"), Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/breast_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kBreast: {} {} {} {}", r.getX(), r.getY(), r.getWidth(), r.getHeight());
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector = new Selector(kShape,
	                        searchPixmapFile("ui/shape_selector.png"), Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/shape_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kShape: {} {} {} {}", r.getX(), r.getY(), r.getWidth(), r.getHeight());
	}
	
	updateUi();
}

void CharacterSettingPanel::resetSlidersValues()
{
	for(vector<Selector *>::const_iterator selectorVector_it = selectorVector.begin();
	    selectorVector_it != selectorVector.end(); selectorVector_it++) {
		(*selectorVector_it)->setCursorPos(glm::ivec2(96, 52));
	}

	selectorListener.ageDists.clear();
	selectorListener.muscleSizeDists.clear();
	selectorListener.breastDists.clear();
	selectorListener.shapeDists.clear();
}

void CharacterSettingPanel::calcSelectorValues(uint32_t index)
{
	for(vector<Selector *>::iterator sel_it = selectorVector.begin();
	    sel_it != selectorVector.end(); sel_it++) {
		Selector * tmp = (*sel_it);
		if(tmp->getID() == index) {

			
			glm::ivec2 foo = InvalidPoint;

			switch(index) {
			case kAge:
				foo = g_global.m_comp.m_kAge;
				selectorListener.ageDists = tmp->getDists();
				break;
			case kBreast:
				foo = g_global.m_comp.m_kBreast;
				selectorListener.breastDists = tmp->getDists();
				break;
			case kMuscleSize:
				foo = g_global.m_comp.m_kMuscleSize;
				selectorListener.muscleSizeDists = tmp->getDists();
				break;
			case kShape:
				foo = g_global.m_comp.m_kShape;
				selectorListener.shapeDists = tmp->getDists();
				break;
			}
			
			if(foo == InvalidPoint) {
				tmp->setCursorPos(glm::ivec2(96, 52));
			} else {
				tmp->setCursorPosFromMousePoint(foo);
			}
			

			break;
		}
	}
}

void CharacterSettingPanel::updateUi()
{
	calcSelectorValues(kAge);
	calcSelectorValues(kBreast);
	calcSelectorValues(kMuscleSize);
	calcSelectorValues(kShape);
}
