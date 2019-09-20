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
 *  File   : BodyPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "BodyPanel.h"
#include "ImageListener.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Tooltip.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

BodyPanel::BodyPanel ()
    : Panel (kComponentID_BodyPanel, Rect(0,16,64,384))
{
}

BodyPanel::~BodyPanel ()
{
  for_each (imageVector.begin (), imageVector.end (), deleteFunctor <Image*> ());
}

void BodyPanel::createWidgets ()
{
  Window &mainWindow = Window::instance ();

  Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
  assert(tooltipPanel);

  const Color color_red (1.0, 0.0, 0.0);
  const Point kTooltipPos(70, 12);
  const float alpha = 1.0;
  Image *body;

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_TorsoParams,
                   searchPixmapFile ("ui/body_01.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Torso parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_HeadParams,
                   searchPixmapFile ("ui/body_02.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Head parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_ShoulderParams,
                   searchPixmapFile ("ui/body_03.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Shoulders parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_NeckParams,
                   searchPixmapFile ("ui/body_04.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Neck parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_UpperArmParams,
                   searchPixmapFile ("ui/body_05.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Upper arms parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_06.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_LowerArmParams,
                   searchPixmapFile ("ui/body_07.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Lower arms parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_AbdomenParams,
                   searchPixmapFile ("ui/body_08.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Abdomen parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_HandParams,
                   searchPixmapFile ("ui/body_09.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Hands parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_PelvisParams,
                   searchPixmapFile ("ui/body_10.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Pelvis parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_UpperLegParams,
                   searchPixmapFile ("ui/body_11.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Upper legs parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_12.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_13.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_14.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_LowerLegParams,
                   searchPixmapFile ("ui/body_15.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Lower legs parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_16.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_ImageBody_FootParams,
                   searchPixmapFile ("ui/body_17.png"),
                   Rect (0,0,32,32));
  body->setListener(&imgListener);
  body->setTooltip(Tooltip("Feet parameters", kTooltipPos, color_red, tooltipPanel));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

  // -------------------------------------------------------------------------
  body = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/body_18.png"),
                   Rect (0,0,32,32));
  body->setAlpha (alpha);
  imageVector.push_back (body);
  addWidget(body);

}
