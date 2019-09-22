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
 *  File   : ComponentID.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef COMPONENTID_H
#define COMPONENTID_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdint>


// Construct a integer composed from a four char literal.
// FOUR_CHAR_CONST('D','U','M','Y'); is a substitute of the uint32_t literal
// 'DUMY'
#ifndef FOUR_CHAR_CONST
#define FOUR_CHAR_CONST(a, b, c, d)                                            \
	(((char)(a)) << 24 | ((char)(b)) << 16 | ((char)(c)) << 8 | ((char)(d)))
#endif

static const uint32_t kComponentID_Dummy = FOUR_CHAR_CONST('D', 'M', 'M', 'Y');

// static const uint32_t kComponentID_TextEntry     = FOUR_CHAR_CONST ('T', 'X',
// 'T', 'A');
static const uint32_t kComponentID_TextPanel =
    FOUR_CHAR_CONST('T', 'X', 'P', 'A');

// ------------------------------------------------------------------
// The Footer Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_FooterPanel =
    FOUR_CHAR_CONST('p', 'a', 'F', 'o');

// The Image ID's of the FooterPanel (kComponentID_FooterPanel)
static const uint32_t kComponentID_ImageFooterPanel_Frames_01 =
    kComponentID_Dummy;

// ------------------------------------------------------------------
// The Utilitybar Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_UtilitybarPanel =
    FOUR_CHAR_CONST('p', 'a', 'U', 't');

// The Image ID's of the  UtilitybarPanel (kComponentID_UtilitybarPanel)
static const uint32_t kComponentID_ImageUtilitybar_Frames_01 =
    kComponentID_Dummy;

// ------------------------------------------------------------------
// The Bottom Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_BottomPanel =
    FOUR_CHAR_CONST('p', 'a', 'B', 't');

// ------------------------------------------------------------------
// The View Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_ViewPanel =
    FOUR_CHAR_CONST('p', 'a', 'V', 'B');

// The Image ID's of the ViewPanel (kComponentID_ViewPanel)

// ------------------------------------------------------------------
// The Body Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_BodyPanel =
    FOUR_CHAR_CONST('p', 'a', 'B', 'o');

// The Image ID's of the  BodyPanel (kComponentID_BodyPanel)
static const uint32_t kComponentID_ImageBody_HeadParams =
    FOUR_CHAR_CONST('H', 'e', 'a', 'd');
static const uint32_t kComponentID_ImageBody_ShoulderParams =
    FOUR_CHAR_CONST('S', 'h', 'l', 'd');
// static const uint32_t kComponentID_ImageBody_RightShoulderParams =
// FOUR_CHAR_CONST ('R', 'S', 'h', 'l');
static const uint32_t kComponentID_ImageBody_NeckParams =
    FOUR_CHAR_CONST('N', 'e', 'c', 'k');
static const uint32_t kComponentID_ImageBody_UpperArmParams =
    FOUR_CHAR_CONST('U', 'A', 'r', 'm');
// static const uint32_t kComponentID_ImageBody_RightUpperArmParams =
// FOUR_CHAR_CONST ('R', 'U', 'A', 'r');
static const uint32_t kComponentID_ImageBody_LowerArmParams =
    FOUR_CHAR_CONST('L', 'A', 'r', 'm');
// static const uint32_t kComponentID_ImageBody_RightLowerArmParams =
// FOUR_CHAR_CONST ('R', 'L', 'A', 'r');
static const uint32_t kComponentID_ImageBody_AbdomenParams =
    FOUR_CHAR_CONST('A', 'b', 'd', 'o');
static const uint32_t kComponentID_ImageBody_TorsoParams =
    FOUR_CHAR_CONST('T', 'o', 'r', 's');
// static const uint32_t kComponentID_ImageBody_TorsoParams2        =
// FOUR_CHAR_CONST ('T', 'o', 'r', '2');
static const uint32_t kComponentID_ImageBody_HandParams =
    FOUR_CHAR_CONST('H', 'a', 'n', 'd');
// static const uint32_t kComponentID_ImageBody_RightHandParams     =
// FOUR_CHAR_CONST ('R', 'H', 'a', 'n');
static const uint32_t kComponentID_ImageBody_PelvisParams =
    FOUR_CHAR_CONST('P', 'e', 'l', 'v');
static const uint32_t kComponentID_ImageBody_UpperLegParams =
    FOUR_CHAR_CONST('U', 'L', 'e', 'g');
