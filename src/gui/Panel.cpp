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
 *  File   : Panel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Panel.h"
#include "gui/CGUtilities.h"
#include "gui/Rect.h"
#include "gui/Widget.h"

#include <glm/glm.hpp>
#include <iostream>

#include "ComponentID.h"

using namespace std;

namespace mhgui
{

Panel::Panel(uint32_t inPanelId, const Rect & inGeometry)
        : Component(inPanelId, inGeometry)
        ,

        widgetList()
        , widgetListChangedCount(0)
        ,

        rightAnchor(false)
        , bottomAnchor(false)
        , parentWindow(NULL)
        , align(HORIZONTAL)
        , maximize(false)
        , rowSpacing(0)
        , columnSpacing(0)
        , backColor(0, 0, 0, 0)
{
}

Panel::~Panel()
{
	// Remove this Panel reference from the parent window.
	if(parentWindow)
		parentWindow->removePanel(this);
}

const Window * Panel::getParentWindow() const
{
	return parentWindow;
}

void Panel::setParentWindow(Window * w)
{
	parentWindow = w;
}

// Put widget into panel
bool Panel::addWidget(Widget * w)
{
	// th ID FOUR_CHAR_CONST ('D', 'M', 'M', 'Y') (means dummy) is the only ID
	// which can occur several times which means "not specified".
	if(w->getID() != FOUR_CHAR_CONST('D', 'M', 'M', 'Y')) {
		// if the given ID is not a dummy then check if a Widget with this id
		// already exists!
		for(list<Widget *>::const_iterator wl_it = widgetList.begin();
		    wl_it != widgetList.end(); wl_it++) {
			if((*w == **wl_it)) {
				cerr << "**** Error: Widget needs a unique name in the same Panel! "
				     << "A Widget with name '" << w->getIDAsString()
				     << "' does yet exist!" << endl;
				return false;
			}
		}
	}

	widgetList.push_back(w);
	++widgetListChangedCount; // mark a change of the Widget List

	w->parentPanel = this;

	// set widgets zero point to the panels position
	w->setZeroPoint(getPosition());

	calcWidgetPosition();

	return true;
}

void Panel::removeWidget(Widget * w)
{
	for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
	    wl_it++) {
		if(w->operator==(**wl_it)) {
			// if removing a widget set zero point to 0,0
			w->setZeroPoint(glm::ivec2(0, 0));
			widgetList.erase(wl_it);
			++widgetListChangedCount; // mark a change of the Widget List
			break;
		}
	}

	calcWidgetPosition();
}

Widget * Panel::getWidget(uint32_t inWidgetId)
{
	for(list<Widget *>::const_iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
	    wl_it++) {
		Widget * widget = (*wl_it);

		if(inWidgetId == widget->getID()) {
			return widget;
		}
	}

	return NULL;
}

void Panel::show()
{
	setVisible(true);
}

void Panel::hide()
{
	setVisible(false);
}

