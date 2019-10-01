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
 *  File   : Window.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <GL/freeglut.h>

#include "MhGui.h"
#include <examples/imgui_impl_glut.h>
#include <examples/imgui_impl_opengl2.h>

#include "gui/CGUtilities.h"
#include "gui/Camera.h"
#include "gui/Size.h"
#include "gui/Window.h"

#include "gui/Panel.h"
#include <iostream>

using namespace std;
using namespace Animorph;

namespace mhgui
{

Window * g_mainWindow;

// Constructor
Window::Window(const Rect & rect, const string & t)
        : Component(FOUR_CHAR_CONST('M', 'W', 'i', 'n'), rect)
        , title(t)                    //!< The window title bar
        , fullscreen(false)           //!< full screen no
        , light0Lum(+1.0)             //!< light 0 luminosity
        , light1Lum(+1.0)             //!< light 1 luminosity
        , light0Pos(+1.0, +1.0, +1.0) //!< light 0 position
        , light1Pos(-1.0, +1.0, +1.0) //!< light 1 position
        , panelList()
        , panelListChangedCount(0)
        , inCamera(NULL)
        , mWindowId(0)
{
}

Window::~Window()
{
	if(mWindowId > 0)
		glutDestroyWindow(mWindowId);
}

Panel * Window::getPanel(uint32_t inPanelId)
{
	for(list<Panel *>::const_iterator pl_it = panelList.begin(); pl_it != panelList.end();
	    pl_it++) {
		Panel * p_cmp = (*pl_it);

		if(inPanelId == p_cmp->getID()) {
			return p_cmp;
		}
	}

	return NULL;
}

void Window::setCamera(Camera * p_camera) { inCamera = p_camera; }

void Window::draw() { drawPanels(); }

void Window::show() { setVisible(true); }

void Window::hide() {}

// Add panel into window
bool Window::addPanel(Panel * p)
{
	if(p == NULL)
		return false;

	Panel * panel = dynamic_cast<Panel *>(p);
	// Just Panels are allowed!
	assert(panel);

	for(list<Panel *>::iterator pl_it = panelList.begin(); pl_it != panelList.end(); pl_it++) {
		Panel * p_cmp = (*pl_it);

		if(*p_cmp == *p) {
			cerr << "Error: Panel needs a unique name! "
			     << "A Panel with name '" << p->getIDAsString() << "' does yet exist!"
			     << endl;
			return false;
		}
	}

	panelList.push_back(panel);
	++panelListChangedCount; // mark a change of the Panel List

	panel->setParentWindow(this);

	// do a reshape event for update
	reshape(getRect().getSize(), *inCamera);

	return true;
}

void Window::removePanel(Panel * p)
{
	if(p == NULL)
		return;

	for(list<Panel *>::iterator pl_it = panelList.begin(); pl_it != panelList.end(); pl_it++) {
		Panel * p_cmp = (*pl_it);

		if(*p_cmp == *p) {
			panelList.erase(pl_it);
			++panelListChangedCount; // mark a change of the Panel List
			return;
		}
	}
}

// For each panel, draw all widgets
void Window::drawPanels()
{
	if(isVisible()) {
		cgutils::enableOrthographicProjection();

		for(list<Panel *>::const_iterator pl_it = panelList.begin();
		    pl_it != panelList.end(); pl_it++) {
			Panel * panel = dynamic_cast<Panel *>(*pl_it);
			assert(panel); // assert if this is not a Panel!

			// printf("Drawing Panel '%s'\n", panel->getIDAsString().c_str());
			panel->draw();
		}
		cgutils::disableOrthographicProjection();
	}
};

void Window::setTitle(const std::string & t)
{
	// hm, this should change the GLUT window title, not?
	title = t;
}

// For each panel, check if mouse is over his widgets
bool Window::isMouseOverPanel(const Point & inMousePos)
{
	bool isOver = false;

	int rememberedPanelListChangedCount = panelListChangedCount;

	for(list<Panel *>::reverse_iterator pl_it = panelList.rbegin(); pl_it != panelList.rend();
	    pl_it++) {
		Panel * panel = (*pl_it);

		isOver = panel->isMouseOverWidgets(inMousePos);
		if(isOver == true) {
			break;
		}

		isOver = panel->isMouseOver(inMousePos);
		if(isOver == true) {
			break;
		}

		/* Check if the panel List has been changed in between
		 * (triggered by a Panel- or a Widget Listener for example)
		 * If it has then "reinitialize" the iterator because it is
		 * possible that it became invalide because of this change
		 * of the list! */
		if(panelListChangedCount != rememberedPanelListChangedCount) {
			rememberedPanelListChangedCount = panelListChangedCount;
			pl_it = panelList.rbegin(); // reinitailize the iterator
		}
	}
	return isOver;
}

// For each panel, check if click is over his widgets
bool Window::isMouseClickPanel(const Point & inMousePos, int button, int state)
{
	bool isClick = false;

	int rememberedPanelListChangedCount = panelListChangedCount;

	for(list<Panel *>::reverse_iterator pl_it = panelList.rbegin(); pl_it != panelList.rend();
	    pl_it++) {
		Panel * panel = (*pl_it);

		isClick = panel->isMouseClickWidgets(inMousePos, button, state);
		if(isClick == true) {
			break;
		}

		isClick = panel->isMouseClick(inMousePos, button, state);
		if(isClick == true) {
			break;
		}

		/* Check if the panel List has been changed in between
		 * (triggered by a Panel- or a Widget Listener for example)
		 * If it has then "reinitialize" the iterator because it is
		 * possible that it became invalide because of this change
		 * of the list! */
		if(panelListChangedCount != rememberedPanelListChangedCount) {
			rememberedPanelListChangedCount = panelListChangedCount;
			pl_it = panelList.rbegin(); // reinitailize the iterator
		}
	}

	return isClick;
}

// For each panel, check if a widget is active
bool Window::isKeyTypePanel(unsigned char key)
{
	bool keyType = false;

	int rememberedPanelListChangedCount = panelListChangedCount;

	for(list<Panel *>::reverse_iterator pl_it = panelList.rbegin(); pl_it != panelList.rend();
	    pl_it++) {
		Panel * panel = (*pl_it);

		keyType = panel->isKeyTypeWidgets(key);
		if(keyType == true) {
			break;
		}

		keyType = panel->isKeyType(key);
		if(keyType == true) {
			break;
		}

		/* Check if the panel List has been changed in between
		 * (triggered by a Panel- or a Widget Listener for example)
		 * If it has then "reinitialize" the iterator because it is
		 * possible that it became invalide because of this change
		 * of the list! */
		if(panelListChangedCount != rememberedPanelListChangedCount) {
			rememberedPanelListChangedCount = panelListChangedCount;
			pl_it = panelList.rbegin(); // reinitailize the iterator
		}
	}
	return keyType;
}

bool Window::isMouseDraggedPanel(const Point & inMousePos)
{
	bool dragged = false;

	int rememberedPanelListChangedCount = panelListChangedCount;

	for(list<Panel *>::reverse_iterator pl_it = panelList.rbegin(); pl_it != panelList.rend();
	    pl_it++) {
		Panel * panel = (*pl_it);

		dragged = panel->isMouseDraggedWidgets(inMousePos);
		if(dragged == true) {
			break;
		}

		dragged = panel->isMouseDragged(inMousePos);
		if(dragged == true) {
			break;
		}

		/* Check if the panel List has been changed in between
		 * (triggered by a Panel- or a Widget Listener for example)
		 * If it has then "reinitialize" the iterator because it is
		 * possible that it became invalide because of this change
		 * of the list! */
		if(panelListChangedCount != rememberedPanelListChangedCount) {
			rememberedPanelListChangedCount = panelListChangedCount;
			pl_it = panelList.rbegin(); // reinitailize the iterator
		}
	}
	return dragged;
}

// Init window with some classic openGL commands
void Window::initWindow()
{
	mWindowId = cgutils::initWindow(getRect(), title.c_str(), light0Pos, light1Pos, light0Lum,
	                                light1Lum);
}

// Glut call back functions
void Window::reshape(const Size & inSize, const Camera & inCamera)
{
	cgutils::reshape(inSize, inCamera);

	for(list<Panel *>::const_iterator pl_it = panelList.begin(); pl_it != panelList.end();
	    pl_it++) {
		Panel * panel = (*pl_it);

		if(panel->getMaximize()) {
			Rect r(0, 0, 0, 0);
			r.resizeTo(inSize);
			panel->setRect(r);
		} else {
			if(panel->getBottomAnchor()) {
				int bottom_old =
				        getSize().getHeight() - panel->getPosition().getY();
				int bottom = inSize.getHeight() - bottom_old;

				panel->setPosition(Point(panel->getPosition().getX(), bottom));
			}
			if(panel->getRightAnchor()) {
				int right_old = getSize().getWidth() - panel->getPosition().getX();
				int right     = inSize.getWidth() - right_old;
				panel->setPosition(Point(right, panel->getPosition().getY()));
			}
		}

		panel->calcWidgetPosition();
	}

	setSize(inSize);
}

} // namespace mhgui