// static const uint32_t kComponentID_ImageBody_RightUpperLegParams =
// FOUR_CHAR_CONST ('R', 'U', 'L', 'g');
static const uint32_t kComponentID_ImageBody_LowerLegParams =
    FOUR_CHAR_CONST('L', 'L', 'e', 'g');
// static const uint32_t kComponentID_ImageBody_RightLowerLegParams =
// FOUR_CHAR_CONST ('R', 'L', 'L', 'g');
static const uint32_t kComponentID_ImageBody_FootParams =
    FOUR_CHAR_CONST('F', 'o', 'o', 't');
// static const uint32_t kComponentID_ImageBody_RightFootParams     =
// FOUR_CHAR_CONST ('R', 'F', 'o', 't'); static const uint32_t
// kComponentID_ImageBody_FootParams2         = FOUR_CHAR_CONST ('F', 'o', '2',
// 't'); static const uint32_t kComponentID_ImageBody_RightFootParams2    =
// FOUR_CHAR_CONST ('R', 'F', '2', 't');

// ------------------------------------------------------------------
// The Tooltip Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_TooltipPanel =
    FOUR_CHAR_CONST('p', 'a', 'T', 'T');

// The Image ID's of the  TooltipPanel (kComponentID_TooltipPanel)

// ------------------------------------------------------------------
// The Face Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_FacePanel =
    FOUR_CHAR_CONST('p', 'a', 'F', 'a');

// The Image ID's of the  FacePanel (kComponentID_FacePanel)
static const uint32_t kComponentID_FacePanel_ForeheadParams =
    FOUR_CHAR_CONST('F', 'r', 'h', 'd');
static const uint32_t kComponentID_FacePanel_EyesParams =
    FOUR_CHAR_CONST('E', 'y', 'e', 's');
static const uint32_t kComponentID_FacePanel_NoseParams =
    FOUR_CHAR_CONST('N', 'o', 's', 'e');
static const uint32_t kComponentID_FacePanel_EarsParams =
    FOUR_CHAR_CONST('E', 'a', 'r', 's');
static const uint32_t kComponentID_FacePanel_MouthParams =
    FOUR_CHAR_CONST('M', 'o', 't', 'h');
static const uint32_t kComponentID_FacePanel_ChinJawParams =
    FOUR_CHAR_CONST('C', 'h', 'J', 'w');
static const uint32_t kComponentID_FacePanel_Cheek =
    FOUR_CHAR_CONST('C', 'h', 'e', 'k');

// ------------------------------------------------------------------
// The Teeth Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_TeethPanel =
    FOUR_CHAR_CONST('p', 'a', 'T', 'e');

// The Image ID's of the  TeethPanel (kComponentID_TeethPanel)
static const uint32_t kComponentID_TeethPanel_TongueParams =
    FOUR_CHAR_CONST('T', 'n', 'g', 'e');
static const uint32_t kComponentID_TeethPanel_TeethParams =
    FOUR_CHAR_CONST('T', 't', 'h', 'e');
/*
static const uint32_t kComponentID_TeethPanel_UpperTeethRightParams  =
FOUR_CHAR_CONST ('U', 'p', 'T', 'R'); static const uint32_t
kComponentID_TeethPanel_UpperTeethCenterParams = FOUR_CHAR_CONST ('U', 'p', 'T',
'C'); static const uint32_t kComponentID_TeethPanel_UpperTeethLeftParams   =
FOUR_CHAR_CONST ('U', 'p', 'T', 'L'); static const uint32_t
kComponentID_TeethPanel_LowerTeethRightParams  = FOUR_CHAR_CONST ('L', 'w', 'T',
'R'); static const uint32_t kComponentID_TeethPanel_LowerTeethCenterParams =
FOUR_CHAR_CONST ('L', 'w', 'T', 'C'); static const uint32_t
kComponentID_TeethPanel_LowerTeethLeftParams   = FOUR_CHAR_CONST ('L', 'w', 'T',
'L');
*/

// ------------------------------------------------------------------
// The Hands Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_HandsPanel =
    FOUR_CHAR_CONST('p', 'a', 'H', 'a');

