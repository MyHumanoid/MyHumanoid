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
 *  File   : UtilitiesSliderListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "UtilitiesSliderListener.h"
#include "BsPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include "TargetSlider.h"
#include <animorph/Mesh.h>
#include <assert.h>
#include <gui/CGUtilities.h>
#include <gui/GLUTWrapper.h>
#include <gui/ImageSlider.h>
#include <stdio.h>

using namespace Animorph;
using namespace std;

const static float kPoseThreshold = 0.04;

UtilitiesSliderListener::UtilitiesSliderListener()
    : AbstractListener()
    , lastTargetName()
{
}

UtilitiesSliderListener::~UtilitiesSliderListener() {}

bool UtilitiesSliderListener::mouseOver(const Point &inMousePos,
                                        Component *source)
{
	return false;
}

bool UtilitiesSliderListener::mouseOut(const Point &inMousePos,
                                       Component *source)
{
	return false;
}

bool UtilitiesSliderListener::mouseWheel(const Point &inMousePos, int inButton,
                                         Component *source)
{
	return true;
}

bool UtilitiesSliderListener::mouseDragged(const Point &inMousePos,
                                           Component *source)
{
	TargetSlider *imgSliderSource =
	    dynamic_cast<TargetSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really a TargetSlider object?

	Animation *animation = g_global.getAnimation();

	if (imgSliderSource->getID() == kComponentID_UtilitiesPanel_Interpolations) {
		animation->setInterpNumber(
		    (unsigned int)(imgSliderSource->getSliderValue()));
	} else if (imgSliderSource->getID() ==
	           kComponentID_UtilitiesPanel_FrameRate) {
		animation->setFrameRate((unsigned int)(imgSliderSource->getSliderValue()));
	}

	return true;
}

bool UtilitiesSliderListener::mousePressed(const Point &inMousePos, int button,
                                           Component *source)
{
	return false;
}

bool UtilitiesSliderListener::mouseReleased(const Point &inMousePos, int button,
                                            Component *source)
{

	return false;
}

bool UtilitiesSliderListener::keyType(unsigned char key, Component *source)
{
	return false;
}
