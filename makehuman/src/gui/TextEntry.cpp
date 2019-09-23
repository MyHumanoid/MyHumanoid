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
 *  File   : TextEntry.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/TextEntry.h"
#include "gui/CGUtilities.h"
#include "gui/GLUTWrapper.h"
#include "gui/Tooltip.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace mhgui
{
// constructor
TextEntry::TextEntry(uint32_t inId, const Rect &inGeometry)
    : Widget(inId, inGeometry)
    , overlay(0, 0, 0, 0)
    , backColor(1, 1, 1, 1)
    , textColor(0, 0, 0, 1)
    , borderColor(0.5, 0.5, 0.5, 1.0)
    , textEntrySysListener(new TextEntrySysListener())
    , overlayEffect(false)
{
	setSysListener(textEntrySysListener);
}

TextEntry::~TextEntry() { delete textEntrySysListener; }

void TextEntry::show() { setVisible(true); }

void TextEntry::hide() { setVisible(false); }

void TextEntry::setOverlayRectangle(const Color &c)
{
	overlay = c;
	overlayEffect = true;
}

void TextEntry::setOverlayRectangle(bool overlayEffect)
{
	this->overlayEffect = overlayEffect;
}

void TextEntry::drawOverlay()
{
	if (isVisible()) {
		if (overlayEffect) {
			cgutils::enableBlend();
			cgutils::drawSquareFill(getAbsoluteRect(), overlay);
			glDisable(GL_BLEND);
		}
	}
}

// draw function
void TextEntry::draw()
{
	if (isVisible()) {

		cgutils::enableBlend();

		cgutils::enableScissor(getAbsoluteRect());

		cgutils::drawSquareFill(getAbsoluteRect(), backColor);

		// draw simple text border
		cgutils::drawSquare(getAbsoluteRect(), borderColor);

		int length = glutBitmapLength(GLUT_BITMAP_HELVETICA_12,
		                              (const unsigned char *)text.c_str());

		int length_cursor = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24,
		                                     (const unsigned char *)"I");

		int width = getSize().getWidth();
		int xpos;

		if (length + length_cursor > width) {
			xpos = width - (length + length_cursor);
		} else {
			xpos = 0;
		}

		// only a hack to position text
		Point pos = getAbsolutePosition();
		Point pos_rel(xpos, 15);

		pos.moveBy(pos_rel);

		cgutils::drawString(pos, GLUT_BITMAP_HELVETICA_12, text, textColor);

		Point pos_cursor(length, 4);
		pos.moveBy(pos_cursor);

		cgutils::drawString(pos, GLUT_BITMAP_TIMES_ROMAN_24, "I",
		                    Color(0.5, 0.5, 0.5));

		cgutils::disableScissor();

		glDisable(GL_BLEND);
	}
}

void TextEntry::addChar(const char c) { text += c; }

void TextEntry::removeChar()
{
	if (text.size())
		text.erase(text.size() - 1);
}
} // namespace mhgui