// The Image ID's of the  HandsPanel (kComponentID_HandsPanel)
// static const uint32_t kComponentID_HandsPanel_RingfingerRightParams   =
// FOUR_CHAR_CONST ('R', 'n', 'g', 'R'); static const uint32_t
// kComponentID_HandsPanel_MiddlefingerRightParams = FOUR_CHAR_CONST ('M', 'd',
// 'd', 'R');
static const uint32_t kComponentID_HandsPanel_MiddlefingerLeftParams =
    FOUR_CHAR_CONST('M', 'd', 'd', 'L');
static const uint32_t kComponentID_HandsPanel_RingfingerLeftParams =
    FOUR_CHAR_CONST('R', 'n', 'g', 'L');
// static const uint32_t kComponentID_HandsPanel_ForefingerRightParams   =
// FOUR_CHAR_CONST ('F', 'r', 'f', 'R');
static const uint32_t kComponentID_HandsPanel_ForefingerLeftParams =
    FOUR_CHAR_CONST('F', 'r', 'f', 'L');
// static const uint32_t kComponentID_HandsPanel_LittlefingerRightParams =
// FOUR_CHAR_CONST ('L', 't', 't', 'R'); static const uint32_t
// kComponentID_HandsPanel_HandRightParams2        = FOUR_CHAR_CONST ('H', 'n',
// 'R', '2'); static const uint32_t kComponentID_HandsPanel_PollexRightParams =
// FOUR_CHAR_CONST ('P', 'l', 'l', 'R');
static const uint32_t kComponentID_HandsPanel_PollexLeftParams =
    FOUR_CHAR_CONST('P', 'l', 'l', 'L');
// static const uint32_t kComponentID_HandsPanel_HandLeftParams2         =
// FOUR_CHAR_CONST ('H', 'n', 'L', '2');
static const uint32_t kComponentID_HandsPanel_LittlefingerLeftParams =
    FOUR_CHAR_CONST('L', 't', 't', 'L');

// ------------------------------------------------------------------
// The Toolbar Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_ToolbarPanel =
    FOUR_CHAR_CONST('p', 'a', 'T', 'B');

// The Image ID's of the  ToolbarPanel (kComponentID_ToolbarPanel)
static const uint32_t kComponentID_ImageToolbarPanel_CharacterSettings =
    FOUR_CHAR_CONST('C', 'h', 'S', 'e');
static const uint32_t kComponentID_ImageToolbarPanel_BodyDetailsRealistic =
    FOUR_CHAR_CONST('B', 'D', 'R', 'e');
// static const uint32_t kComponentID_ImageToolbarPanel_BodyDetailsFantasy   =
// FOUR_CHAR_CONST ('B', 'D', 'F', 'a');
static const uint32_t kComponentID_ImageToolbarPanel_Poses =
    FOUR_CHAR_CONST('P', 'o', 's', 'e');
// static const uint32_t kComponentID_ImageToolbarPanel_Lights               =
// FOUR_CHAR_CONST ('L', 'i', 'g', 'h'); static const uint32_t
// kComponentID_ImageToolbarPanel_Expressions          = FOUR_CHAR_CONST ('E',
// 'x', 'p', 'r');
static const uint32_t kComponentID_ImageToolbarPanel_Asymmetry =
    FOUR_CHAR_CONST('A', 's', 'i', 'm');
static const uint32_t kComponentID_ImageToolbarPanel_Hairs =
    FOUR_CHAR_CONST('H', 'a', 'i', 'r');
// static const uint32_t kComponentID_ImageToolbarPanel_Materials            =
// FOUR_CHAR_CONST ('M', 'a', 't', 'e');
static const uint32_t kComponentID_ImageToolbarPanel_MorphingList =
    FOUR_CHAR_CONST('M', 'o', 'L', 'i');
static const uint32_t kComponentID_ImageToolbarPanel_Animation =
    FOUR_CHAR_CONST('A', 'n', 'i', 'm');
static const uint32_t kComponentID_ImageToolbarPanel_BodySettings =
    FOUR_CHAR_CONST('B', 'o', 'S', 'e');
static const uint32_t kComponentID_ImageToolbarPanel_PoseBodySettings =
    FOUR_CHAR_CONST('P', 'B', 'S', 't');

// ------------------------------------------------------------------
// The Target Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_TargetPanel =
    FOUR_CHAR_CONST('p', 'a', 'T', 'P');
static const uint32_t kComponentID_CloseTargetPanel =
    FOUR_CHAR_CONST('p', 'a', 'C', 'T');
static const uint32_t kComponentID_PrevTargetPanel =
    FOUR_CHAR_CONST('p', 'a', 'P', 'T');
