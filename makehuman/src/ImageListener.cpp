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
#include "BottomPanel.h"
#include "BsCategoryPanel.h"
#include "BsPanel.h"
#include "CharacterSettingPanel.h"
#include "ClothesPanel.h"
#include "ComponentID.h"
#include "ConsoleCommands.h"
#include "FacePanel.h"
#include "Global.h"
#include "HandsPanel.h"
#include "PoseTargetPanel.h"
#include "PosesBodyPanel.h"
#include "PosesBsCategoryPanel.h"
#include "PosesBsPanel.h"
#include "SplashPanel.h"
#include "TargetPanel.h"
#include "TeethPanel.h"
#include "ToolbarPanel.h"
#include "UtilitiesPanel.h"
#include "util.h"
#include <animorph/DirectoryList.h>
#include <animorph/MathUtil.h>
#include <animorph/ObjExporter.h>
#include <animorph/RIBExporter.h>
#include <assert.h>
#include <gui/CGUtilities.h>
#include <gui/Camera.h>
#include <gui/Console.h>
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

bool ImageListener::mouseOver(const Point &inMousePos, Component *source)
{
	Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(Color(1, 0, 0, 0.5));

	return false;
}

bool ImageListener::mouseOut(const Point &inMousePos, Component *source)
{
	Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return false;
}

bool ImageListener::mouseDragged(const Point &inMousePos, Component *source)
{

	Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return true;
}

bool ImageListener::mouseWheel(const Point &inMousePos, int inButton,
                               Component *source)
{
	return false;
}

bool ImageListener::mousePressed(const Point &inMousePos, int button,
                                 Component *source)
{
	if (button == GLUT_LEFT_BUTTON) {
		Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?
		imgSource->setOverlayRectangle(Color(1, 1, 1, 0.5));

		return true;
	}
	return false;
}

