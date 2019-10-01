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
 *  File   : ImageSlider.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef IMAGESLIDER_H
#define IMAGESLIDER_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Image.h"
#include <iostream>

namespace mhgui
{

class Rect;
class ImageSliderSysListener;

/// Image widget with a slider underneath the image
class ImageSlider : public Image
{
protected:
	float                    minValue;
	float                    maxValue;
	float                    sliderValue;
	float                    valueStep;
	int                      oldMouseX;
	int                      overlayMultiplier;
	ImageSliderSysListener * imageSliderSysListener;

public:
	/*!
	 * @param inGeometry the font takes currently 10 pixel. So choose height
	 *        10 pixel larger than you like your image.
	 */
	ImageSlider(uint32_t inImageID, const std::string & inSliderImageFilename,
	            const Rect & inGeometry, float inMinValue, float inMaxValue);

	~ImageSlider();

	float getSliderValue();
	void  setSliderValue(float newValue);
	void  draw();
	void  drawOverlay();
	void  decreaseValue(int n);
	void  increaseValue(int n);
	void  setOldMouseX(int mouseX);
	int   getOldMouseX();
	float getMinValue() { return minValue; }
	float getMaxValue() { return maxValue; }
	void  setMinValue(float value) { minValue = value; }
	void  setMaxValue(float value) { maxValue = value; }
	void  setOverlayMultiplier(int value) { overlayMultiplier = (value > 0 ? value : 1); }
	void  setStep(float inValueStep) { valueStep = inValueStep; }
};

} // namespace mhgui

#endif // IMAGESLIDER_H
