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
 *  File   : ImageListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "ImageListener.h"
#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include "util.h"
#include <animorph/DirectoryList.h>
#include <animorph/ObjExporter.h>
#include <assert.h>
#include <gui/CGUtilities.h>
#include <gui/Camera.h>
#include <gui/GLUTWrapper.h>
#include <gui/Image.h>
#include <gui/Window.h>
#include <iostream>

using namespace std;

const static Point kBottomPosition1(0, 446);
const static Point kBottomPosition2(0, 464);
const static Point kBottomPosition3(0, 496);
const static Point kBottomPosition4(0, 336);
const static Point kBottomPosition5(0, 144);
const static Point kBottomPosition6(0, 400);
const static Point kBottomPosition7(0, 472);

ImageListener::ImageListener()
        : AbstractListener()
{
}

ImageListener::~ImageListener()
{
}

bool ImageListener::mouseOver(const Point & inMousePos, Component * source)
{
	Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(Color(1, 0, 0, 0.5));

	return false;
}

bool ImageListener::mouseOut(const Point & inMousePos, Component * source)
{
	Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return false;
}

bool ImageListener::mouseDragged(const Point & inMousePos, Component * source)
{

	Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return true;
}

bool ImageListener::mouseWheel(const Point & inMousePos, int inButton, Component * source)
{
	return false;
}

bool ImageListener::mousePressed(const Point & inMousePos, int button, Component * source)
{
	if(button == GLUT_LEFT_BUTTON) {
		Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?
		imgSource->setOverlayRectangle(Color(1, 1, 1, 0.5));

		return true;
	}
	return false;
}

bool ImageListener::mouseReleased(const Point & inMousePos, int button, Component * source)
{

	if(button == GLUT_LEFT_BUTTON) {
		Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?

		if(imgSource->getAbsoluteRect().isHitBy(inMousePos)) {

			// DialogPanel *dialogPanel;// = new DialogPanel (imgSource->getID ());

			Window & mainWindow = *g_mainWindow;
			imgSource->setOverlayRectangle(false);

			switch(imgSource->getID()) {
			// toolbar buttons
			// utilitybar buttons
			default:
				cerr << "Unknown widget handler!" << imgSource->getIDAsString()
				     << endl;
			}

			return true;
		}
	}

	return false;
}

bool ImageListener::keyType(unsigned char key, Component * source)
{
	return false;
}

void ImageListener::hidePanels(const Modes currentMode)
{
	if(currentMode == CHARACTER_SETTING) {
		hideCharacterSetting();
	}
}

void ImageListener::hideCharacterSetting()
{
	Window & mainWindow = *g_mainWindow;
	Panel *  p          = mainWindow.getPanel(kComponentID_CharacterSettingPanel);
	mainWindow.removePanel(p);
	delete p;
}

// show:

void ImageListener::showCharacterSetting()
{
	Window & mainWindow = *g_mainWindow;

	CharacterSettingPanel * characterSettingPanel = new CharacterSettingPanel();
	mainWindow.addPanel(characterSettingPanel);
	characterSettingPanel->createWidgets();
	characterSettingPanel->show_all();
}
