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
 *  File   : Global.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "animorph/BodySettings.h"
#include <animorph/Mesh.h>
#include <gui/Camera.h>

using Animorph::SavedCompSetting;

extern Animorph::Mesh g_mesh;

struct GlobalPaths {
};

struct Global {
	GlobalPaths paths;

	mhgui::Camera *   camera          = NULL;
	mhgui::Autozoom * autozoom        = NULL;
	bool       drawGrid        = false;
	bool       quotedBox       = false;
	bool       m_enableTexture = false;
	bool       subdivision     = false;
	
	
	SavedCompSetting m_comp;
	
	void clearFuzzy() {
		m_comp = SavedCompSetting();
	}
};

extern Global g_global;

struct WindowVisibility {
	bool glInfo = false;
	bool performance         = false;
	bool show_demo_window    = false;
	bool about               = false;
};

extern WindowVisibility g_displayWin;

extern bool g_morphMode;


static constexpr char mh_app_name[] = "MyHumanoid";
static constexpr char mh_version[]  = "0.1.0";

