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
#include "ImageListener.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>

using namespace std;
using namespace Animorph;

CharacterSettingPanel::CharacterSettingPanel()
    : Panel(kComponentID_CharacterSettingPanel, Rect(0, 16, 192, 580))
{
	setAlignment(VERTICAL);
}

CharacterSettingPanel::~CharacterSettingPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	for_each(selectorVector.begin(), selectorVector.end(),
	         deleteFunctor<Selector *>());
}

void CharacterSettingPanel::createWidgets()
{
	Window &mainWindow = *g_mainWindow;

	Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color c(1.0, 0.0, 0.0);
	const Point kTooltipPos(70, 12);
	// --------------------------------------------------------------------------
	//  Image *image = new Image(kComponentID_Dummy,
	//                    searchPixmapFile ("ui/dummy_image.png"),
	//                    Rect(0,0,192,10));
	//  image->setKillMouseDrag(true);
	//  imageVector.push_back(image);
	//  addWidget (image);
	// --------------------------------------------------------------------------

	selector = new Selector(kComponentID_CharacterSettingPanel_Age,
	                        searchPixmapFile("ui/age_selector.png"),
	                        Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/age_cursor.png"));
	selector->setPoints(2, 5);
	selector->setListener(&selectorListener);
	selector->setShowLines(false);
	selectorVector.push_back(selector);
	addWidget(selector);
	m_age = selector;

	Point *cursorPosition = Global::instance().getFuzzyValue(selector->getID());
	if (cursorPosition == NULL) {
		selector->setCursorPos(Point(96, 52));
	} else {
		selector->setCursorPosFromMousePoint(*cursorPosition);
	}
	selectorListener.ageDists = selector->getDists();
	// --------------------------------------------------------------------------
	//  image = new Image(kComponentID_Dummy,
	//                    searchPixmapFile ("ui/dummy_image.png"),
	//                    Rect(0,0,192,10));
	//  image->setKillMouseDrag(true);
	//  imageVector.push_back(image);
	//  addWidget (image);
	// -------------------------------------------------------------------------
	selector = new Selector(kComponentID_CharacterSettingPanel_MuscleSize,
	                        searchPixmapFile("ui/muscle_size_selector.png"),
	                        Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/muscle_size_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);
	m_muscleSize = selector;

	cursorPosition = Global::instance().getFuzzyValue(selector->getID());
	if (cursorPosition == NULL) {
		selector->setCursorPos(Point(96, 52));
	} else {
		selector->setCursorPosFromMousePoint(*cursorPosition);
	}
	selectorListener.muscleSizeDists = selector->getDists();
	// --------------------------------------------------------------------------
	//  image = new Image(kComponentID_Dummy,
	//                    searchPixmapFile ("ui/dummy_image.png"),
	//                    Rect(0,0,192,10));
	//  image->setKillMouseDrag(true);
	//  imageVector.push_back(image);
	//  addWidget (image);
	// -------------------------------------------------------------------------
	selector = new Selector(kComponentID_CharacterSettingPanel_Breast,
	                        searchPixmapFile("ui/breast_selector.png"),
	                        Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/breast_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);
	m_breast = selector;

	cursorPosition = Global::instance().getFuzzyValue(selector->getID());
	if (cursorPosition == NULL) {
		selector->setCursorPos(Point(96, 52));
	} else {
		selector->setCursorPosFromMousePoint(*cursorPosition);
	}
	selectorListener.breastDists = selector->getDists();
	// --------------------------------------------------------------------------
	//  image = new Image(kComponentID_Dummy,
	//                    searchPixmapFile ("ui/dummy_image.png"),
	//                    Rect(0,0,192,10));
	//  image->setKillMouseDrag(true);
	//  imageVector.push_back(image);
	//  addWidget (image);
	// -------------------------------------------------------------------------
	selector = new Selector(kComponentID_CharacterSettingPanel_Shape,
	                        searchPixmapFile("ui/shape_selector.png"),
	                        Rect(0, 0, 192, 104));
	selector->setCursorTexture(searchPixmapFile("ui/shape_cursor.png"));
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	selectorVector.push_back(selector);
	addWidget(selector);
	m_shape = selector;

	cursorPosition = Global::instance().getFuzzyValue(selector->getID());
	if (cursorPosition == NULL) {
		selector->setCursorPos(Point(96, 52));
	} else {
		selector->setCursorPosFromMousePoint(*cursorPosition);
	}
	selectorListener.shapeDists = selector->getDists();
}

void CharacterSettingPanel::resetSlidersValues()
{
	for (vector<Selector *>::const_iterator selectorVector_it =
	         selectorVector.begin();
	     selectorVector_it != selectorVector.end(); selectorVector_it++) {
		(*selectorVector_it)->setCursorPos(Point(96, 52));
	}

	selectorListener.ageDists.clear();
	selectorListener.muscleSizeDists.clear();
	selectorListener.breastDists.clear();
	selectorListener.shapeDists.clear();
}

void CharacterSettingPanel::calcSelectorValues(uint32_t index)
{
	Point *cursorPosition = Global::instance().getFuzzyValue(index);

	for (vector<Selector *>::iterator sel_it = selectorVector.begin();
	     sel_it != selectorVector.end(); sel_it++) {
		Selector *tmp = (*sel_it);
		if (tmp->getID() == index) {
			if (cursorPosition == NULL) {
				tmp->setCursorPos(Point(96, 52));
			} else {
				tmp->setCursorPosFromMousePoint(*cursorPosition);
			}

			switch (index) {
			case kComponentID_CharacterSettingPanel_Age:
				selectorListener.ageDists = tmp->getDists();
				break;

			case kComponentID_CharacterSettingPanel_Breast:
				selectorListener.breastDists = tmp->getDists();
				break;

			case kComponentID_CharacterSettingPanel_MuscleSize:
				selectorListener.muscleSizeDists = tmp->getDists();
				break;

			case kComponentID_CharacterSettingPanel_Shape:
				selectorListener.shapeDists = tmp->getDists();
				break;
			}

			break;
		}
	}
}
