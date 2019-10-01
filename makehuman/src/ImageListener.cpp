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
#include "BodyPanel.h"
#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "FacePanel.h"
#include "Global.h"
#include "HandsPanel.h"
#include "TargetPanel.h"
#include "TeethPanel.h"
#include "ToolbarPanel.h"
#include "util.h"
#include <animorph/DirectoryList.h>
#include <animorph/MathUtil.h>
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

ImageListener::~ImageListener() {}

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
			case kComponentID_ImageToolbarPanel_CharacterSettings: {
				if(g_global.getAppMode() == CHARACTER_SETTING)
					return true;

				hidePanels(g_global.getAppMode());

				if(g_global.getAppMode() != BODY_DETAILS &&
				   g_global.getAppMode() != BODY_SETTINGS) {
					Mesh * mesh = g_global.getMesh();
					assert(mesh);

					if(!g_morphMode) {
						g_morphMode = !g_morphMode;
						mesh->bodyDetailsMode();
					}
				}

				g_global.setAppMode(CHARACTER_SETTING);
				showCharacterSetting();
			} break;
			case kComponentID_ImageToolbarPanel_BodyDetailsRealistic: {
				if(g_global.getAppMode() == BODY_DETAILS)
					return true;

				hidePanels(g_global.getAppMode());

				if(g_global.getAppMode() != CHARACTER_SETTING &&
				   g_global.getAppMode() != BODY_SETTINGS) {
					Mesh * mesh = g_global.getMesh();
					assert(mesh);

					if(!g_morphMode) {
						g_morphMode = !g_morphMode;
						mesh->bodyDetailsMode();
					}
				}

				g_global.setAppMode(BODY_DETAILS);
				showBodyDetails();
			} break;
			case kComponentID_ImageToolbarPanel_MorphingList: {
				Panel * p = mainWindow.getPanel(kComponentID_TargetPanel);
				mainWindow.removePanel(p);
				delete p;

				if(g_global.getAppMode() == BODY_DETAILS ||
				   g_global.getAppMode() == CHARACTER_SETTING) {
					TargetPanel * targetPanel;

					int x       = mainWindow.getSize().getWidth() - 210;
					targetPanel = new TargetPanel(applied_target_list,
					                              Rect(x, 104, 160, 460));
					mainWindow.addPanel(targetPanel);
					targetPanel->createWidgets();
					targetPanel->show();
				}
			} break;
			// utilitybar buttons
			case kComponentID_CloseTargetPanel: {
				Panel * p = mainWindow.getPanel(kComponentID_TargetPanel);
				mainWindow.removePanel(p);
			} break;
			case kComponentID_NextTargetPanel: {
				MultiPanel * p = dynamic_cast<MultiPanel *>(
				        mainWindow.getPanel(kComponentID_TargetPanel));
				assert(p);
				p->pageNext();
				break;
			}
			case kComponentID_PrevTargetPanel: {
				MultiPanel * p = dynamic_cast<MultiPanel *>(
				        mainWindow.getPanel(kComponentID_TargetPanel));
				assert(p);
				p->pageBack();
				break;
			}
			default:
				cerr << "Unknown widget handler!" << imgSource->getIDAsString()
				     << endl;
			}

			return true;
		}
	}

	return false;
}

bool ImageListener::keyType(unsigned char key, Component * source) { return false; }

void ImageListener::hidePanels(const Modes currentMode)
{
	if(currentMode == POSES) {
		hidePoses();
	} else if(currentMode == BODY_DETAILS) {
		hideBodyDetails();
	} else if(currentMode == CHARACTER_SETTING) {
		hideCharacterSetting();
	}
}

void ImageListener::hideBodyDetails()
{
	Window & mainWindow = *g_mainWindow;
	Panel *  p          = mainWindow.getPanel(kComponentID_BodyPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_FacePanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TeethPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_HandsPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

void ImageListener::hideCharacterSetting()
{
	Window & mainWindow = *g_mainWindow;
	Panel *  p          = mainWindow.getPanel(kComponentID_CharacterSettingPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

void ImageListener::hidePoses()
{
	Window & mainWindow = *g_mainWindow;
	Panel *  p          = mainWindow.getPanel(kComponentID_BodyPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
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

void ImageListener::showBodyDetails()
{
	Window & mainWindow = *g_mainWindow;

	BodyPanel * bodyPanel = new BodyPanel();
	mainWindow.addPanel(bodyPanel);
	bodyPanel->createWidgets();
	bodyPanel->show_all();

	FacePanel * facePanel = new FacePanel();
	mainWindow.addPanel(facePanel);
	facePanel->createWidgets();
	facePanel->show_all();

	TeethPanel * teethPanel = new TeethPanel();
	mainWindow.addPanel(teethPanel);
	teethPanel->createWidgets();
	teethPanel->show_all();

	HandsPanel * handsPanel = new HandsPanel();
	mainWindow.addPanel(handsPanel);
	handsPanel->createWidgets();
	handsPanel->show_all();
}
