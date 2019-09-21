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
 *  File   : PoseTargetSelectionListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <animorph/Mesh.h>

#include <gui/CGUtilities.h>
#include <gui/Component.h>
#include <gui/GLUTWrapper.h>
#include <gui/Image.h>
#include <gui/Window.h>

#include <assert.h>
#include <stdio.h>

#include "ComponentID.h"
#include "Global.h"
#include "PoseTargetPanel.h"
#include "PoseTargetSelectionListener.h"

using namespace Animorph;
using namespace std;

PoseTargetSelectionListener::PoseTargetSelectionListener()
    : AbstractListener()
{
}

PoseTargetSelectionListener::~PoseTargetSelectionListener() {}

bool PoseTargetSelectionListener::mouseOver(const Point &inMousePos,
                                            Component *source)
{
	Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(Color(1, 0, 0, 0.5));

	return false;
}

bool PoseTargetSelectionListener::mouseOut(const Point &inMousePos,
                                           Component *source)
{
	Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
	assert(imgSource); // Check if this is really an Image object?

	imgSource->setOverlayRectangle(false);

	return false;
}

bool PoseTargetSelectionListener::mouseDragged(const Point &inMousePos,
                                               Component *source)
{
	return true;
}

bool PoseTargetSelectionListener::mouseWheel(const Point &inMousePos,
                                             int inButton, Component *source)
{
	return false;
}

bool PoseTargetSelectionListener::mousePressed(const Point &inMousePos,
                                               int button, Component *source)
{
	if (button == GLUT_LEFT_BUTTON) {
		Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?
		imgSource->setOverlayRectangle(Color(1, 1, 1, 0.5));

		return true;
	}
	return false;
}