static const uint32_t kComponentID_NextTargetPanel =
    FOUR_CHAR_CONST('p', 'a', 'N', 'T');
static const uint32_t kComponentID_TargetPanel_Target = kComponentID_Dummy;

// ------------------------------------------------------------------
// The Poses Body Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_PosesBodyPanel =
    FOUR_CHAR_CONST('p', 'a', 'P', 'B');

// The Image ID's of the  PosesBodyPanel (kComponentID_PosesBodyPanel)
static const uint32_t kComponentID_PosesImageBody_RightCollarParams =
    FOUR_CHAR_CONST('P', 'R', 'C', 'l');
static const uint32_t kComponentID_PosesImageBody_HeadParams =
    FOUR_CHAR_CONST('P', 'H', 'd', 'e');
static const uint32_t kComponentID_PosesImageBody_LeftCollarParams =
    FOUR_CHAR_CONST('P', 'L', 'C', 'l');
static const uint32_t kComponentID_PosesImageBody_RightUpperArmParams =
    FOUR_CHAR_CONST('P', 'R', 'U', 'A');
static const uint32_t kComponentID_PosesImageBody_NeckParams =
    FOUR_CHAR_CONST('P', 'N', 'c', 'k');
static const uint32_t kComponentID_PosesImageBody_LeftUpperArmParams =
    FOUR_CHAR_CONST('P', 'L', 'U', 'A');
static const uint32_t kComponentID_PosesImageBody_TorsoParams =
    FOUR_CHAR_CONST('P', 'T', 'r', 's');
static const uint32_t kComponentID_PosesImageBody_LeftLowerArmParams =
    FOUR_CHAR_CONST('P', 'L', 'L', 'A');
static const uint32_t kComponentID_PosesImageBody_RightLowerArmParams =
    FOUR_CHAR_CONST('P', 'R', 'L', 'A');
static const uint32_t kComponentID_PosesImageBody_PivotTransformation =
    FOUR_CHAR_CONST('P', 'B', 'B', 'u');
static const uint32_t kComponentID_PosesImageBody_LeftHandParams =
    FOUR_CHAR_CONST('P', 'L', 'H', 'n');
static const uint32_t kComponentID_PosesImageBody_LeftUpperLegParams =
    FOUR_CHAR_CONST('P', 'L', 'U', 'L');
static const uint32_t kComponentID_PosesImageBody_RightUpperLegParams =
    FOUR_CHAR_CONST('P', 'R', 'U', 'L');
static const uint32_t kComponentID_PosesImageBody_RightHandParams =
    FOUR_CHAR_CONST('P', 'R', 'H', 'n');
static const uint32_t kComponentID_PosesImageBody_LeftLowerLegParams =
    FOUR_CHAR_CONST('P', 'L', 'L', 'L');
static const uint32_t kComponentID_PosesImageBody_RightLowerLegParams =
    FOUR_CHAR_CONST('P', 'R', 'L', 'L');
static const uint32_t kComponentID_PosesImageBody_LeftFootParams =
    FOUR_CHAR_CONST('P', 'L', 'F', 't');
static const uint32_t kComponentID_PosesImageBody_RightFootParams =
    FOUR_CHAR_CONST('P', 'R', 'F', 't');
static const uint32_t kComponentID_PosesImageBody_LeftRingfinger3Params =
    FOUR_CHAR_CONST('P', 'L', 'R', '3');
static const uint32_t kComponentID_PosesImageBody_LeftMiddlefinger3Params =
    FOUR_CHAR_CONST('P', 'L', 'M', '3');
static const uint32_t kComponentID_PosesImageBody_LeftForefinger3Params =
    FOUR_CHAR_CONST('P', 'L', 'F', '3');
static const uint32_t kComponentID_PosesImageBody_LeftRingfinger2Params =
    FOUR_CHAR_CONST('P', 'L', 'R', '2');
static const uint32_t kComponentID_PosesImageBody_LeftMiddlefinger2Params =
    FOUR_CHAR_CONST('P', 'L', 'M', '2');
static const uint32_t kComponentID_PosesImageBody_LeftForefinger2Params =
    FOUR_CHAR_CONST('P', 'L', 'F', '2');
static const uint32_t kComponentID_PosesImageBody_LeftLittlefinger3Params =
    FOUR_CHAR_CONST('P', 'L', 'L', '3');
static const uint32_t kComponentID_PosesImageBody_LeftMiddlefinger1Params =
    FOUR_CHAR_CONST('P', 'L', 'M', '1');
