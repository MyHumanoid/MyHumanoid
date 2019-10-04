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
 *  File   : SelectorListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "SelectorListener.h"
#include "ComponentID.h"
#include "Global.h"
#include <gui/Selector.h>
#include "log/log.h"

#include <assert.h>
#include <stdio.h>

using namespace Animorph;
using namespace std;

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

bool SelectorListener::mouseOver(const glm::ivec2 & inMousePos, Component * source)
{
	return false;
}

bool SelectorListener::mouseOut(const glm::ivec2 & inMousePos, Component * source)
{
	return false;
}

bool SelectorListener::mouseDragged(const glm::ivec2 & inMousePos, Component * source)
{
	int xDist = abs(oldPos.x - inMousePos.x);
	int yDist = abs(oldPos.y - inMousePos.y);

	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?

	g_global.setFuzzyValue(selectorSource->getID(), inMousePos);

	if(xDist > 3 || yDist > 3) {
		oldPos = inMousePos;
		calcWidgetTargets(*selectorSource);
	}

	return true;
}

bool SelectorListener::mouseWheel(const glm::ivec2 & inMousePos, int inButton, Component * source)
{
	return false;
}

bool SelectorListener::mousePressed(const glm::ivec2 & inMousePos, int button, Component * source)
{
	return true;
}

bool SelectorListener::mouseReleased(const glm::ivec2 & inMousePos, int button, Component * source)
{
	oldPos = inMousePos;

	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?

	g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	calcWidgetTargets(*selectorSource);

	g_mesh.calcNormals();

	return true;
}

bool SelectorListener::keyType(unsigned char key, Component * source)
{
	return false;
}

void SelectorListener::calcWidgetTargets(Selector & selectorSource)
{

	switch(selectorSource.getID()) {
	case kComponentID_CharacterSettingPanel_Age:
		ageDists = selectorSource.getDists();
		break;

	case kComponentID_CharacterSettingPanel_MuscleSize:
		muscleSizeDists = selectorSource.getDists();
		break;

	case kComponentID_CharacterSettingPanel_Breast:
		breastDists = selectorSource.getDists();
		break;

	case kComponentID_CharacterSettingPanel_Shape:
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
