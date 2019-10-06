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

#include <iostream>

#include "ComponentID.h"


namespace mhgui
{

Window * g_mainWindow;

// Constructor
Window::Window(const Rect & rect, const string & t)
        : geometry(rect)
        , title(t)                    //!< The window title bar
        , fullscreen(false)           //!< full screen no
        , light0Lum(+1.0)             //!< light 0 luminosity
        , light1Lum(+1.0)             //!< light 1 luminosity
        , light0Pos(+1.0, +1.0, +1.0) //!< light 0 position
        , light1Pos(-1.0, +1.0, +1.0) //!< light 1 position
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
}

void Window::setSize(const glm::ivec2 & inSize)
{
	geometry.resizeTo(inSize);
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

	setSize(inSize);
}

} // namespace mhgui
