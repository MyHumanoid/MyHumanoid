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

SelectorListener::SelectorListener()
        : AbstractListener()
        , ageLabels()
        , muscleSizeLabels()
        , breastLabels()
        , shapeLabels()
        , oldPos(0, 0)
{
	ageLabels.push_back("female_10");
	ageLabels.push_back("female_30");
	ageLabels.push_back("female_50");
	ageLabels.push_back("female_70");
	ageLabels.push_back("female_90");
	ageLabels.push_back("male_10");
	ageLabels.push_back("male_30");
	ageLabels.push_back("male_50");
	ageLabels.push_back("male_70");
	ageLabels.push_back("male_90");

	muscleSizeLabels.push_back("skinny_nomuscle");
	muscleSizeLabels.push_back("big_nomuscle");
	muscleSizeLabels.push_back("skinny_muscle");
	muscleSizeLabels.push_back("big_muscle");

	breastLabels.push_back("cone_little");
	breastLabels.push_back("cone_big");
	breastLabels.push_back("sphere_little");
	breastLabels.push_back("sphere_big");

	shapeLabels.push_back("brevilinear_vshape");
	shapeLabels.push_back("brevilinear_peershape");
	shapeLabels.push_back("longilinear_vshape");
	shapeLabels.push_back("longilinear_peershape");
}

SelectorListener::~SelectorListener()
{
}

bool SelectorListener::mouseOver(const Point & inMousePos, Component * source)
{
	return false;
}

bool SelectorListener::mouseOut(const Point & inMousePos, Component * source)
{
	return false;
}

bool SelectorListener::mouseDragged(const Point & inMousePos, Component * source)
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

bool SelectorListener::mouseWheel(const Point & inMousePos, int inButton, Component * source)
{
	return false;
}

bool SelectorListener::mousePressed(const Point & inMousePos, int button, Component * source)
{
	return true;
}

bool SelectorListener::mouseReleased(const Point & inMousePos, int button, Component * source)
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
	vector<float>::const_iterator di_end = ageDists.end();
	
	for(const float & di_it : ageDists) {
	//for(vector<float>::const_iterator di_it = ageDists.begin(); di_it != di_end; di_it++) {
		if(i < ageLabels.size()) {
			string tmpTargetName("ages/" + ageLabels[i++] + ".target");

			// if((*di_it) > 0)
			//{
			//  std::cout << tmpTargetName << " " << (*di_it) << std::endl;
			//}

			g_mesh.doMorph(tmpTargetName, di_it);
		}
	}

	vector<float>::const_iterator ms_end = muscleSizeDists.end();
	vector<float>::const_iterator br_end = breastDists.end();
	
	for(const float & ms_it : muscleSizeDists) {
	//for(vector<float>::const_iterator ms_it = muscleSizeDists.begin(); ms_it != ms_end;
	//    ms_it++) {
	
		i = 0;
		
		for(const float & di_it : ageDists) {
		//for(vector<float>::const_iterator di_it = ageDists.begin(); di_it != di_end;
		//    di_it++) {
			if(j < muscleSizeLabels.size() && i < ageLabels.size()) {
				string tmpTargetName("muscleSize/" + ageLabels[i] + "_" +
				                     muscleSizeLabels[j] + ".target");
				float  tmpTargetValue = di_it * ms_it;

				// if(tmpTargetValue > 0)
				//{
				//  std::cout << tmpTargetName << " " << (*di_it) << " " << (*ms_it)
				//  << " " << tmpTargetValue << std::endl;
				//}

				g_mesh.doMorph(tmpTargetName, tmpTargetValue);

				// breast widget

				k = 0;
				if(i <= 4) {
					
					for(const float & br_it : breastDists) {
					//for(vector<float>::const_iterator br_it =
					//            breastDists.begin();
					//    br_it != br_end; br_it++) {
						
						
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

							g_mesh.doMorph(tmpTargetName,
							               tmpTargetValue);
						}
						k++;
					}
				}
			}
			i++;
		}
		j++;
	}

	i                                    = 0;
	
	for(const float & sh_it : shapeDists) {
	//vector<float>::const_iterator sh_end = shapeDists.end();
	//for(vector<float>::const_iterator sh_it = shapeDists.begin(); sh_it != sh_end; sh_it++) {
		if(i < shapeLabels.size()) {
			string tmpTargetName("shapes/" + shapeLabels[i++] + ".target");

			// if((*di_it) > 0)
			//{
			//  std::cout << tmpTargetName << " " << (*sh_it) << std::endl;
			//}

			g_mesh.doMorph(tmpTargetName, sh_it);
		}
	}
	// std::cout << "--------------------------" << std::endl;
}
