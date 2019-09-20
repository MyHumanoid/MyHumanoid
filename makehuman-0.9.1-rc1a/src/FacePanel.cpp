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
 *  File   : FacePanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "FacePanel.h"
#include "ImageListener.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Tooltip.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

FacePanel::FacePanel ()
    : Panel (kComponentID_FacePanel, Rect(64,16,128,192))

{
}

FacePanel::~FacePanel ()
{
  for_each (imageVector.begin (), imageVector.end (), deleteFunctor <Image*> ());
}


void FacePanel::createWidgets ()
{
  Window &mainWindow = Window::instance ();
  Panel *tooltipPanel = mainWindow.getPanel (kComponentID_TooltipPanel);
  assert(tooltipPanel);

  const Color color_red (1.0, 0.0, 0.0);
  const float alpha = 1.0;
  const Point kTooltipPos(70, 12);

  Image *face;

  // -------------------------------------------------------------------------
  face= new Image(kComponentID_Dummy,
                  searchPixmapFile ("ui/face_01.png"),
                  Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_02.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_03.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_04.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_05.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_ForeheadParams,
                   searchPixmapFile ("ui/face_06.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Forehead parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_07.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_08.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_09.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_EyesParams,
                   searchPixmapFile ("ui/face_10.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Eyes parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_11.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_12.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_NoseParams,
                   searchPixmapFile ("ui/face_13.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Nose parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_Cheek,
                   searchPixmapFile ("ui/face_14.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Cheek parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_EarsParams,
                   searchPixmapFile ("ui/face_15.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Ears parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_16.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_MouthParams,
                   searchPixmapFile ("ui/face_17.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Mouth parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_18.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_19.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_20.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_FacePanel_ChinJawParams,
                   searchPixmapFile ("ui/face_21.png"),
                   Rect(0,0,32,32));
  face->setListener(&imgListener1);
  face->setTooltip(Tooltip("Chin Jaw parameters", kTooltipPos, color_red, tooltipPanel));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_22.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_23.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

  // -------------------------------------------------------------------------
  face = new Image(kComponentID_Dummy,
                   searchPixmapFile ("ui/face_24.png"),
                   Rect(0,0,32,32));
  face->setAlpha (alpha);
  imageVector.push_back (face);
  addWidget (face);

}
