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
 *  File   : Selector.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef SELECTOR_H
#define SELECTOR_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Point.h"
#include "Texture.h"
#include "Widget.h"
#include "animorph/Color.h"
#include <string>

#define CURSOR_SIZE 6
#define HALF_CURSOR_SIZE CURSOR_SIZE / 2

using Animorph::Color;

using std::string;

namespace mhgui
{

class Tooltip;
class SelectorSysListener;

/* ========================================================================== */
/**
 */
/* ========================================================================== */
class Selector : public Widget
{
	// Textures
public:
	Texture texture;
	Texture textureDisabled;

	string imageFilename;
	string imageFilenameDisabled;

	bool textureIsInited; // used by lazyLoadTexture
	bool textureDisabledIsInited;

	bool textureCursorIsInited;

	SelectorSysListener *selectorSysListener;
	float alpha;

	bool enabled;
	bool showLines;

	Point cursorPos;

	int rows;
	int cols;

	float maxValue;
	float cellRatio;

	std::vector<Point> points;  // from left-bottom point to right-up point
	std::vector<string> labels; // from left-bottom point to right-up point

	Color linesColor;
	Color backgroundColor;
	Color cursorColor;

	Texture cursorTexture;
	string cursorFilename;

	// intentionally not implemented
	Selector(const Selector &);
	Selector &operator=(const Selector &);

public:
	Selector(uint32_t inId, const string &inFilename, const Rect &inGeometry);
	virtual ~Selector();

	void setAlpha(float);
	float getAlpha() const { return alpha; }

	void setEnabled(bool inEnabled) { enabled = inEnabled; }
	bool isEnabled() { return enabled; }

	void setShowLines(bool inShowLines) { showLines = inShowLines; }

	void setCursorPos(const Point &inCursorPos) { cursorPos = inCursorPos; }
	const Point &getCursorPos() const { return cursorPos; }

	void setCursorPosFromMousePoint(const Point &inMousePoint);

	std::vector<float> getDists();
	void setDisabledTexture(const string &inFilename);

	const Texture &getTextures(); // return the textureID

	void setPoints(int inRows, int inCols);

	void setCursorColor(const Color &inColor) { cursorColor = inColor; }
	void setBackgroundColor(const Color &inColor) { backgroundColor = inColor; }
	void setLinesColor(const Color &inColor) { linesColor = inColor; }
	void setCursorTexture(const string &inFilename);
	void setLabels(const std::vector<string> &inLabels) { labels = inLabels; }
	std::vector<string> getLabels() { return labels; }

	const Texture &getCursorTextures(); // return the cursor textureID

	virtual void show();
	virtual void hide();
	virtual void draw();
	virtual void drawOverlay();

protected:
	bool lazyLoadTexture();
	bool lazyLoadCursorTexture();
};

} // namespace mhgui

#endif // SELECTOR_H