void Panel::calcWidgetPosition()
{
	if(align == FREE) {
		// do not much because alignment is FREE
		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * widget = (*wl_it);
			widget->setZeroPoint(getPosition());
		}
	} else if(align == HORIZONTAL) {
		int xpos     = 0;
		int ypos     = 0;
		int ybiggest = 0;

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * widget = (*wl_it);
			widget->setZeroPoint(getPosition());

			// some references to ease access
			const int widgetWidth  = widget->getSize().getWidth() + columnSpacing;
			const int widgetHeight = widget->getSize().getHeight() + rowSpacing;
			const int panelWidth   = getSize().getWidth();
			const int panelHeight  = getSize().getHeight();

			// empty space in current x row
			if((xpos + widgetWidth <= panelWidth) &&
			   (ypos + widgetHeight <= panelHeight)) {
				widget->setPosition(glm::ivec2(xpos, ypos));

				// after position a widget increment current x position
				xpos += widgetWidth;

				// find biggest y position in a row
				ybiggest = glm::max<int>(ybiggest, ypos + widgetHeight);
			}
			// x row full, but space in new y row
			else if((xpos + widgetWidth > panelWidth) &&
			        (ybiggest + widgetHeight <= panelHeight)) {
				// new row -> reset x position
				xpos = 0;

				widget->setPosition(glm::ivec2(xpos, ybiggest));

				// after position a widget increment current x position
				xpos += widgetWidth;

				// the new y position for this row is the biggest y from the last
				// row
				ypos = ybiggest;
			}
			// no more space available in Panel
			else {
				// set position to 0,0 and hide the widget
				widget->setPosition(glm::ivec2(0, 0));
				widget->hide();
				break;
			}
		}
	} else if(align == VERTICAL) {
		int xpos     = 0;
		int ypos     = 0;
		int xbiggest = 0;

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * widget = (*wl_it);
			widget->setZeroPoint(getPosition());

			// some references to ease access
			const int widgetWidth  = widget->getSize().getWidth() + columnSpacing;
			const int widgetHeight = widget->getSize().getHeight() + rowSpacing;
			const int panelWidth   = getSize().getWidth();
			const int panelHeight  = getSize().getHeight();

			// empty space in current y row
			if((ypos + widgetHeight <= panelHeight) &&
			   (xpos + widgetWidth <= panelWidth)) {
				widget->setPosition(glm::ivec2(xpos, ypos));

				// after position a widget increment current y position
				ypos += widgetHeight;

				// find biggest x position in a row
				xbiggest = glm::max<int>(xbiggest, xpos + widgetWidth);
			}
			// y row full, but space in new x row
			else if((ypos + widgetHeight > panelHeight) &&
			        (xpos + widgetWidth <= panelWidth)) {
				// new row -> reset y position
				ypos = 0;

				widget->setPosition(glm::ivec2(xbiggest, ypos));

				// after position a widget increment current y position
				ypos += widgetHeight;

				// the new y position for this row is the biggest y from the last
				// row
				xpos = xbiggest;
			}
			// no more space available in Panel
			else {
				// set position to 0,0 and hide the widget
				widget->setPosition(glm::ivec2(0, 0));
				widget->hide();
				break;
			}
		}
	}
}

// Draw all widgets
void Panel::draw()
{
	if(isVisible()) {
		cgutils::enableBlend();
		cgutils::drawSquareFill(getRect(), backColor);

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * w = (*wl_it);

			w->draw_wrapper();
		}

		// draw overlay effects
		// This is great, because widgets couldn't overlap effects. Perhaps there's
		// a problem if a effect draws over another panel and this panel is drawn
		// before the effect. If this case is a problem in future the effect drawing
		// should be moved to the window class...
		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * w = (*wl_it);

			w->drawOverlay();
		}

		glDisable(GL_BLEND);
	}
}

bool Panel::isMouseDraggedWidgets(const glm::ivec2 & inMousePos)
{
	if(isVisible()) {
		bool dragged = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * w = (*wl_it);

			dragged = w->isMouseDragged(inMousePos);
			if(dragged == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if(widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}

		} // for(;;)
		return dragged;
	} else
		return false;
}

// Check if mouse is over a widget
bool Panel::isMouseOverWidgets(const glm::ivec2 & inMousePos)
{
	if(isVisible()) {
		bool isOver = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++) {
			Widget * w = (*wl_it);

			isOver = w->isMouseOver(inMousePos);
			if(isOver == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if(widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}
		}
		return isOver;
	} else
		return false;
}

// Check if click is over a widget
bool Panel::isMouseClickWidgets(const glm::ivec2 & inMousePos, int button, int state)
{
	// cerr << "isMouseClickWidgets start" << endl;
	if(isVisible()) {
		bool isClick = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for(list<Widget *>::iterator wl_it = widgetList.begin(); wl_it != widgetList.end();
		    wl_it++)
		/*for (unsigned int i = 0; i < widgetList.size (); i++)*/
		{
			Widget * w = (*wl_it);

			isClick = w->isMouseClick(inMousePos, button, state);

			if(isClick == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if(widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}
		}

		return isClick;
	} else {
		return false;
	}
}

// Check keyboard events
bool Panel::isKeyTypeWidgets(unsigned char key)
{
	if(isVisible()) {
		bool keyType = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		list<Widget *>::const_iterator it = widgetList.begin();
		while(it != widgetList.end()) {
			keyType = (*it++)->isKeyType(key);
			if(keyType == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if(widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				it = widgetList.begin(); // reinitailize the iterator
			}
		}
		return keyType;
	} else
		return false;
}

void Panel::show_all()
{
	// show the panel itself
	show();

	list<Widget *>::const_iterator it = widgetList.begin();
	while(it != widgetList.end()) {
		Widget * w = *it++;

		assert(w);
		// show all widgets in the current page of the panel
		w->show();
	}
}

} // namespace mhgui
