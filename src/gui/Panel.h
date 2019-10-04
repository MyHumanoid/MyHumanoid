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
 *  File: Panel.h
 *  MakeHuman Project <info@makehuman.org>
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Window.h"
#include <animorph/Color.h>
#include <cassert>
#include <list>

using Animorph::Color;
using std::list;
using std::string;

namespace mhgui
{

class Widget;

/// Component that contains other widgets in rows and columns
class Panel : public Component
{
public:
	Panel(uint32_t inPanelId, const Rect & inGeometry);
	virtual ~Panel();

	enum Alignment { HORIZONTAL, VERTICAL, FREE };

	virtual void show();
	virtual void hide();
	virtual void draw();

	bool     addWidget(Widget * w);
	void     removeWidget(Widget * w);
	Widget * getWidget(uint32_t inWidgetId);

	virtual bool isMouseOverWidgets(const glm::ivec2 & inMousePos);
	virtual bool isMouseClickWidgets(const glm::ivec2 & inMousePos, int button, int state);
	virtual bool isMouseDraggedWidgets(const glm::ivec2 & inMousePos);
	virtual bool isKeyTypeWidgets(unsigned char key);

	void show_all();

	void setBottomAnchor(bool anchor) { bottomAnchor = anchor; }
	bool getBottomAnchor() { return bottomAnchor; }

	void setRightAnchor(bool anchor) { rightAnchor = anchor; }
	bool getRightAnchor() { return rightAnchor; }

	void      setAlignment(Alignment align) { this->align = align; }
	Alignment getAlignment() { return align; }

	void          setBackgroundColor(const Color & c) { backColor = c; }
	const Color & getBackgroundColor() { return backColor; }

	// TODO: maximize couldn't been unset!
	void setMaximize(bool maximize) { this->maximize = maximize; }
	bool getMaximize() { return maximize; }

	void setRowSpacing(int space) { rowSpacing = space; }
	int  getRowSpacing() { return rowSpacing; }

	void setColumnSpacing(int space) { columnSpacing = space; }
	int  getColumnSpacing() { return columnSpacing; }

	virtual void calcWidgetPosition();

	// this is not really needed, as there's only one top-level-window,
	// but it's easier to access this way...
	const Window * getParentWindow() const;
	void           setParentWindow(Window * w);

protected:
	list<Widget *> widgetList;
	int            widgetListChangedCount;

	bool     rightAnchor;
	bool     bottomAnchor;
	Window * parentWindow;

	Alignment align;
	bool      maximize;
	int       rowSpacing;
	int       columnSpacing;

private: // intentionally not implemeted
	Panel(const Panel &);
	Panel & operator=(const Panel &);

	Color backColor;

public:
	typedef list<Widget *>::iterator WidgetIterator;

	WidgetIterator begin() { return widgetList.begin(); }
	WidgetIterator end() { return widgetList.end(); }
};

} // namespace mhgui
