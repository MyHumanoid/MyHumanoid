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
 *  File   : PoseTargetSliderListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <animorph/Mesh.h>
#include <mhgui/ImageSlider.h>
#include <mhgui/CGUtilities.h>
#include <mhgui/GLUTWrapper.h>
#include <stdio.h>
#include <assert.h>
#include "PoseTargetSliderListener.h"
#include "TargetSlider.h"
#include "Global.h"

using namespace Animorph;
using namespace std;

const static float kPoseThreshold = 0.04;

PoseTargetSliderListener::PoseTargetSliderListener()
: AbstractListener(),
  lastTargetName()
{
}

PoseTargetSliderListener::~PoseTargetSliderListener()
{
}

bool PoseTargetSliderListener::mouseOver (const Point& inMousePos, Component *source)
{
  return false;
}

bool PoseTargetSliderListener::mouseOut (const Point& inMousePos, Component *source)
{
  return false;
}

bool PoseTargetSliderListener::mouseWheel    (const Point& inMousePos, int inButton, Component *source )
{
  #ifdef GLUT_WHEEL_UP
    return mouseDragged(inMousePos, source);
  #else
    return false;
  #endif
}

bool PoseTargetSliderListener::mouseDragged (const Point& inMousePos, Component *source)
{
  TargetSlider *imgSliderSource = dynamic_cast<TargetSlider *>(source); // req. RTTI!
  assert(imgSliderSource); // Check if this is really a TargetSlider object?

  if(lastTargetName != imgSliderSource->getTargetName())
  {
    lastTargetName = imgSliderSource->getTargetName();
    lastTargetValue = imgSliderSource->getSliderValue ();
  }
  else
  {
    if(Global::instance().getSubdivision())
    {
      Global::instance().setLightMesh(true);
    }

    float diffValue = lastTargetValue - imgSliderSource->getSliderValue ();
    if(diffValue < -kPoseThreshold || diffValue > kPoseThreshold)
    {
      lastTargetValue = imgSliderSource->getSliderValue ();

      Global &global = Global::instance ();
      Mesh *mesh = global.getMesh ();
      mesh->setPose (imgSliderSource->getTargetName (),
                    imgSliderSource->getSliderValue ());
    }
  }

  return true;
}

bool PoseTargetSliderListener::mousePressed(const Point& inMousePos, int button, Component *source)
{
  return false;
}

bool PoseTargetSliderListener::mouseReleased (const Point& inMousePos, int button, Component *source)
{
  TargetSlider *imgSliderSource = dynamic_cast<TargetSlider *>(source); // req. RTTI!
  assert(imgSliderSource); // Check if this is really a TargetSlider object?

  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();

  mesh->setPose (imgSliderSource->getTargetName (),
		 imgSliderSource->getSliderValue ());

  mesh->calcNormals ();

  if(Global::instance().getSubdivision())
  {
    mesh->calcSubsurf ();
    Global::instance().setLightMesh(false);
  }

  return false;
}

bool PoseTargetSliderListener::keyType (unsigned char key, Component *source)
{
  return false;
}
