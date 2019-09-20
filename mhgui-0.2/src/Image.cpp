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
 *  File   : Image.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/mhgui/Image.h"
#include "../include/mhgui/Tooltip.h"
#include "../include/mhgui/CGUtilities.h"
#include "../include/mhgui/ImageSysListener.h"
#include "../include/mhgui/ImageData.h"
#include "../include/mhgui/GLUTWrapper.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace mhgui {

//constructor
Image::Image (uint32_t      inId,
              const std::string& inFilename,
              const Rect&   inGeometry)
    : Widget(inId, inGeometry),
    imageFilename(inFilename),
    imageFilenameOver(),
    imageFilenameDisabled(),

    textureDisabled(),
    texture(),
    textureOver(),

    textureIsInited(false),
    textureOverIsInited(false),
    textureDisabledIsInited(false),

    imageSysListener(new ImageSysListener()),
    alpha        (1.0),
    overlay (0,0,0,0),
    overlayEffect (false),
    enabled(true),
    kill_mouse_drag(false)
{

   setSysListener(imageSysListener);

   if(inFilename.length() > 4)
   {
     imageFilenameOver     = inFilename.substr(0, inFilename.length() - 4) + "_over.png";
     imageFilenameDisabled = inFilename.substr(0, inFilename.length() - 4) + "_disa.png";
   }
}

Image::~Image()
{
  delete imageSysListener; // remove the listener again
}

void Image::setOverlayTexture(const std::string& inFilename)
{
  imageFilenameOver = inFilename;
}

void Image::setDisabledTexture(const std::string& inFilename)
{
  imageFilenameDisabled = inFilename;
}

void Image::setAlpha(float a)
{
  alpha = a;
}

void Image::show ()
{
  setVisible(true);

  lazyLoadTexture();
}

void Image::hide ()
{
  setVisible(false);
}

//Return the ID assigned
const Texture& Image::getTextures ()
{
  lazyLoadTexture();
  return enabled ? texture : textureDisabled;
}

//Return the ID assigned
const Texture& Image::getTexturesOver ()
{
  lazyLoadTexture(true);
  return textureOver;
}

void Image::setOverlayRectangle (const Color& c)
{
  overlay = c;
  overlayEffect = true;
}

void Image::setOverlayRectangle (bool overlayEffect)
{
  this->overlayEffect = overlayEffect;
}

void Image::drawOverlay()
{
  if (isVisible())
  {
    if (overlayEffect && enabled)
    {
      if(lazyLoadTexture(true))
      {
        cgutils::enableBlend ();
        cgutils::drawSquareFillTexture (getAbsoluteRect(), 1.0, textureOver);
        cgutils::disableBlend ();
      }
      else
      {
        cgutils::enableBlend ();
        cgutils::drawSquareFill (getAbsoluteRect(), overlay);
        cgutils::disableBlend ();
      }
    }
  }
}

//draw function
void Image::draw()
{
  if (isVisible())
  {
    if (lazyLoadTexture())
    {
      cgutils::enableBlend ();
      cgutils::drawSquareFillTexture (getAbsoluteRect(), alpha, getTextures());
      cgutils::disableBlend ();
    }
    else
    {
      cgutils::enableBlend ();
      cgutils::drawSquareFill (getAbsoluteRect(), Color (1.0,1.0,1.0, enabled ? alpha : alpha / 2));
      cgutils::disableBlend ();
    }
  }
}

bool Image::isMouseDragged (const Point& inMousePos)
{
  bool drag = false;

  if(this->kill_mouse_drag == true && isActive()){
    drag = true;
    return drag;
  }
  return Component::isMouseDragged(inMousePos);
}

bool Image::isMouseClick (const Point& inMousePos, int button, int state)
{

  if(isEnabled() == false){
    return false;
  }
  return Component::isMouseClick(inMousePos,button,state);
}

/* ==========================================================================
 * Private Methods
 * ========================================================================== */
/* ========================================================================== */
/** Load a image file lazy whose filename has been initialized in the
 *  constructor.
 *
 * Note: This call justtries to load a image once and remembers if it could be
 * gathered successfully or not. Succeed calls just return if the image is
 * available or not.
 *
 * @return true if the given Image file could be loaded, false otherwise.
 */
/* ========================================================================== */
bool Image::lazyLoadTexture(bool over)
{
  bool &isInited = (over ? textureOverIsInited : (enabled ? textureIsInited : textureDisabledIsInited));
  string &filename = (over ? imageFilenameOver : (enabled ? imageFilename : imageFilenameDisabled));
  Texture &text = (over ? textureOver : (enabled ? texture : textureDisabled));

  if (filename.empty())
    return false;

  if (isInited) // already loaded?
    return true;

  isInited = true;

  // read the PNG file using pngLoad
  // raw data from PNG file is in image buffer
  if (text.load (filename) == false)
  {
    cerr << "(pngLoad) " << filename << " FAILED" << endl;;
    return false;
  }

  return true;
}

} // namespace mhgui
