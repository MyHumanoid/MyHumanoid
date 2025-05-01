// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright (C) 2005-2007  MakeHuman Project

#pragma once

#include <functional>

#include <SDL3/SDL.h>

#include "animorph/BodySettings.h"
#include "animorph/Mesh.h"
#include "gui/Camera.h"

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

	std::function<void()> toggleFullscreen;
	SavedCompSetting m_comp;
	
	void clearFuzzy() {
		m_comp = SavedCompSetting();
	}
};

extern Global g_global;

extern bool g_morphMode;

struct AppState {
	SDL_Window*   mainWindow;
	SDL_GLContext mainContext;
	
	struct WindowVisibility {
		bool glInfo = false;
		bool performance         = false;
		bool show_demo_window    = false;
		bool about               = false;
	} window;
	
};


static constexpr char mh_app_name[] = "MyHumanoid";

