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
 *  File   : Selector.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <iostream>

#include <glm/glm.hpp>

#include "gui/Selector.h"
#include "gui/CGUtilities.h"
#include "gui/GLUTWrapper.h"
#include "gui/SelectorSysListener.h"

using std::cerr;
using std::endl;

namespace mhgui
{

// constructor
Selector::Selector(uint32_t inId, const std::string & inFilename, const Rect & inGeometry)
        : Widget(inId, inGeometry)
        ,

        texture()
        , textureDisabled()
        ,

        imageFilename(inFilename)
        , imageFilenameDisabled()
        ,

        textureIsInited(false)
        , textureDisabledIsInited(false)
        , textureCursorIsInited(false)
        ,

        selectorSysListener(new SelectorSysListener())
        , alpha(1.0)
        , enabled(true)
        , showLines(true)
        , cursorPos(0, 0)
        , rows(2)
        , cols(2)
        , points()
        , labels()
        , linesColor(0.0, 0.0, 0.0, 1.0)
        , backgroundColor(1.0, 1.0, 1.0, 1.0)
        , cursorColor(1.0, 0.0, 0.0, 1.0)
        , cursorTexture()
        , cursorFilename()
{

	setSysListener(selectorSysListener);

	if(inFilename.length() > 4) {
		imageFilenameDisabled = inFilename.substr(0, inFilename.length() - 4) + "_disa.png";
	}

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			glm::ivec2 tmp(j * getSize().getWidth() / (cols - 1),
			          i * getSize().getHeight() / (rows - 1));
			points.push_back(tmp);
		}
	}

	float cellWidth  = getSize().getWidth() / (cols - 1);
	float cellHeight = getSize().getHeight() / (rows - 1);
	cellRatio        = cellWidth / cellHeight;
	maxValue         = glm::min(cellWidth, cellHeight * cellRatio);
}

Selector::~Selector()
{
	delete selectorSysListener; // remove the listener again
}

void Selector::setPoints(int inRows, int inCols)
{
	rows = inRows;
	cols = inCols;

	points.clear();

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			glm::ivec2 tmp(j * getSize().getWidth() / (cols - 1),
			          i * getSize().getHeight() / (rows - 1));
			points.push_back(tmp);
		}
	}

	float cellWidth  = getSize().getWidth() / (cols - 1);
	float cellHeight = getSize().getHeight() / (rows - 1);

	cellRatio = cellWidth / cellHeight;
	maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
}

void Selector::setCursorPosFromMousePoint(const glm::ivec2 & inMousePoint)
{
	int x;
	int y;

	if(inMousePoint.x < getAbsolutePosition().x) {
		x = getAbsolutePosition().x;
	} else if(inMousePoint.x > getAbsolutePosition().x + getSize().getWidth()) {
		x = getAbsolutePosition().x + getSize().getWidth();
	} else {
		x = inMousePoint.x;
	}

	if(inMousePoint.y < getAbsolutePosition().y) {
		y = getAbsolutePosition().y;
	} else if(inMousePoint.y > getAbsolutePosition().y + getSize().getHeight()) {
		y = getAbsolutePosition().y + getSize().getHeight();
	} else {
		y = inMousePoint.y;
	}

	glm::ivec2 tmp(x, getAbsolutePosition().y + getSize().getHeight() - y);
	setCursorPos(tmp);
}

void Selector::setDisabledTexture(const std::string & inFilename)
{
	imageFilenameDisabled = inFilename;
}

void Selector::setCursorTexture(const std::string & inFilename)
{
	cursorFilename = inFilename;
}

void Selector::setAlpha(float a)
{
	alpha = a;
}

void Selector::show()
{
	setVisible(true);

	lazyLoadTexture();
}

void Selector::hide()
{
	setVisible(false);
}

// Return the ID assigned
const Texture & Selector::getTextures()
{
	lazyLoadTexture();
	return enabled ? texture : textureDisabled;
}

const Texture & Selector::getCursorTextures()
{
	lazyLoadCursorTexture();
	return cursorTexture;
}

std::vector<float> Selector::getDists()
{
	std::vector<float> ret;

	std::vector<glm::ivec2>::iterator vp_end = points.end();
	for(std::vector<glm::ivec2>::iterator vp_it = points.begin(); vp_it != vp_end; vp_it++) {
		glm::ivec2 & tmp(*vp_it);

		float dist  = sqrt(pow(tmp.x - cursorPos.x, 2) +
                                  pow((tmp.y - cursorPos.y) * cellRatio, 2));
		float value = 1 - (dist / maxValue);
		if(value > 0) {
			ret.push_back(value);
		} else {
			ret.push_back(0.0f);
		}
	}

	return ret;
}

void Selector::drawOverlay()
{
	draw();
}

// draw function
void Selector::draw()
{
	if(isVisible()) {
		cgutils::enableBlend();
		if(lazyLoadTexture()) {
			cgutils::drawSquareFillTexture(getAbsoluteRect(), alpha, getTextures());
		} else {
			cgutils::drawSquareFill(getAbsoluteRect(), backgroundColor);
		}

		// cursor
		const glm::ivec2 & pos  = getAbsolutePosition();
		const Size &  size = getSize();
		const Rect    cur(cursorPos.x + pos.x - HALF_CURSOR_SIZE,
                               (size.getHeight() + pos.y) - cursorPos.y -
                                       HALF_CURSOR_SIZE,
                               CURSOR_SIZE, CURSOR_SIZE);
		if(lazyLoadCursorTexture()) {
			cgutils::drawSquareFillTexture(cur, alpha, getCursorTextures());
		} else {
			cgutils::drawSquareFill(cur, cursorColor);
		}

		if(showLines) {
			// lines
			for(int i = 1; i < rows - 1; i++) {
				int y = (i * size.getHeight() / (rows - 1));
				cgutils::drawLine2D(
				        glm::ivec2(pos.x, pos.y + y),
				        glm::ivec2(pos.x + size.getWidth(), pos.y + y),
				        linesColor);
			}

			for(int i = 1; i < cols - 1; i++) {
				int x = (i * size.getWidth() / (cols - 1));
				cgutils::drawLine2D(
				        glm::ivec2(pos.x + x, pos.y),
				        glm::ivec2(pos.x + x, pos.y + size.getHeight()),
				        linesColor);
			}
		}

		glDisable(GL_BLEND);
	}
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
bool Selector::lazyLoadTexture()
{
	bool &    isInited = enabled ? textureIsInited : textureDisabledIsInited;
	string &  filename = enabled ? imageFilename : imageFilenameDisabled;
	Texture & text     = enabled ? texture : textureDisabled;

	if(filename.empty())
		return false;

	if(isInited) // already loaded?
		return true;

	isInited = true;

	// read the PNG file using pngLoad
	// raw data from PNG file is in image buffer
	if(text.load(filename) == false) {
		cerr << "(pngLoad) " << filename << " FAILED" << endl;
		;
		return false;
	}

	return true;
}

bool Selector::lazyLoadCursorTexture()
{
	bool &    isInited = textureCursorIsInited;
	string &  filename = cursorFilename;
	Texture & text     = cursorTexture;

	if(filename.empty())
		return false;

	if(isInited) // already loaded?
		return true;

	isInited = true;

	// read the PNG file using pngLoad
	// raw data from PNG file is in image buffer
	if(text.load(filename) == false) {
		cerr << "(pngLoad) " << filename << " FAILED" << endl;
		;
		return false;
	}

	return true;
}

} // namespace mhgui