static const uint32_t kComponentID_PosesImageBody_LeftForefinger1Params =
    FOUR_CHAR_CONST('P', 'L', 'F', '1');
static const uint32_t kComponentID_PosesImageBody_LeftLittlefinger2Params =
    FOUR_CHAR_CONST('P', 'L', 'L', '2');
static const uint32_t kComponentID_PosesImageBody_LeftRingfinger1Params =
    FOUR_CHAR_CONST('P', 'L', 'R', '1');
static const uint32_t kComponentID_PosesImageBody_LeftPollex3Params =
    FOUR_CHAR_CONST('P', 'L', 'P', '3');
static const uint32_t kComponentID_PosesImageBody_LeftLittlefinger1Params =
    FOUR_CHAR_CONST('P', 'L', 'L', '1');
static const uint32_t kComponentID_PosesImageBody_LeftPollex2Params =
    FOUR_CHAR_CONST('P', 'L', 'P', '2');
static const uint32_t kComponentID_PosesImageBody_LeftPollex1Params =
    FOUR_CHAR_CONST('P', 'L', 'P', '1');
static const uint32_t kComponentID_PosesImageBody_RightForefinger3Params =
    FOUR_CHAR_CONST('P', 'R', 'F', '3');
static const uint32_t kComponentID_PosesImageBody_RightMiddlefinger3Params =
    FOUR_CHAR_CONST('P', 'R', 'M', '3');
static const uint32_t kComponentID_PosesImageBody_RightRingfinger3Params =
    FOUR_CHAR_CONST('P', 'R', 'R', '3');
static const uint32_t kComponentID_PosesImageBody_RightForefinger2Params =
    FOUR_CHAR_CONST('P', 'R', 'F', '2');
static const uint32_t kComponentID_PosesImageBody_RightMiddlefinger2Params =
    FOUR_CHAR_CONST('P', 'R', 'M', '2');
static const uint32_t kComponentID_PosesImageBody_RightRingfinger2Params =
    FOUR_CHAR_CONST('P', 'R', 'R', '2');
static const uint32_t kComponentID_PosesImageBody_RightForefinger1Params =
    FOUR_CHAR_CONST('P', 'R', 'F', '1');
static const uint32_t kComponentID_PosesImageBody_RightMiddlefinger1Params =
    FOUR_CHAR_CONST('P', 'R', 'M', '1');
static const uint32_t kComponentID_PosesImageBody_RightLittlefinger3Params =
    FOUR_CHAR_CONST('P', 'R', 'L', '3');
static const uint32_t kComponentID_PosesImageBody_RightPollex3Params =
    FOUR_CHAR_CONST('P', 'R', 'P', '3');
static const uint32_t kComponentID_PosesImageBody_RightRingfinger1Params =
    FOUR_CHAR_CONST('P', 'R', 'R', '1');
static const uint32_t kComponentID_PosesImageBody_RightLittlefinger2Params =
    FOUR_CHAR_CONST('P', 'R', 'L', '2');
static const uint32_t kComponentID_PosesImageBody_RightPollex2Params =
    FOUR_CHAR_CONST('P', 'R', 'P', '2');
static const uint32_t kComponentID_PosesImageBody_RightPollex1Params =
    FOUR_CHAR_CONST('P', 'R', 'P', '1');
static const uint32_t kComponentID_PosesImageBody_RightLittlefinger1Params =
    FOUR_CHAR_CONST('P', 'R', 'L', '1');
