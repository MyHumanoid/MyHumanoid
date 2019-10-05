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
 *  File   : Window.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Component.h"
#include <animorph/Color.h>
#include <animorph/Matrix.h>
#include <glm/vec3.hpp>
#include <list>
#include <string>
//#include "Panel.h"
#include "Camera.h"
#include "ImageData.h"
#include "Texture.h"

using std::list;
using std::string;


class CharacterSettingPanel;

namespace mhgui
{

class Panel;
class Size;

/// Object oriented interface to a GLUT window
class Window : public Component
{
	string    title;
	bool      fullscreen;
	float     light0Lum;
	float     light1Lum;
	glm::vec3 light0Pos;
	glm::vec3 light1Pos;

	list<Panel *> panelList; ///< Panels contain widgets, we contain the panels
	int           panelListChangedCount;

	Camera * inCamera;
	int      mWindowId; ///< The GLUT Window ID;

public:
	Window(const Rect & rect, const std::string & t);
	virtual ~Window();

	Window & operator=(Window &) = delete;
	Window(const Window &)       = delete;

	void drawPanels();

	void setTitle(const string & t);

	// Some "shortcut functions" that wrap a series of OpenGL lines
	void initWindow();
	// void toggleFullscreen (); --> this needs to be implemented!

	/// For a GLUT callback
	void reshape(const Size & inSize, const Camera & inCamera);

	void setCamera(Camera * p_camera);

	// main functions
	bool    addPanel(Panel * p /*, bool now = true*/);
	Panel * getPanel(uint32_t inPanelId);

	bool isMouseOverPanel(const glm::ivec2 & inMousePos);
	bool isMouseClickPanel(const glm::ivec2 & inMousePos, int button, int state);
	bool isMouseDraggedPanel(const glm::ivec2 & inMousePos);
	bool isKeyTypePanel(unsigned char key);

	void defaultDisplay();

	// texture functions
	bool loadPNG(const string & filename);

	virtual void draw();
	virtual void show();
	virtual void hide();
	
	
	CharacterSettingPanel * characterSettingPanel;
};

extern Window * g_mainWindow;

} // namespace mhgui