bool PoseTargetSelectionListener::mouseReleased(const Point &inMousePos,
                                                int button, Component *source)
{
	if (button == GLUT_LEFT_BUTTON) {
		Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
		assert(imgSource); // Check if this is really an Image object?

		imgSource->setOverlayRectangle(false);

		if (!imgSource->getAbsoluteRect().isHitBy(inMousePos))
			return false;

		string target;

		switch (imgSource->getID()) {
			// The Image ID's of the  PoseBodyPanel (kComponentID_PosesBodyPanel)
		case kComponentID_PosesImageBody_RightCollarParams:
			target = "260_right_collar";
			break;
		case kComponentID_PosesImageBody_HeadParams:
			target = "300_head";
			break;
		case kComponentID_PosesImageBody_LeftCollarParams:
			target = "280_left_collar";
			break;
		case kComponentID_PosesImageBody_RightUpperArmParams:
			target = "220_right_upper_arm";
			break;
		case kComponentID_PosesImageBody_NeckParams:
			target = "320_neck";
			break;
		case kComponentID_PosesImageBody_LeftUpperArmParams:
			target = "240_left_upper_arm";
			break;
		case kComponentID_PosesImageBody_TorsoParams:
			target = "360_torso";
			break;
		case kComponentID_PosesImageBody_LeftLowerArmParams:
			target = "160_left_lower_arm";
			break;
		case kComponentID_PosesImageBody_RightLowerArmParams:
			target = "140_right_lower_arm";
			break;
		case kComponentID_PosesImageBody_LeftHandParams:
			target = "080_left_hand";
			break;
		case kComponentID_PosesImageBody_LeftUpperLegParams:
			target = "200_left_upper_leg";
			break;
		case kComponentID_PosesImageBody_RightUpperLegParams:
			target = "180_right_upper_leg";
			break;
		case kComponentID_PosesImageBody_RightHandParams:
			target = "060_right_hand";
			break;
		case kComponentID_PosesImageBody_LeftLowerLegParams:
			target = "120_left_lower_leg";
			break;
		case kComponentID_PosesImageBody_RightLowerLegParams:
			target = "100_right_lower_leg";
			break;
		case kComponentID_PosesImageBody_LeftFootParams:
			target = "040_left_foot";
			break;
		case kComponentID_PosesImageBody_RightFootParams:
			target = "020_right_foot";
			break;
		case kComponentID_PosesImageBody_LeftRingfinger3Params:
			target = "070_left_ringfinger_3";
			break;
		case kComponentID_PosesImageBody_LeftMiddlefinger3Params:
			target = "067_left_middlefinger_3";
			break;
		case kComponentID_PosesImageBody_LeftForefinger3Params:
			target = "064_left_forefinger_3";
			break;
		case kComponentID_PosesImageBody_LeftRingfinger2Params:
			target = "071_left_ringfinger_2";
			break;
		case kComponentID_PosesImageBody_LeftMiddlefinger2Params:
			target = "068_left_middlefinger_2";
			break;
		case kComponentID_PosesImageBody_LeftForefinger2Params:
			target = "065_left_forefinger_2";
			break;
		case kComponentID_PosesImageBody_LeftLittlefinger3Params:
			target = "073_left_littlefinger_3";
			break;
		case kComponentID_PosesImageBody_LeftMiddlefinger1Params:
			target = "069_left_middlefinger_1";
			break;
		case kComponentID_PosesImageBody_LeftForefinger1Params:
			target = "066_left_forefinger_1";
			break;
		case kComponentID_PosesImageBody_LeftLittlefinger2Params:
			target = "074_left_littlefinger_2";
			break;
		case kComponentID_PosesImageBody_LeftRingfinger1Params:
			target = "072_left_ringfinger_1";
			break;
		case kComponentID_PosesImageBody_LeftPollex3Params:
			target = "061_left_pollex_3";
			break;
		case kComponentID_PosesImageBody_LeftLittlefinger1Params:
			target = "075_left_littlefinger_1";
			break;
		case kComponentID_PosesImageBody_LeftPollex2Params:
			target = "062_left_pollex_2";
			break;
		case kComponentID_PosesImageBody_LeftPollex1Params:
			target = "063_left_pollex_1";
			break;
		case kComponentID_PosesImageBody_RightForefinger3Params:
			target = "044_right_forefinger_3";
			break;
		case kComponentID_PosesImageBody_RightMiddlefinger3Params:
			target = "047_right_middlefinger_3";
			break;
		case kComponentID_PosesImageBody_RightRingfinger3Params:
			target = "050_right_ringfinger_3";
			break;
		case kComponentID_PosesImageBody_RightForefinger2Params:
			target = "045_right_forefinger_2";
			break;
		case kComponentID_PosesImageBody_RightMiddlefinger2Params:
			target = "048_right_middlefinger_2";
			break;
		case kComponentID_PosesImageBody_RightRingfinger2Params:
			target = "051_right_ringfinger_2";
			break;
		case kComponentID_PosesImageBody_RightForefinger1Params:
			target = "046_right_forefinger_1";
			break;
		case kComponentID_PosesImageBody_RightMiddlefinger1Params:
			target = "049_right_middlefinger_1";
			break;
		case kComponentID_PosesImageBody_RightLittlefinger3Params:
			target = "053_right_littlefinger_3";
			break;
		case kComponentID_PosesImageBody_RightPollex3Params:
			target = "041_right_pollex_3";
			break;
		case kComponentID_PosesImageBody_RightRingfinger1Params:
			target = "052_right_ringfinger_1";
			break;
		case kComponentID_PosesImageBody_RightLittlefinger2Params:
			target = "054_right_littlefinger_2";
			break;
		case kComponentID_PosesImageBody_RightPollex2Params:
			target = "042_right_pollex_2";
			break;
		case kComponentID_PosesImageBody_RightPollex1Params:
			target = "043_right_pollex_1";
			break;
		case kComponentID_PosesImageBody_RightLittlefinger1Params:
			target = "055_right_littlefinger_1";
			break;
		case kComponentID_PosesImageBody_RightFoot42Params:
			target = "002_right_footfinger_4_2";
			break;
		case kComponentID_PosesImageBody_RightFoot52Params:
			target = "000_right_footfinger_5_2";
			break;
		case kComponentID_PosesImageBody_RightFoot51Params:
			target = "001_right_footfinger_5_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot51Params:
			target = "022_left_footfinger_5_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot52Params:
			target = "021_left_footfinger_5_2";
			break;
		case kComponentID_PosesImageBody_LeftFoot42Params:
			target = "023_left_footfinger_4_2";
			break;
		case kComponentID_PosesImageBody_RightFoot32Params:
			target = "004_right_footfinger_3_2";
			break;
		case kComponentID_PosesImageBody_RightFoot31Params:
			target = "005_right_footfinger_3_1";
			break;
		case kComponentID_PosesImageBody_RightFoot41Params:
			target = "003_right_footfinger_4_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot41Params:
			target = "024_left_footfinger_4_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot31Params:
			target = "026_left_footfinger_3_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot32Params:
			target = "025_left_footfinger_3_2";
			break;
		case kComponentID_PosesImageBody_RightFoot22Params:
			target = "006_right_footfinger_2_2";
			break;
		case kComponentID_PosesImageBody_RightFoot21Params:
			target = "007_right_footfinger_2_1";
			break;
		case kComponentID_PosesImageBody_RightFoot11Params:
			target = "009_right_footfinger_1_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot11Params:
			target = "030_left_footfinger_1_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot21Params:
			target = "028_left_footfinger_2_1";
			break;
		case kComponentID_PosesImageBody_LeftFoot22Params:
			target = "027_left_footfinger_2_2";
			break;
		case kComponentID_PosesImageBody_RightFoot12Params:
			target = "008_right_footfinger_1_2";
			break;
		case kComponentID_PosesImageBody_LeftFoot12Params:
			target = "029_left_footfinger_1_2";
			break;
		case kComponentID_PosesImageBody_PivotTransformation:
			target = "380_pivot";
			break;
		default:
			return false;
			break;
		}

		Window &mainWindow = Window::instance();

		PoseTargetPanel *targetPanel = dynamic_cast<PoseTargetPanel *>(
		    mainWindow.getPanel(kComponentID_TargetPanel));

		// Check if this Target Panel is not the current one?
		if ((targetPanel == NULL) || (targetPanel->getCategory() != target)) {
			// No? The create it
			int x = mainWindow.getSize().getWidth() - 210;

			mainWindow.removePanel(targetPanel);
			delete targetPanel;

			targetPanel = new PoseTargetPanel(target, Rect(x, 40, 210, 517));

			mainWindow.addPanel(targetPanel);
			targetPanel->createWidgets();
		}

		targetPanel->show_all();

		return true;
	}

	return false;
}

bool PoseTargetSelectionListener::keyType(unsigned char key, Component *source)
{
	return false;
}