bool ImageListener::mouseReleased(const Point &inMousePos, int button,
                                  Component *source)
{

	BsPanel *bs_p;
	PosesBsPanel *pb_p;
	PoseTargetPanel *pt_p;
	TargetPanel *tg_p;
	CharacterSettingPanel *cs_p;
	if (button == GLUT_LEFT_BUTTON) {
		Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?

		if (imgSource->getAbsoluteRect().isHitBy(inMousePos)) {
			Global &global = Global::instance();

			// DialogPanel *dialogPanel;// = new DialogPanel (imgSource->getID ());

			Window &mainWindow = *g_mainWindow;
			imgSource->setOverlayRectangle(false);

			switch (imgSource->getID()) {
			// toolbar buttons
			case kComponentID_ImageToolbarPanel_LoadBodysettings: {
				if (global.getAppMode() == BODY_DETAILS ||
				    global.getAppMode() == CHARACTER_SETTING ||
				    global.getAppMode() == CLOTHES) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Load_Bodysettings,
					    kConsoleMessage_Load_Bodysettings, getMyBodysettingsPath());
				} else if (global.getAppMode() == POSES) {
					mainWindow.getConsole()->openWithCommand(kConsoleCommand_Load_Poses,
					                                         kConsoleMessage_Load_Poses,
					                                         getMyPosesPath());
				} else if (global.getAppMode() == ANIMATIONS) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Load_Animations, kConsoleMessage_Load_Animations,
					    getMyPosesPath());
				}
			} break;
			case kComponentID_ImageToolbarPanel_SaveBodysettings: {
				if (global.getAppMode() == BODY_DETAILS ||
				    global.getAppMode() == CHARACTER_SETTING ||
				    global.getAppMode() == CLOTHES) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Save_Bodysettings,
					    kConsoleMessage_Save_Bodysettings, getMyBodysettingsPath());
				} else if (global.getAppMode() == POSES) {
					mainWindow.getConsole()->openWithCommand(kConsoleCommand_Save_Poses,
					                                         kConsoleMessage_Save_Poses,
					                                         getMyPosesPath());
				} else if (global.getAppMode() == ANIMATIONS) {
					// mainWindow.getConsole()->openWithCommand(kConsoleCommand_Save_Animations,
					// kConsoleMessage_Save_Animations, getMyPosesPath());
				}
			} break;
			case kComponentID_ImageToolbarPanel_ExportWavefrontObj:
				if (global.getAppMode() != ANIMATIONS) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Export_Object, kConsoleMessage_Export_Object,
					    getMyObjPath());
				}
				break;
			case kComponentID_ImageToolbarPanel_ExportColladaObj:
				if (global.getAppMode() != ANIMATIONS) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Export_Collada, kConsoleMessage_Export_Collada,
					    getMyColladaPath());
				}
				break;
			case kComponentID_ImageToolbarPanel_LoadBackground:
				if (global.getAppMode() != ANIMATIONS) {
					mainWindow.getConsole()->openWithCommand(
					    kConsoleCommand_Load_Background, kConsoleMessage_Load_Background,
					    getMyBackgroundsPath());
				}
				break;

			case kComponentID_ImageToolbarPanel_CharacterSettings: {
				if (global.getAppMode() == CHARACTER_SETTING)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != BODY_DETAILS &&
				    global.getAppMode() != BODY_SETTINGS &&
				    global.getAppMode() != CLOTHES) {
					Mesh *mesh = global.getMesh();
					assert(mesh);

					mesh->bodyDetailsMode();

					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				if (global.getAppMode() == ANIMATIONS) {
					global.getAnimation()->setStarted(false);
					global.getAnimation()->setLoaded(false);
				}

				global.setAppMode(CHARACTER_SETTING);
				showCharacterSetting();
			} break;
			case kComponentID_ImageToolbarPanel_BodyDetailsRealistic: {
				if (global.getAppMode() == BODY_DETAILS)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != CHARACTER_SETTING &&
				    global.getAppMode() != BODY_SETTINGS &&
				    global.getAppMode() != CLOTHES) {
					Mesh *mesh = global.getMesh();
					assert(mesh);

					mesh->bodyDetailsMode();

					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				if (global.getAppMode() == ANIMATIONS) {
					global.getAnimation()->setStarted(false);
					global.getAnimation()->setLoaded(false);
				}

				global.setAppMode(BODY_DETAILS);
				showBodyDetails();
			} break;
			/*      case kComponentID_ImageToolbarPanel_Clothes:
				      {
				        if(global.getAppMode() == CLOTHES) return true;

				        hidePanels(global.getAppMode());

				        if(global.getAppMode() == ANIMATIONS)
				        {
				          global.getAnimation()->setStarted(false);
				          global.getAnimation()->setLoaded(false);
				        }

				        global.setAppMode(CLOTHES);
				        showClothes();
				      }
				      break;
			*/
			case kComponentID_ImageToolbarPanel_Poses: {
				if (global.getAppMode() == POSES)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != ANIMATIONS &&
				    global.getAppMode() != POSES_BODY_SETTINGS) {
					Mesh *mesh = global.getMesh();
					assert(mesh);
					mesh->poseMode();
					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				if (global.getAppMode() == ANIMATIONS) {
					global.getAnimation()->setStarted(false);
					global.getAnimation()->setLoaded(false);
				}

				global.setAppMode(POSES);
				showPoses();
			} break;
			case kComponentID_ImageToolbarPanel_BodySettings: {
				if (global.getAppMode() == BODY_SETTINGS)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != CHARACTER_SETTING &&
				    global.getAppMode() != BODY_DETAILS &&
				    global.getAppMode() != CLOTHES) {
					Mesh *mesh = global.getMesh();
					assert(mesh);

					mesh->bodyDetailsMode();

					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				if (global.getAppMode() == ANIMATIONS) {
					global.getAnimation()->setStarted(false);
					global.getAnimation()->setLoaded(false);
				}

				global.setAppMode(BODY_SETTINGS);
				showBsCategory();
			} break;
			case kComponentID_ImageToolbarPanel_PoseBodySettings: {
				if (global.getAppMode() == POSES_BODY_SETTINGS)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != ANIMATIONS && global.getAppMode() != POSES) {
					Mesh *mesh = global.getMesh();
					assert(mesh);

					mesh->poseMode();
					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				if (global.getAppMode() == ANIMATIONS) {
					global.getAnimation()->setStarted(false);
					global.getAnimation()->setLoaded(false);
				}

				global.setAppMode(POSES_BODY_SETTINGS);
				showPosesBsCategory();
			} break;
			/*     case kComponentID_ImageToolbarPanel_Lights:

				     break;
				   case kComponentID_ImageToolbarPanel_Expressions:

				     break;

				   case kComponentID_ImageToolbarPanel_Asymmetry:

				     break;
				   case kComponentID_ImageToolbarPanel_Hairs:

				     break;
				   case kComponentID_ImageToolbarPanel_Materials:

				     break;
		 */
			case kComponentID_ImageToolbarPanel_Rendering:
				rendering(mainWindow, NORMAL);
				break;
			case kComponentID_ImageToolbarPanel_RenderingPreview: {
				rendering(mainWindow, PREVIEW);
			} break;
			case kComponentID_ImageToolbarPanel_MorphingList: {
				Panel *p = mainWindow.getPanel(kComponentID_TargetPanel);
				mainWindow.removePanel(p);
				delete p;

				if (global.getAppMode() == BODY_DETAILS ||
				    global.getAppMode() == CHARACTER_SETTING) {
					TargetPanel *targetPanel;

					int x = mainWindow.getSize().getWidth() - 210;
					targetPanel =
					    new TargetPanel(applied_target_list, Rect(x, 104, 160, 460));
					mainWindow.addPanel(targetPanel);
					targetPanel->createWidgets();
					targetPanel->show();
				} else if (global.getAppMode() == POSES) {
					PoseTargetPanel *targetPanel;

					int x = mainWindow.getSize().getWidth() - 210;
					targetPanel =
					    new PoseTargetPanel(applied_target_list, Rect(x, 40, 210, 517));
					mainWindow.addPanel(targetPanel);
					targetPanel->createWidgets();
					targetPanel->show_all();
				} else if (global.getAppMode() == CLOTHES) {
					// TODO: what happens when in clothes mode ?
				} else if (global.getAppMode() == ANIMATIONS) {
					// TODO: what happens when in animation mode ?
				}
			} break;
			case kComponentID_ImageToolbarPanel_Animation: {
				if (global.getAppMode() == ANIMATIONS)
					return true;

				hidePanels(global.getAppMode());

				if (global.getAppMode() != POSES) {
					Mesh *mesh = global.getMesh();
					assert(mesh);
					mesh->animationMode();

					if (global.getSubdivision()) {
						mesh->calcSubsurf();
					}
				}

				global.setAppMode(ANIMATIONS);
				showUtilities();
			} break;
			case kComponentID_ImageToolbarPanel_Reset: {
				switch (global.getAppMode()) {

				case POSES:
					pt_p = dynamic_cast<PoseTargetPanel *>(
					    mainWindow.getPanel(kComponentID_TargetPanel));
					if (pt_p != NULL) {
						pt_p->resetTargetValues();
					}
					ResetMeshPose(global);
					break;

				case POSES_BODY_SETTINGS:
					ResetMeshPose(global);
					break;
				case BODY_DETAILS:
					tg_p = dynamic_cast<TargetPanel *>(
					    mainWindow.getPanel(kComponentID_TargetPanel));
					if (tg_p != NULL) {
						tg_p->resetTargetValues();
					}
					ResetMeshMorph(global);
					break;
				case BODY_SETTINGS:
					ResetMeshMorph(global);
					break;
				case CHARACTER_SETTING:
					cs_p = dynamic_cast<CharacterSettingPanel *>(
					    mainWindow.getPanel(kComponentID_CharacterSettingPanel));
					if (cs_p != NULL) {
						cs_p->resetSlidersValues();
					}
					ResetMeshMorph(global);
					break;
				default:
					break;
				}
			} break;
			// utilitybar buttons
			case kComponentID_ImageUtilitybar_FlatShading: {
				if (global.getFlatShading())
					global.setFlatShading(false);
				else
					global.setFlatShading(true);
			} break;
			case kComponentID_ImageUtilitybar_RenderPreview: {
				rendering(mainWindow, PREVIEW);
			} break;
			case kComponentID_CloseTargetPanel: {
				Panel *p = mainWindow.getPanel(kComponentID_TargetPanel);
				mainWindow.removePanel(p);
			} break;

			// main character setting panel icons
			/*    case kComponentID_CharacterSettingPanel_Reset:
				    {

				      CharacterSettingPanel *csp = (CharacterSettingPanel
				 *)mainWindow.getPanel (kComponentID_CharacterSettingPanel); if(csp !=
				 NULL)
				      {
				        csp->resetTargetValues();
				      }

				      BsPanel *bsp = (BsPanel
				 *)mainWindow.getPanel(kComponentID_TargetPanel); if(bsp != NULL)
				      {
				        bsp->resetTargetValues();
				      }

				      Mesh *mesh = global.getMesh ();
				      assert (mesh);

				      mesh->resetMorph();
				      mesh->calcNormals();

				    }
				    break;
		*/
			case kComponentID_NextTargetPanel: {
				MultiPanel *p = dynamic_cast<MultiPanel *>(
				    mainWindow.getPanel(kComponentID_TargetPanel));
				assert(p);
				p->pageNext();
				break;
			}
			case kComponentID_PrevTargetPanel: {
				MultiPanel *p = dynamic_cast<MultiPanel *>(
				    mainWindow.getPanel(kComponentID_TargetPanel));
				assert(p);
				p->pageBack();
				break;
			}
			case kComponentID_UtilitiesPanel_StartStop: {
				Animation *animation = global.getAnimation();
				if (animation->isLoaded()) {
					animation->setStarted(!animation->isStarted());
				} else {
					mainWindow.getConsole()->open();
					mainWindow.getConsole()->printMessage(
					    kConsoleMessage_NotLoaded_Animations);
				}
				break;
			}
			case kComponentID_UtilitiesPanel_Reset: {
				Animation *animation = global.getAnimation();
				if (animation->isLoaded()) {
					animation->setStarted(false);
				}
				animation->reset();

				break;
			}

			default:
				cerr << "Unknown widget handler!" << imgSource->getIDAsString() << endl;
			}

			return true;
		}
	}

	return false;
}

