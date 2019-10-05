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


#include <animorph/Mesh.h>
#include <gui/Camera.h>
#include <gui/Texture.h>

using namespace Animorph;
using namespace mhgui;

extern Animorph::Mesh g_mesh;

struct GlobalPaths {
};

const glm::ivec2 InvalidPoint = glm::ivec2(-666, -666);

struct Global {
	GlobalPaths paths;

	Camera *   camera          = NULL;
	Autozoom * autozoom        = NULL;
	bool       drawGrid        = false;
	bool       quotedBox       = false;
	bool       m_enableTexture = false;
	bool       subdivision     = false;
	
	
	glm::ivec2 m_kAge = InvalidPoint;
	glm::ivec2 m_kMuscleSize = InvalidPoint;
	glm::ivec2 m_kBreast = InvalidPoint;
	glm::ivec2 m_kShape = InvalidPoint;
	
	void clearFuzzy() {
		m_kAge = InvalidPoint;
		m_kMuscleSize = InvalidPoint;
		m_kBreast = InvalidPoint;
		m_kShape = InvalidPoint;
	}
};

extern Global g_global;

struct WindowVisibility {
	bool performance         = false;
	bool show_demo_window    = false;
	bool about               = false;
};

extern WindowVisibility g_displayWin;

extern bool g_morphMode;
