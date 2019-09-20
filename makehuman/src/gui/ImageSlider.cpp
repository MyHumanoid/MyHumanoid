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
 *  File   : ImageSlider.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Rect.h"
#include "gui/ImageSlider.h"
#include "gui/CGUtilities.h"

#include "gui/ImageSliderSysListener.h"
#include "gui/GLUTWrapper.h"

#include <iostream>

using namespace std;
using namespace Animorph;

namespace mhgui {

ImageSlider::ImageSlider (uint32_t      inImageID,
                          const std::string& inSliderImageFilename,
                          const Rect&   inGeometry,
                          float         inMinValue,
                          float         inMaxValue)
    : Image (inImageID, inSliderImageFilename, inGeometry),
    minValue((inMinValue < inMaxValue) ? inMinValue : 0.0),
    maxValue((inMinValue < inMaxValue) ? inMaxValue : 1.0),
    sliderValue(minValue),
    valueStep(0.01),
    oldMouseX(0),
    overlayMultiplier(2),
    imageSliderSysListener(new ImageSliderSysListener())
{
  setSysListener(imageSliderSysListener);
}

ImageSlider::~ImageSlider()
{
  delete imageSliderSysListener;
}

void ImageSlider::draw ()
{
  if (isVisible())
  {
    char sValue[8];
    int valueLength;
    Color c;

    snprintf(sValue, sizeof(sValue), "%1.2f",sliderValue);
    valueLength = cgutils::getFontLength(GLUT_BITMAP_HELVETICA_10, sValue);

    cgutils::enableBlend ();

    Rect rect(getAbsoluteRect());
    rect.resizeBy(Size(0, -10));

    if (lazyLoadTexture())
    {
      cgutils::drawSquareFillTexture (rect, getAlpha(), getTextures());
    }

    cgutils::disableBlend ();

    if (sliderValue == 0)
    {
      c.rgb (1, 1, 1);
    }
    else
    {
      c.rgb (1, 0, 0);
    }

    Point textPos(getAbsolutePosition().getX() + (getSize().getWidth() - valueLength) / 2,
                  getAbsolutePosition().getY() + getSize().getHeight());

    cgutils::drawString (textPos, GLUT_BITMAP_HELVETICA_10, sValue, c);
  }
}

void ImageSlider::drawOverlay ()
{
  if (isVisible())
  {
    cgutils::enableBlend ();
    if (isLastMouseOver())
    {
      Rect rect(getAbsolutePosition().getX() - ((overlayMultiplier - 1) * getSize().getWidth()) / 2,
                getAbsolutePosition().getY() - ((overlayMultiplier - 1) * (getSize().getHeight() - 10)),
                getSize().getWidth() * overlayMultiplier,
                (getSize().getHeight() - 10) * overlayMultiplier);

      // TODO: do background color configurable!
      cgutils::drawSquareFill (rect, Color (0.0, 0.3, 0.8, 0.5));

      if (lazyLoadTexture(true))
      {
        cgutils::drawSquareFillTexture (rect, getAlpha(), getTexturesOver());
      }
    }
    cgutils::disableBlend ();
  }
}

float ImageSlider::getSliderValue ()
{
  return sliderValue;
}

void ImageSlider::setSliderValue (float newValue)
{
  if(newValue < valueStep && newValue > -valueStep) newValue = 0;
  if(newValue < minValue) sliderValue = minValue;
  else if(newValue > maxValue) sliderValue = maxValue;
  else sliderValue = newValue;
}

void ImageSlider::setOldMouseX (int mouseX)
{
  oldMouseX = mouseX;
}

int ImageSlider::getOldMouseX ()
{
  return oldMouseX;
}

void ImageSlider::decreaseValue (int n)
{
  setSliderValue(sliderValue - (n * valueStep));
}

void ImageSlider::increaseValue (int n)
{
  setSliderValue(sliderValue + (n * valueStep));
}

} // namespace mhgui
