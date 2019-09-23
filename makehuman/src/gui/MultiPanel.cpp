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
 *  File   : MultiPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/MultiPanel.h"
#include "gui/CGUtilities.h"
#include "gui/Panel.h"
#include "gui/Rect.h"
#include "gui/Widget.h"

namespace mhgui
{

MultiPanel::MultiPanel(uint32_t inMultiPanelId, const Rect &inGeometry)
    : Panel(inMultiPanelId, inGeometry)
    , panelList()
    , numPages(0)
    , currentPage(0)
{
}

MultiPanel::~MultiPanel() {}

// Put panel into multipanel
bool MultiPanel::addPanel(Panel *p)
{
	p->setVisible(true);
	p->show_all();
	panelList.push_back(p);
	++numPages;

	return true;
}

void MultiPanel::removePanel(Panel *p)
{
	for (PanelIterator pl_it = begin(); pl_it != panelList.end(); pl_it++) {
		if (p->operator==(**pl_it)) {
			panelList.erase(pl_it);
			--numPages;
			if (currentPage >= numPages) {
				currentPage = numPages - 1;
			}
			break;
		}
	}
}

// Draw visible panel
void MultiPanel::draw()
{
	if (isVisible()) {
		if (numPages > 0) {
			panelList[currentPage]->draw();
		}

		cgutils::enableBlend();

		for (list<Widget *>::iterator wl_it = widgetList.begin();
		     wl_it != widgetList.end(); wl_it++) {
			Widget *w = (*wl_it);

			w->draw_wrapper();
		}

		// draw overlay effects
		// This is great, because widgets couldn't overlap effects. Perhaps there's
		// a problem if a effect draws over another panel and this panel is drawn
		// before the effect. If this case is a problem in future the effect drawing
		// should be moved to the window class...
		for (list<Widget *>::iterator wl_it = widgetList.begin();
		     wl_it != widgetList.end(); wl_it++) {
			Widget *w = (*wl_it);

			w->drawOverlay();
		}

		glDisable(GL_BLEND);
	}
}

bool MultiPanel::hasNextPage() { return (currentPage < numPages - 1); }

bool MultiPanel::hasPrevPage() { return (currentPage > 0); }

void MultiPanel::checkControlsVisibility()
{
	if (prevPage != NULL) {
		if (hasPrevPage()) {
			prevPage->show();
		} else {
			prevPage->hide();
		}
	}

	if (nextPage != NULL) {
		if (hasNextPage()) {
			nextPage->show();
		} else {
			nextPage->hide();
		}
	}
}

void MultiPanel::pageNext()
{
	if (hasNextPage()) {
		panelList[currentPage++]->hide();
		panelList[currentPage]->show();
	}

	checkControlsVisibility();
}

void MultiPanel::pageBack()
{
	if (hasPrevPage()) {
		panelList[currentPage--]->hide();
		panelList[currentPage]->show();
	}

	checkControlsVisibility();
}

bool MultiPanel::isMouseOverWidgets(const Point &inMousePos)
{
	if (isVisible()) {
		bool isOver = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for (list<Widget *>::iterator wl_it = widgetList.begin();
		     wl_it != widgetList.end(); wl_it++) {
			Widget *w = (*wl_it);

			isOver = w->isMouseOver(inMousePos);
			if (isOver == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if (widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}
		}

		if (!isOver && numPages > 0) {
			isOver = panelList[currentPage]->isMouseOverWidgets(inMousePos);
		}
		return isOver;
	} else {
		return false;
	}
}

bool MultiPanel::isMouseClickWidgets(const Point &inMousePos, int button,
                                     int state)
{
	if (isVisible()) {
		bool isClick = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for (list<Widget *>::iterator wl_it = widgetList.begin();
		     wl_it != widgetList.end(); wl_it++)
		/*for (unsigned int i = 0; i < widgetList.size (); i++)*/
		{
			Widget *w = (*wl_it);

			isClick = w->isMouseClick(inMousePos, button, state);

			if (isClick == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if (widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}
		}

		if (!isClick && numPages > 0) {
			isClick = panelList[currentPage]->isMouseClickWidgets(inMousePos, button,
			                                                      state);
		}

		return isClick;
	} else {
		return false;
	}
}

bool MultiPanel::isMouseDraggedWidgets(const Point &inMousePos)
{
	if (isVisible()) {
		bool dragged = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		for (list<Widget *>::iterator wl_it = widgetList.begin();
		     wl_it != widgetList.end(); wl_it++) {
			Widget *w = (*wl_it);

			dragged = w->isMouseDragged(inMousePos);
			if (dragged == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if (widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				wl_it = widgetList.begin(); // reinitailize the iterator
			}

		} // for(;;)

		if (!dragged && numPages > 0) {
			dragged = panelList[currentPage]->isMouseDraggedWidgets(inMousePos);
		}
		return dragged;
	} else {
		return false;
	}
}

bool MultiPanel::isKeyTypeWidgets(unsigned char key)
{
	if (isVisible()) {
		bool keyType = false;

		int rememberedWidgetListChangedCount = widgetListChangedCount;

		list<Widget *>::const_iterator it = widgetList.begin();
		while (it != widgetList.end()) {
			keyType = (*it++)->isKeyType(key);
			if (keyType == true) {
				break;
			}

			/* Check if the widget List has been changed in between
			 * (triggered by a Widget Listener for example)
			 * If it has then "reinitialize" the iterator because it is
			 * possible that it became invalide because of this change
			 * of the list! */
			if (widgetListChangedCount != rememberedWidgetListChangedCount) {
				rememberedWidgetListChangedCount = widgetListChangedCount;
				it = widgetList.begin(); // reinitailize the iterator
			}
		}

		if (!keyType && numPages > 0) {
			keyType = panelList[currentPage]->isKeyTypeWidgets(key);
		}

		return keyType;
	} else {
		return false;
	}
}

void MultiPanel::calcWidgetPosition()
{
	Panel::calcWidgetPosition();

	for (PanelIterator pl_it = begin(); pl_it != panelList.end(); pl_it++) {
		Panel *panel = (*pl_it);

		panel->setPosition(getPosition());
		panel->calcWidgetPosition();
	}
}

} // namespace mhgui
