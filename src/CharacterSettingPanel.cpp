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
#include <assert.h>
#include <stdio.h>

#include <gui/Rect.h>
#include <gui/Selector.h>
#include <gui/Window.h>

#include "ComponentID.h"
#include "Global.h"
#include "log/log.h"

using mhgui::Rect;
using mhgui::Image;
using mhgui::Selector;




// ================================================================================================


// ================================================================================================

SelectorListener::SelectorListener()
    : AbstractListener()
    , oldPos(0, 0)
{
}

SelectorListener::~SelectorListener()
{
}

bool SelectorListener::mouseDragged(const glm::ivec2 & inMousePos, mhgui::Component * source)
{
	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setCursorPosFromMousePoint(inMousePos);
	
	int xDist = abs(oldPos.x - inMousePos.x);
	int yDist = abs(oldPos.y - inMousePos.y);
	
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	
	if(xDist > 3 || yDist > 3) {
		oldPos = inMousePos;
		calcWidgetTargets(*selectorSource, inMousePos);
	}
	
	return true;
}

bool SelectorListener::mousePressed(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setCursorPosFromMousePoint(inMousePos);
	
	selectorSource->setActive(true);
	selectorSource->setClickConsumed(false);
	
	return true;
}

bool SelectorListener::mouseReleased(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	mhgui::Selector * selectorSource = dynamic_cast<mhgui::Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setActive(false);
	selectorSource->setClickConsumed(false);
	
	oldPos = inMousePos;
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	calcWidgetTargets(*selectorSource, inMousePos);
	
	g_mesh.calcNormals();
	
	return true;
}

void SelectorListener::calcWidgetTargets(mhgui::Selector & sel, glm::ivec2 inMousePos)
{
	
	switch(sel.getID()) {
	case kAge:
		g_global.m_comp.m_kAge = inMousePos;
		grids.agePos = sel.cursorPos;
		grids.ageDists = grids.ageGrid.calculateDists(grids.agePos);
		break;
	case kMuscleSize:
		g_global.m_comp.m_kMuscleSize = inMousePos;
		grids.muscleSizePos = sel.cursorPos;
		grids.muscleSizeDists = grids.muscleSizeGrid.calculateDists(grids.muscleSizePos);
		break;
	case kBreast:
		g_global.m_comp.m_kBreast = inMousePos;
		grids.breastPos = sel.cursorPos;
		grids.breastDists = grids.breastGrid.calculateDists(grids.breastPos);
		break;
	case kShape:
		g_global.m_comp.m_kShape = inMousePos;
		grids.shapePos = sel.cursorPos;
		grids.shapeDists = grids.shapeGrid.calculateDists(grids.shapePos);
		break;
	}
	
	grids.applyCompositeMorphTargets();
}


// ================================================================================================
// ================================================================================================


CharacterSettingPanel::CharacterSettingPanel()
        : Panel(kComponentID_CharacterSettingPanel, Rect(0, 16, 192, 580))
{
	setAlignment(VERTICAL);
}

CharacterSettingPanel::~CharacterSettingPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	
	delete m_age;
	delete m_muscleSize;
	delete m_breast;
	delete m_shape;
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
	
	mhgui::Selector *         selector;
	
	selector = new Selector(kAge,
	                        searchPixmapFile("ui/age_selector.png"), Rect(0, 0, 192, 104));
	
	lis.grids.ageGrid.calcPoints(selector->getSize(), 2, 5);
	
	selector->setListener(&lis);
	m_age = selector;
	addWidget(selector);
	
	{
		auto & r = selector->getRect();
		log_info("kAge: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
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
	
	lis.grids.muscleSizeGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_muscleSize = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kMuscleSize: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
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
	
	lis.grids.breastGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_breast = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kBreast: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
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
	
	lis.grids.shapeGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_shape = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kShape: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	
	updateUi();
}

void CharacterSettingPanel::resetSlidersValues()
{
	m_age->cursorPos = glm::ivec2(96, 52);
	m_muscleSize->cursorPos = glm::ivec2(96, 52);
	m_breast->cursorPos = glm::ivec2(96, 52);
	m_shape->cursorPos = glm::ivec2(96, 52);
	
	lis.grids.ageDists.clear();
	lis.grids.muscleSizeDists.clear();
	lis.grids.breastDists.clear();
	lis.grids.shapeDists.clear();
}

void CharacterSettingPanel::calcSelectorValues(Selector * sel)
{
	glm::ivec2 foo = InvalidPoint;

	switch(sel->getID()) {
	case kAge:
		foo = g_global.m_comp.m_kAge;
		break;
	case kBreast:
		foo = g_global.m_comp.m_kBreast;
		break;
	case kMuscleSize:
		foo = g_global.m_comp.m_kMuscleSize;
		break;
	case kShape:
		foo = g_global.m_comp.m_kShape;
		break;
	}
	
	if(foo == InvalidPoint) {
		sel->cursorPos = glm::ivec2(96, 52);
	} else {
		sel->setCursorPosFromMousePoint(foo);
	}
	
	switch(sel->getID()) {
	case kAge:
		lis.grids.agePos = sel->cursorPos;
		lis.grids.ageDists = lis.grids.ageGrid.calculateDists(lis.grids.agePos);
		break;
	case kBreast:
		lis.grids.breastPos = sel->cursorPos;
		lis.grids.breastDists = lis.grids.breastGrid.calculateDists(lis.grids.breastPos);
		break;
	case kMuscleSize:
		lis.grids.muscleSizePos = sel->cursorPos;
		lis.grids.muscleSizeDists = lis.grids.muscleSizeGrid.calculateDists(lis.grids.muscleSizePos);
		break;
	case kShape:
		lis.grids.shapePos = sel->cursorPos;
		lis.grids.shapeDists = lis.grids.shapeGrid.calculateDists(lis.grids.shapePos);
		break;
	}
	
}

void CharacterSettingPanel::updateUi()
{
	calcSelectorValues(m_age);
	calcSelectorValues(m_breast);
	calcSelectorValues(m_muscleSize);
	calcSelectorValues(m_shape);
}
