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


static std::array<string, 10> ageLabels = {
    "female_10",
    "female_30",
    "female_50",
    "female_70",
    "female_90",
    "male_10",
    "male_30",
    "male_50",
    "male_70",
    "male_90"
};

static std::array<string, 4> muscleSizeLabels = {
    "skinny_nomuscle",
    "big_nomuscle",
    "skinny_muscle",
    "big_muscle"
};

static std::array<string, 4> breastLabels = {
    "cone_little",
    "cone_big",
    "sphere_little",
    "sphere_big"
};

static std::array<string, 4> shapeLabels = {
    "brevilinear_vshape",
    "brevilinear_peershape",
    "longilinear_vshape",
    "longilinear_peershape"
};


SelectorListener::SelectorListener()
    : AbstractListener()
    , oldPos(0, 0)
{
}

SelectorListener::~SelectorListener()
{
}

bool SelectorListener::mouseOver(const glm::ivec2 & inMousePos, mhgui::Component * source)
{
	return false;
}

bool SelectorListener::mouseOut(const glm::ivec2 & inMousePos, mhgui::Component * source)
{
	return false;
}

bool SelectorListener::mouseDragged(const glm::ivec2 & inMousePos, mhgui::Component * source)
{
	int xDist = abs(oldPos.x - inMousePos.x);
	int yDist = abs(oldPos.y - inMousePos.y);
	
	mhgui::Selector * selectorSource = dynamic_cast<mhgui::Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	
	if(xDist > 3 || yDist > 3) {
		oldPos = inMousePos;
		calcWidgetTargets(*selectorSource, inMousePos);
	}
	
	return true;
}

bool SelectorListener::mouseWheel(const glm::ivec2 & inMousePos, int inButton, mhgui::Component * source)
{
	return false;
}

bool SelectorListener::mousePressed(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	return true;
}

bool SelectorListener::mouseReleased(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	oldPos = inMousePos;
	
	mhgui::Selector * selectorSource = dynamic_cast<mhgui::Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	calcWidgetTargets(*selectorSource, inMousePos);
	
	g_mesh.calcNormals();
	
	return true;
}

bool SelectorListener::keyType(unsigned char key, mhgui::Component * source)
{
	return false;
}

void SelectorListener::calcWidgetTargets(mhgui::Selector & selectorSource, glm::ivec2 inMousePos)
{
	
	switch(selectorSource.getID()) {
	case kAge:
		g_global.m_comp.m_kAge = inMousePos;
		ageDists = selectorSource.getDists();
		break;
	case kMuscleSize:
		g_global.m_comp.m_kMuscleSize = inMousePos;
		muscleSizeDists = selectorSource.getDists();
		break;
	case kBreast:
		g_global.m_comp.m_kBreast = inMousePos;
		breastDists = selectorSource.getDists();
		break;
	case kShape:
		g_global.m_comp.m_kShape = inMousePos;
		shapeDists = selectorSource.getDists();
		break;
	}
	
	calcWidgetTargetsFOO();
}

void SelectorListener::calcWidgetTargetsFOO()
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	
	// std::cout << "--------------------------" << std::endl;
	for(const float & di_it : ageDists) {
		if(i < ageLabels.size()) {
			string tmpTargetName("ages/" + ageLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, di_it);
		}
	}
	
	for(const float & ms_it : muscleSizeDists) {
		
		i = 0;
		
		for(const float & di_it : ageDists) {
			if(j < muscleSizeLabels.size() && i < ageLabels.size()) {
				string tmpTargetName("muscleSize/" + ageLabels[i] + "_" +
				                     muscleSizeLabels[j] + ".target");
				float  tmpTargetValue = di_it * ms_it;
				
				g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
				
				// breast widget
				
				k = 0;
				if(i <= 4) {
					
					for(const float & br_it : breastDists) {
						
						if(k < breastLabels.size()) {
							string tmpTargetName(
							    "breast/" + ageLabels[i] + "_" +
							    muscleSizeLabels[j] + "_" +
							    breastLabels[k] + ".target");
							float tmpTargetValue =
							    di_it * ms_it * br_it;
							
							if(tmpTargetValue > 0) {
								//log_info("{} {}", tmpTargetName, tmpTargetValue);
							}
							
							g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
						}
						k++;
					}
				}
			}
			i++;
		}
		j++;
	}
	
	i = 0;
	
	for(const float & sh_it : shapeDists) {
		if(i < shapeLabels.size()) {
			string tmpTargetName("shapes/" + shapeLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, sh_it);
		}
	}
}






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
	selector->setPoints(2, 5);
	selector->setListener(&selectorListener);
	selector->setShowLines(false);
	m_age = selector;
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
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	m_muscleSize = selector;
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
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	m_breast = selector;
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
	selector->setPoints(2, 2);
	selector->setListener(&selectorListener);
	m_shape = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kShape: {} {} {} {}", r.getX(), r.getY(), r.getWidth(), r.getHeight());
	}
	
	updateUi();
}

void CharacterSettingPanel::resetSlidersValues()
{
	m_age->setCursorPos(glm::ivec2(96, 52));
	m_muscleSize->setCursorPos(glm::ivec2(96, 52));
	m_breast->setCursorPos(glm::ivec2(96, 52));
	m_shape->setCursorPos(glm::ivec2(96, 52));
	
	selectorListener.ageDists.clear();
	selectorListener.muscleSizeDists.clear();
	selectorListener.breastDists.clear();
	selectorListener.shapeDists.clear();
}

void CharacterSettingPanel::calcSelectorValues(Selector * sel)
{
	glm::ivec2 foo = InvalidPoint;

	switch(sel->getID()) {
	case kAge:
		foo = g_global.m_comp.m_kAge;
		selectorListener.ageDists = sel->getDists();
		break;
	case kBreast:
		foo = g_global.m_comp.m_kBreast;
		selectorListener.breastDists = sel->getDists();
		break;
	case kMuscleSize:
		foo = g_global.m_comp.m_kMuscleSize;
		selectorListener.muscleSizeDists = sel->getDists();
		break;
	case kShape:
		foo = g_global.m_comp.m_kShape;
		selectorListener.shapeDists = sel->getDists();
		break;
	}
	
	if(foo == InvalidPoint) {
		sel->setCursorPos(glm::ivec2(96, 52));
	} else {
		sel->setCursorPosFromMousePoint(foo);
	}
}

void CharacterSettingPanel::updateUi()
{
	calcSelectorValues(m_age);
	calcSelectorValues(m_breast);
	calcSelectorValues(m_muscleSize);
	calcSelectorValues(m_shape);
}
