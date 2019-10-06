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
 *  File   : CharacterSettingPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "util.h"
#include "gui/AbstractListener.h"
#include <gui/Image.h>
#include <gui/Panel.h>
#include <gui/Selector.h>
#include <vector>



struct Grid
{
	int rows = 2;
	int cols = 2;
	
	float maxValue;
	float cellRatio;
	
	// from left-bottom point to right-up point
	std::vector<glm::ivec2> points;
	
	void calcPoints(glm::ivec2 size, int inRows, int inCols);
	std::vector<float> calculateDists(glm::ivec2 cursorPos) const;
};

struct Grids {
	Grid ageGrid;
	Grid muscleSizeGrid;
	Grid breastGrid;
	Grid shapeGrid;
	
	std::vector<float> ageDists;
	std::vector<float> muscleSizeDists;
	std::vector<float> breastDists;
	std::vector<float> shapeDists;
	
	void calcWidgetTargetsFOO() const;
};



class SelectorListener : public mhgui::AbstractListener
{
public:
	void calcWidgetTargets(mhgui::Selector & selectorSource, glm::ivec2 inMousePos);
	void calcWidgetTargetsFOO();
	
	glm::ivec2 oldPos;
	
public:
	SelectorListener();
	virtual ~SelectorListener();
	
	virtual bool mousePressed(const glm::ivec2 & inMousePos, int inButton, mhgui::Component * source);
	virtual bool mouseReleased(const glm::ivec2 & inMousePos, int inButton, mhgui::Component * source);
	virtual bool mouseDragged(const glm::ivec2 & inMousePos, mhgui::Component * source);
	
	Grids grids;
};




class CharacterSettingPanel : public mhgui::Panel
{
public:
	CharacterSettingPanel();
	virtual ~CharacterSettingPanel();

	void createWidgets();

	// void resetTargetValues(const string currentTarget = "");

	void resetSlidersValues();
	void calcSelectorValues(mhgui::Selector *sel);
	void updateUi();

private: // intentionally not implemented
	CharacterSettingPanel(const CharacterSettingPanel &);
	CharacterSettingPanel & operator=(const CharacterSettingPanel &);

public:
	std::vector<mhgui::Image *>    imageVector;
	
	SelectorListener   lis;
	
	
	mhgui::Selector * m_age;
	mhgui::Selector * m_muscleSize;
	mhgui::Selector * m_breast;
	mhgui::Selector * m_shape;
	
};
