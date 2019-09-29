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
 *  File   : TargetSelectionListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "TargetSelectionListener.h"
#include <animorph/Mesh.h>
#include <gui/CGUtilities.h>
#include <gui/Image.h>

#include <gui/Camera.h>
#include <gui/Component.h>
#include <gui/GLUTWrapper.h>
#include <gui/Window.h>

#include <assert.h>
#include <stdio.h>

#include "ComponentID.h"
#include "Global.h"
#include "TargetPanel.h"

using namespace Animorph;
using namespace std;

TargetSelectionListener::TargetSelectionListener()
        : AbstractListener()
{
}

TargetSelectionListener::~TargetSelectionListener() {}

bool TargetSelectionListener::mouseOver(const Point & inMousePos, Component * source)
{
	Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(Color(1, 0, 0, 0.5));

	return false;
}

bool TargetSelectionListener::mouseOut(const Point & inMousePos, Component * source)
{
	Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return false;
}

bool TargetSelectionListener::mouseDragged(const Point & inMousePos, Component * source)
{
	return true;
}

bool TargetSelectionListener::mouseWheel(const Point & inMousePos, int inButton, Component * source)
{
	return false;
}

bool TargetSelectionListener::mousePressed(const Point & inMousePos, int button, Component * source)
{
	if(button == GLUT_LEFT_BUTTON) {
		Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?
		imgSource->setOverlayRectangle(Color(1, 1, 1, 0.5));

		return true;
	}
	return false;
}

