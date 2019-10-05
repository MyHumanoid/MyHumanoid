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

#include "GlInclude.h"
#include <GL/freeglut.h>

#include "MhUi.h"
#include <examples/imgui_impl_glut.h>
#include <examples/imgui_impl_opengl2.h>

#include "gui/CGUtilities.h"
#include "gui/Camera.h"
#include "gui/Window.h"

#include "gui/Panel.h"
#include <iostream>

#include "ComponentID.h"


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

void Window::setCamera(Camera * p_camera)
{
	inCamera = p_camera;
}

void Window::draw()
{
	drawPanels();
}

// Add panel into window
bool Window::addPanel(Panel * p)
{
	if(p == NULL)
		return false;

	Panel * panel = dynamic_cast<Panel *>(p);
	// Just Panels are allowed!
	assert(panel);

	panelList.push_back(panel);
	++panelListChangedCount; // mark a change of the Panel List

	// do a reshape event for update
	reshape(getRect().size, *inCamera);

	return true;
}

// For each panel, draw all widgets
void Window::drawPanels()
{
	cgutils::enableOrthographicProjection();

	for(list<Panel *>::const_iterator pl_it = panelList.begin();
		pl_it != panelList.end(); pl_it++) {
		Panel * panel = dynamic_cast<Panel *>(*pl_it);
		assert(panel); // assert if this is not a Panel!

		// printf("Drawing Panel '%s'\n", panel->getIDAsString().c_str());
		panel->draw();
	}
	cgutils::disableOrthographicProjection();
};

// For each panel, check if click is over his widgets
bool Window::isMouseClickPanel(const glm::ivec2 & inMousePos, int button, int state)
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

bool Window::isMouseDraggedPanel(const glm::ivec2 & inMousePos)
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
void Window::reshape(const glm::ivec2 & inSize, const Camera & inCamera)
{
	cgutils::reshape(inSize, inCamera);

	for(list<Panel *>::const_iterator pl_it = panelList.begin(); pl_it != panelList.end();
	    pl_it++) {
		Panel * panel = (*pl_it);

		panel->calcWidgetPosition();
	}

	setSize(inSize);
}

} // namespace mhgui
