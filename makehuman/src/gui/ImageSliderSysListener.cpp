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
 *  File   : ImageSliderSysListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/ImageSliderSysListener.h"
#include "gui/CGUtilities.h"
#include "gui/GLUTWrapper.h"
#include "gui/ImageSlider.h"

#include <cassert>
#include <cstdio>
#include <iostream>

using namespace Animorph;
using namespace std;

namespace mhgui
{

ImageSliderSysListener::ImageSliderSysListener()
    : AbstractListener()
{
}

ImageSliderSysListener::~ImageSliderSysListener() {}

bool ImageSliderSysListener::mouseOver(const Point &inMousePos,
                                       Component *source)
{
	cgutils::redisplay();
	return false;
}

bool ImageSliderSysListener::mouseOut(const Point &inMousePos,
                                      Component *source)
{
	cgutils::redisplay();
	return false;
}

bool ImageSliderSysListener::mouseWheel(const Point &inMousePos, int inButton,
                                        Component *source)
{
#ifdef GLUT_WHEEL_UP
	ImageSlider *imgSliderSource =
	    dynamic_cast<ImageSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really an ImageSlider object?

	int value = 1;

	if (inButton == GLUT_WHEEL_UP) {
		imgSliderSource->increaseValue(value);
		cgutils::redisplay();
		return true;
	} else if (inButton == GLUT_WHEEL_DOWN) {
		imgSliderSource->decreaseValue(value);
		cgutils::redisplay();
		return true;
	}
#endif

	return false;
}

bool ImageSliderSysListener::mouseDragged(const Point &inMousePos,
                                          Component *source)
{
	ImageSlider *imgSliderSource =
	    dynamic_cast<ImageSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really an ImageSlider object?

	if (inMousePos.getX() > imgSliderSource->getOldMouseX() &&
	    imgSliderSource->getSliderValue() < imgSliderSource->getMaxValue()) {
		int value = inMousePos.getX() - imgSliderSource->getOldMouseX();
		imgSliderSource->increaseValue(value);
		imgSliderSource->setOldMouseX(inMousePos.getX());
		cgutils::redisplay();
	} else if (inMousePos.getX() < imgSliderSource->getOldMouseX() &&
	           imgSliderSource->getSliderValue() >
	               imgSliderSource->getMinValue()) {
		int value = imgSliderSource->getOldMouseX() - inMousePos.getX();
		imgSliderSource->decreaseValue(value);
		imgSliderSource->setOldMouseX(inMousePos.getX());
		cgutils::redisplay();
	}

	return true;
}

bool ImageSliderSysListener::mousePressed(const Point &inMousePos, int button,
                                          Component *source)
{
	ImageSlider *imgSliderSource =
	    dynamic_cast<ImageSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really an ImageSlider object?
	imgSliderSource->setActive(true);
	imgSliderSource->setOldMouseX(inMousePos.getX());
	return true;
}

bool ImageSliderSysListener::mouseReleased(const Point &inMousePos, int button,
                                           Component *source)
{
	ImageSlider *imgSliderSource =
	    dynamic_cast<ImageSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really an ImageSlider object?

	if (button == GLUT_LEFT_BUTTON) {
		if (imgSliderSource->isActive()) {
			imgSliderSource->setActive(false);
			imgSliderSource->setOldMouseX(0);
			return true;
		}
	} else if (button == GLUT_RIGHT_BUTTON) {
		if (imgSliderSource->isActive()) {
			imgSliderSource->setActive(false);
			imgSliderSource->setSliderValue(0);
			imgSliderSource->setOldMouseX(0);
			return true;
		}
	}
	return false;
}

bool ImageSliderSysListener::keyType(unsigned char key, Component *source)
{
	return false;
}

} // namespace mhgui