bool TargetSelectionListener::mouseReleased(const Point & inMousePos, int button,
                                            Component * source)
{
	if(button == GLUT_LEFT_BUTTON) {
		Image * imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?

		imgSource->setOverlayRectangle(false);

		if(!imgSource->getAbsoluteRect().isHitBy(inMousePos))
			return false;

		string target;

		switch(imgSource->getID()) {
			// The Image ID's of the  BodyPanel (kComponentID_BodyPanel)
		case kComponentID_ImageBody_HeadParams:
			target = "head";
			break;
		case kComponentID_ImageBody_ShoulderParams:
			target = "shoulders";
			break;
			/*
			    case kComponentID_ImageBody_RightShoulderParams:
			      target = "r_shoulder";
			      break;
			*/
		case kComponentID_ImageBody_NeckParams:
			target = "neck";
			break;
		case kComponentID_ImageBody_UpperArmParams:
			target = "upper_arms";
			break;
			/*
			    case kComponentID_ImageBody_RightUpperArmParams:
			      target = "r_upper_arm";
			      break;
			*/
		case kComponentID_ImageBody_LowerArmParams:
			target = "lower_arms";
			break;
			/*
			    case kComponentID_ImageBody_RightLowerArmParams:
			      target = "r_lower_arm";
			      break;
			*/
		case kComponentID_ImageBody_AbdomenParams:
			target = "abdomen";
			break;
		case kComponentID_ImageBody_TorsoParams:
			target = "torso";
			break;
			/*
			    case kComponentID_ImageBody_TorsoParams2:
			      target = "torso2";
			      break;
			*/
		case kComponentID_ImageBody_HandParams:
			target = "hands";
			break;
			/*
			    case kComponentID_ImageBody_RightHandParams:
			      target = "r_hand";
			      break;
			*/
		case kComponentID_ImageBody_PelvisParams:
			target = "pelvis";
			break;
		case kComponentID_ImageBody_UpperLegParams:
			target = "upper_legs";
			break;
			/*
			    case kComponentID_ImageBody_RightUpperLegParams:
			      target = "r_upper_leg";
			      break;
			*/
		case kComponentID_ImageBody_LowerLegParams:
			target = "lower_legs";
			break;
			/*
			    case kComponentID_ImageBody_RightLowerLegParams:
			      target = "r_lower_leg";
			      break;
			    case kComponentID_ImageBody_FootParams2:
			      target = "foot2";
			      break;
			    case kComponentID_ImageBody_RightFootParams2:
			      target = "r_foot2";
			      break;
			*/
		case kComponentID_ImageBody_FootParams:
			target = "feet";
			break;
			/*
			    case kComponentID_ImageBody_RightFootParams:
			      target = "r_foot";
			      break;
			*/
			// The Image ID's of the  FacePanel (kComponentID_FacePanel)
			/*
			    case kComponentID_FacePanel_RightEyeParams2:
			      target = "r_eye2";
			      break;
			    case kComponentID_FacePanel_LeftEyeParams2:
			      target = "l_eye2";
			      break;
			    case kComponentID_FacePanel_BrowRidgeParams2:
			      target = "forehead";
			      break;
			    case kComponentID_FacePanel_RightCheekParams:
			      target = "r_cheek";
			      break;
			    case kComponentID_FacePanel_NoseParams:
			      target = "nose";
			      break;
			    case kComponentID_FacePanel_LeftCheekParams:
			      target = "l_cheek";
			      break;
			    case kComponentID_FacePanel_LeftEyeParams:
			      target = "l_eye";
			      break;
			    case kComponentID_FacePanel_RightEyeParams:
			      target = "r_eye";
			      break;
			    case kComponentID_FacePanel_LeftEarParams:
			      target = "l_ear";
			      break;
			    case kComponentID_FacePanel_RightEarParams:
			      target = "r_ear";
			      break;
			    case kComponentID_FacePanel_MouthParams:
			      target = "mouth";
			      break;
			    case kComponentID_FacePanel_LeftEarParams2:
			      target = "l_ear2";
			      break;
			    case kComponentID_FacePanel_RightEarParams2:
			      target = "r_ear2";
			      break;
			    case kComponentID_FacePanel_ChinJawParams:
			      target = "chin_jaw";
			      break;
			    case kComponentID_FacePanel_MouthParams2:
			      target = "mouth2";
			      break;
			    case kComponentID_FacePanel_MouthParams3:
			      target = "mouth3";
			      break;
			    case kComponentID_FacePanel_BrowRidgeParams:
			      target = "brow_ridge";
			      break;
			*/
		case kComponentID_FacePanel_ForeheadParams:
			target = "forehead";
			break;
		case kComponentID_FacePanel_EyesParams:
			target = "eyes";
			break;
		case kComponentID_FacePanel_NoseParams:
			target = "nose";
			break;
		case kComponentID_FacePanel_EarsParams:
			target = "ears";
			break;
		case kComponentID_FacePanel_MouthParams:
			target = "mouth";
			break;
		case kComponentID_FacePanel_ChinJawParams:
			target = "chin_jaw";
			break;
		case kComponentID_FacePanel_Cheek:
			target = "cheek";
			break;

			// The Image ID's of the  TeethPanel (kComponentID_TeethPanel)
		case kComponentID_TeethPanel_TongueParams:
			target = "tongue";
			break;
		case kComponentID_TeethPanel_TeethParams:
			target = "teeth";
			break;
			/*
			    case kComponentID_TeethPanel_UpperTeethRightParams:
			      target = "upper_teeth_right";
			      break;
			    case kComponentID_TeethPanel_UpperTeethCenterParams:
			      target = "upper_teeth_center";
			      break;
			    case kComponentID_TeethPanel_UpperTeethLeftParams:
			      target = "upper_teeth_left";
			      break;
			    case kComponentID_TeethPanel_LowerTeethRightParams:
			      target = "lower_teeth_right";
			      break;
			    case kComponentID_TeethPanel_LowerTeethCenterParams:
			      target = "lower_teeth_center";
			      break;
			    case kComponentID_TeethPanel_LowerTeethLeftParams:
			      target = "lower_teeth_left";
			      break;
			*/
			// The Image ID's of the  HandsPanel (kComponentID_HandsPanel)
			/*
			    case kComponentID_HandsPanel_RingfingerRightParams:
			      target = "r_ringfinger";
			      break;
			    case kComponentID_HandsPanel_MiddlefingerRightParams:
			      target = "r_middlefinger";
			      break;
			*/
		case kComponentID_HandsPanel_MiddlefingerLeftParams:
			target = "middlefinger";
			break;
		case kComponentID_HandsPanel_RingfingerLeftParams:
			target = "ringfinger";
			break;
			/*
			    case kComponentID_HandsPanel_ForefingerRightParams:
			      target = "r_forefinger";
			      break;
			*/
		case kComponentID_HandsPanel_ForefingerLeftParams:
			target = "forefinger";
			break;
			/*
			    case kComponentID_HandsPanel_LittlefingerRightParams:
			      target = "r_littlefinger";
			      break;
			    case kComponentID_HandsPanel_HandRightParams2:
			      target = "r_hand2";
			      break;
			    case kComponentID_HandsPanel_PollexRightParams:
			      target = "r_pollex";
			      break;
			*/
		case kComponentID_HandsPanel_PollexLeftParams:
			target = "pollex";
			break;
			/*
			    case kComponentID_HandsPanel_HandLeftParams2:
			      target = "l_hand2";
			      break;
			*/
		case kComponentID_HandsPanel_LittlefingerLeftParams:
			target = "littlefinger";
			break;
		default:
			return false;
			break;
		}

		Window & mainWindow = *g_mainWindow;

		TargetPanel * targetPanel =
		        dynamic_cast<TargetPanel *>(mainWindow.getPanel(kComponentID_TargetPanel));

		// Check if this Target Panel is not the current one?
		if((targetPanel == NULL) || (targetPanel->getCategory() != target)) {
			// No? The create it
			int x = mainWindow.getSize().getWidth() - 210;

			mainWindow.removePanel(targetPanel);
			delete targetPanel;

			targetPanel = new TargetPanel(target, Rect(x, 104, 160, 460));

			mainWindow.addPanel(targetPanel);
			targetPanel->createWidgets();
		}

		Autozoom * autozoom = g_global.getAutozoom();
		Camera *   camera   = g_global.getCamera();
		assert(autozoom);

		Mesh * mesh = g_global.getMesh();
		assert(mesh);
		VertexVector & vertexvector(mesh->getVertexVectorRef());

		std::string pathAutozoom_data =
		        searchDataDir("targets") + "/" + target + "/" + target + ".camera";
		if(autozoom->lazyLoadData(pathAutozoom_data)) {
			camera->moveCameraAnimated(pathAutozoom_data,
			                           autozoom->getAutozoomData(pathAutozoom_data),
			                           vertexvector);
		}

		targetPanel->show();
		// targetPanel->show_all();

		return true;
	}

	return false;
}

bool TargetSelectionListener::keyType(unsigned char key, Component * source) { return false; }