static const uint32_t kComponentID_PosesImageBody_RightFoot42Params =
    FOUR_CHAR_CONST('P', 'R', '4', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot52Params =
    FOUR_CHAR_CONST('P', 'R', '5', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot51Params =
    FOUR_CHAR_CONST('P', 'R', '5', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot51Params =
    FOUR_CHAR_CONST('P', 'L', '5', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot52Params =
    FOUR_CHAR_CONST('P', 'L', '5', '2');
static const uint32_t kComponentID_PosesImageBody_LeftFoot42Params =
    FOUR_CHAR_CONST('P', 'L', '4', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot32Params =
    FOUR_CHAR_CONST('P', 'R', '3', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot31Params =
    FOUR_CHAR_CONST('P', 'R', '3', '1');
static const uint32_t kComponentID_PosesImageBody_RightFoot41Params =
    FOUR_CHAR_CONST('P', 'R', '4', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot41Params =
    FOUR_CHAR_CONST('P', 'L', '4', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot31Params =
    FOUR_CHAR_CONST('P', 'L', '3', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot32Params =
    FOUR_CHAR_CONST('P', 'L', '3', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot22Params =
    FOUR_CHAR_CONST('P', 'R', '2', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot21Params =
    FOUR_CHAR_CONST('P', 'R', '2', '1');
static const uint32_t kComponentID_PosesImageBody_RightFoot11Params =
    FOUR_CHAR_CONST('P', 'R', '1', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot11Params =
    FOUR_CHAR_CONST('P', 'L', '1', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot21Params =
    FOUR_CHAR_CONST('P', 'L', '2', '1');
static const uint32_t kComponentID_PosesImageBody_LeftFoot22Params =
    FOUR_CHAR_CONST('P', 'L', '2', '2');
static const uint32_t kComponentID_PosesImageBody_RightFoot12Params =
    FOUR_CHAR_CONST('P', 'R', '1', '2');
static const uint32_t kComponentID_PosesImageBody_LeftFoot12Params =
    FOUR_CHAR_CONST('P', 'L', '1', '2');

// ------------------------------------------------------------------
// The Character Setting Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_CharacterSettingPanel =
    FOUR_CHAR_CONST('p', 'a', 'C', 'A');

// The ImageSlider ID's of the  CharacterSettingPanel
// (kComponentID_CharacterSettingPanel)
static const uint32_t kComponentID_CharacterSettingPanel_EndoLeft =
    FOUR_CHAR_CONST('S', 'E', 'n', 'L');
static const uint32_t kComponentID_CharacterSettingPanel_EndoMiddle =
    FOUR_CHAR_CONST('S', 'E', 'n', 'M');
static const uint32_t kComponentID_CharacterSettingPanel_EndoRight =
    FOUR_CHAR_CONST('S', 'E', 'n', 'R');
static const uint32_t kComponentID_CharacterSettingPanel_MesoLeft =
    FOUR_CHAR_CONST('S', 'M', 'e', 'L');
static const uint32_t kComponentID_CharacterSettingPanel_MesoMiddle =
    FOUR_CHAR_CONST('S', 'M', 'e', 'M');
static const uint32_t kComponentID_CharacterSettingPanel_MesoRight =
    FOUR_CHAR_CONST('S', 'M', 'e', 'R');
static const uint32_t kComponentID_CharacterSettingPanel_EctoLeft =
    FOUR_CHAR_CONST('S', 'E', 'c', 'L');
static const uint32_t kComponentID_CharacterSettingPanel_EctoMiddle =
    FOUR_CHAR_CONST('S', 'E', 'c', 'M');
static const uint32_t kComponentID_CharacterSettingPanel_EctoRight =
    FOUR_CHAR_CONST('S', 'E', 'c', 'R');

static const uint32_t kComponentID_CharacterSettingPanel_Age =
    FOUR_CHAR_CONST('A', 'g', 'e', 'X');
static const uint32_t kComponentID_CharacterSettingPanel_MuscleSize =
    FOUR_CHAR_CONST('M', 's', 'S', 'z');
static const uint32_t kComponentID_CharacterSettingPanel_Breast =
    FOUR_CHAR_CONST('B', 'r', 's', 't');
static const uint32_t kComponentID_CharacterSettingPanel_Shape =
    FOUR_CHAR_CONST('S', 'h', 'p', 'a');

// ------------------------------------------------------------------
// The Splash Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_SplashPanel =
    FOUR_CHAR_CONST('S', 'p', 'l', 's');

// ------------------------------------------------------------------
// The Clothes Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_ClothesPanel =
    FOUR_CHAR_CONST('C', 'l', 't', 'h');

// ------------------------------------------------------------------
// The Utilities Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_UtilitiesPanel =
    FOUR_CHAR_CONST('U', 't', 'l', 't');

static const uint32_t kComponentID_UtilitiesPanel_Interpolations =
    FOUR_CHAR_CONST('I', 'n', 't', 'p');
static const uint32_t kComponentID_UtilitiesPanel_FrameRate =
    FOUR_CHAR_CONST('F', 'r', 'm', 'R');
static const uint32_t kComponentID_UtilitiesPanel_Reset =
    FOUR_CHAR_CONST('R', 's', 't', 'e');

// ------------------------------------------------------------------
// The Body Settings Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_BsCategoryPanel =
    FOUR_CHAR_CONST('B', 's', 'C', 'a');
static const uint32_t kComponentID_BsCategoryPanel_MyBS =
    FOUR_CHAR_CONST('M', 'y', 'B', 'S');
static const uint32_t kComponentID_BsCategoryPanel_Category1 =
    FOUR_CHAR_CONST('C', 'a', '0', '1');
static const uint32_t kComponentID_BsCategoryPanel_Category2 =
    FOUR_CHAR_CONST('C', 'a', '0', '2');
static const uint32_t kComponentID_BsCategoryPanel_Category3 =
    FOUR_CHAR_CONST('C', 'a', '0', '3');
static const uint32_t kComponentID_BsCategoryPanel_Category4 =
    FOUR_CHAR_CONST('C', 'a', '0', '4');
static const uint32_t kComponentID_BsCategoryPanel_Category5 =
    FOUR_CHAR_CONST('C', 'a', '0', '5');
static const uint32_t kComponentID_BsCategoryPanel_Category6 =
    FOUR_CHAR_CONST('C', 'a', '0', '6');
static const uint32_t kComponentID_BsCategoryPanel_Category7 =
    FOUR_CHAR_CONST('C', 'a', '0', '7');
static const uint32_t kComponentID_BsCategoryPanel_Category8 =
    FOUR_CHAR_CONST('C', 'a', '0', '8');
static const uint32_t kComponentID_BsCategoryPanel_Category9 =
    FOUR_CHAR_CONST('C', 'a', '0', '9');
static const uint32_t kComponentID_BsCategoryPanel_Category10 =
    FOUR_CHAR_CONST('C', 'a', '1', '0');
static const uint32_t kComponentID_BsCategoryPanel_Category11 =
    FOUR_CHAR_CONST('C', 'a', '1', '1');
static const uint32_t kComponentID_BsCategoryPanel_Category12 =
    FOUR_CHAR_CONST('C', 'a', '1', '2');
static const uint32_t kComponentID_BsCategoryPanel_Category13 =
    FOUR_CHAR_CONST('C', 'a', '1', '3');
static const uint32_t kComponentID_BsCategoryPanel_Category14 =
    FOUR_CHAR_CONST('C', 'a', '1', '4');
static const uint32_t kComponentID_BsCategoryPanel_Category15 =
    FOUR_CHAR_CONST('C', 'a', '1', '5');
static const uint32_t kComponentID_BsCategoryPanel_Category16 =
    FOUR_CHAR_CONST('C', 'a', '1', '6');
static const uint32_t kComponentID_BsCategoryPanel_Category17 =
    FOUR_CHAR_CONST('C', 'a', '1', '7');

// ------------------------------------------------------------------
// The Poses Body Settings Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_PosesBsCategoryPanel =
    FOUR_CHAR_CONST('P', 'B', 'C', 'a');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category1 =
    FOUR_CHAR_CONST('P', 'C', '0', '1');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category2 =
    FOUR_CHAR_CONST('P', 'C', '0', '2');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category3 =
    FOUR_CHAR_CONST('P', 'C', '0', '3');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category4 =
    FOUR_CHAR_CONST('P', 'C', '0', '4');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category5 =
    FOUR_CHAR_CONST('P', 'C', '0', '5');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category6 =
    FOUR_CHAR_CONST('P', 'C', '0', '6');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category7 =
    FOUR_CHAR_CONST('P', 'C', '0', '7');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category8 =
    FOUR_CHAR_CONST('P', 'C', '0', '8');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category9 =
    FOUR_CHAR_CONST('P', 'C', '0', '9');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category10 =
    FOUR_CHAR_CONST('P', 'C', '1', '0');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category11 =
    FOUR_CHAR_CONST('P', 'C', '1', '1');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category12 =
    FOUR_CHAR_CONST('P', 'C', '1', '2');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category13 =
    FOUR_CHAR_CONST('P', 'C', '1', '3');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category14 =
    FOUR_CHAR_CONST('P', 'C', '1', '4');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category15 =
    FOUR_CHAR_CONST('P', 'C', '1', '5');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category16 =
    FOUR_CHAR_CONST('P', 'C', '1', '6');
static const uint32_t kComponentID_PosesBsCategoryPanel_Category17 =
    FOUR_CHAR_CONST('P', 'C', '1', '7');
#endif // COMPONENTID_H 1