bool ImageListener::keyType(unsigned char key, Component *source)
{
	return false;
}

void ImageListener::hidePanels(const Modes currentMode)
{
	if (currentMode == POSES) {
		hidePoses();
	} else if (currentMode == BODY_DETAILS) {
		hideBodyDetails();
	} else if (currentMode == CHARACTER_SETTING) {
		hideCharacterSetting();
	} else if (currentMode == CLOTHES) {
		hideClothes();
	} else if (currentMode == ANIMATIONS) {
		hideUtilities();
	} else if (currentMode == BODY_SETTINGS) {
		hideBsCategory();
	} else if (currentMode == POSES_BODY_SETTINGS) {
		hidePosesBsCategory();
	}
}

void ImageListener::hideBodyDetails()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_BodyPanel);
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
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_CharacterSettingPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

void ImageListener::hidePoses()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_BodyPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

void ImageListener::hideClothes()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_ClothesPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

void ImageListener::hideUtilities()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_UtilitiesPanel);
	mainWindow.removePanel(p);
	delete p;

	ToolbarPanel *tp =
	    (ToolbarPanel *)mainWindow.getPanel(kComponentID_ToolbarPanel);

	Image *img =
	    (Image *)(tp->getWidget(kComponentID_ImageToolbarPanel_MorphingList));
	img->setEnabled(true);

	img =
	    (Image *)(tp->getWidget(kComponentID_ImageToolbarPanel_SaveBodysettings));
	img->setEnabled(true);

	img = (Image *)(tp->getWidget(
	    kComponentID_ImageToolbarPanel_ExportWavefrontObj));
	img->setEnabled(true);
}
void ImageListener::hideBsCategory()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_BsCategoryPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}
void ImageListener::hidePosesBsCategory()
{
	Window &mainWindow = *g_mainWindow;
	Panel *p = mainWindow.getPanel(kComponentID_PosesBsCategoryPanel);
	mainWindow.removePanel(p);
	delete p;

	p = mainWindow.getPanel(kComponentID_TargetPanel);
	mainWindow.removePanel(p);
	delete p;
}

