// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright (C) 2005-2007  MakeHuman Project

#pragma once

#include <functional>

#include <SDL3/SDL.h>

#include "animorph/Mesh.h"
#include "gui/Camera.h"

extern Animorph::Mesh g_mesh;

struct Global {
	std::function<void()> toggleFullscreen;
};

extern Global g_global;

extern bool g_morphMode;

struct AppState {
	
	SDL_Window*   mainWindow;
	SDL_GLContext mainContext;
	mhgui::Camera *   camera          = NULL;
	mhgui::Autozoom * autozoom        = NULL;
	
	struct Args {
		bool debug = false;
		bool editor = true;
		bool background = true;
	} args;
	
	struct Ui {
		struct Request {
			bool quit = false;
			bool quitAccept = false;
		} request;
		
	} ui;
	
	struct WindowVisibility {
		bool glInfo = false;
		bool performance         = false;
		bool show_demo_window    = false;
		bool about               = false;
		bool settings = false;
	} uiState;
	
	struct Settings {
		bool askOnClose = true;
		bool developerMode = false;
	} settings;
	
};


static constexpr char mh_app_name[] = "MyHumanoid";

