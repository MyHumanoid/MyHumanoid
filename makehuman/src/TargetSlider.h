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
 *  File   : TargetSlider.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef TARGETSLIDER_H
#define TARGETSLIDER_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <iostream>
#include <gui/ImageSlider.h>

using std::string;
using std::cerr;
using std::endl;

using namespace mhgui;

class TargetSlider : public ImageSlider
{
private:
  string targetName;

public:
  TargetSlider (uint32_t      inImageID,
               const string& inSliderImageFilename,
	             const string& inTargetName,
               const Rect&   inGeometry,
               float         inMinValue,
               float         inMaxValue) :
  ImageSlider (inImageID, inSliderImageFilename,
               inGeometry, inMinValue, inMaxValue),
  targetName (inTargetName)
  {}

  virtual ~TargetSlider () {}

  void setTargetName (const string& name) {targetName = name;}
  string getTargetName () {return targetName;}
};

#endif // TARGETSLIDER_H