// show:

void ImageListener::showCharacterSetting()
{
	Window &mainWindow = *g_mainWindow;

	CharacterSettingPanel *characterSettingPanel = new CharacterSettingPanel();
	mainWindow.addPanel(characterSettingPanel);
	characterSettingPanel->createWidgets();
	characterSettingPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition7);
	p->createWidgets();
}

void ImageListener::showBodyDetails()
{
	Window &mainWindow = *g_mainWindow;

	BodyPanel *bodyPanel = new BodyPanel();
	mainWindow.addPanel(bodyPanel);
	bodyPanel->createWidgets();
	bodyPanel->show_all();

	FacePanel *facePanel = new FacePanel();
	mainWindow.addPanel(facePanel);
	facePanel->createWidgets();
	facePanel->show_all();

	TeethPanel *teethPanel = new TeethPanel();
	mainWindow.addPanel(teethPanel);
	teethPanel->createWidgets();
	teethPanel->show_all();

	HandsPanel *handsPanel = new HandsPanel();
	mainWindow.addPanel(handsPanel);
	handsPanel->createWidgets();
	handsPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition4);
	p->createWidgets();
}

void ImageListener::showPoses()
{
	Window &mainWindow = *g_mainWindow;
	PosesBodyPanel *posesPanel = new PosesBodyPanel();
	mainWindow.addPanel(posesPanel);
	posesPanel->createWidgets();
	posesPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition2);
	p->createWidgets();
}

void ImageListener::showClothes()
{
	Window &mainWindow = *g_mainWindow;
	ClothesPanel *clothesPanel = new ClothesPanel();
	mainWindow.addPanel(clothesPanel);
	clothesPanel->createWidgets();
	clothesPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition2);
	p->createWidgets();
}

void ImageListener::showUtilities()
{
	Window &mainWindow = *g_mainWindow;
	UtilitiesPanel *utilitiesPanel = new UtilitiesPanel();
	mainWindow.addPanel(utilitiesPanel);
	utilitiesPanel->createWidgets();
	utilitiesPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition5);
	p->createWidgets();

	ToolbarPanel *tp =
	    (ToolbarPanel *)mainWindow.getPanel(kComponentID_ToolbarPanel);

	Image *img =
	    (Image *)(tp->getWidget(kComponentID_ImageToolbarPanel_MorphingList));
	img->setEnabled(false);

	img =
	    (Image *)(tp->getWidget(kComponentID_ImageToolbarPanel_SaveBodysettings));
	img->setEnabled(false);

	img = (Image *)(tp->getWidget(
	    kComponentID_ImageToolbarPanel_ExportWavefrontObj));
	img->setEnabled(false);
}

void ImageListener::showBsCategory()
{
	Window &mainWindow = *g_mainWindow;
	BsCategoryPanel *bsPanel = new BsCategoryPanel();
	mainWindow.addPanel(bsPanel);
	bsPanel->createWidgets();
	bsPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition6);
	p->createWidgets();
}

void ImageListener::showPosesBsCategory()
{
	Window &mainWindow = *g_mainWindow;
	PosesBsCategoryPanel *posesBsPanel = new PosesBsCategoryPanel();
	mainWindow.addPanel(posesBsPanel);
	posesBsPanel->createWidgets();
	posesBsPanel->show_all();

	BottomPanel *p = (BottomPanel *)mainWindow.getPanel(kComponentID_BottomPanel);
	assert(p);
	p->setPosition(kBottomPosition6);
	p->createWidgets();
}

// TODO: remove global parameter
void ImageListener::ResetMeshMorph(Global &global)
{
	Mesh *mesh = global.getMesh();
	assert(mesh);
	mesh->resetMorph();

	loadDefaultBodySettings();

	Global::instance().resetFuzzyValues();
}

void ImageListener::ResetMeshPose(Global &global)
{
	Mesh *mesh = global.getMesh();
	assert(mesh);
	mesh->resetPose();

	if (global.getSubdivision()) {
		mesh->calcSubsurf();
	}
}
