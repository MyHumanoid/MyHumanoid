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
 *  File   : makehuman.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <stdexcept>

#include <fmt/format.h>

#include "GlInclude.h"

#include "MhUi.h"
#include "MhUiMorph.h"
#include "MhUiPose.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <animorph/Mesh.h>
#include <animorph/util.h>

#include <gui/CGUtilities.h>
#include <gui/Camera.h>

#include "Logger.h"


#include "render/DebugGl.h"
#include "render/RenderUtils.h"
#include "render/Shader.h"

#include "StringUtils.h"

#include <time.h>

#include "ComponentID.h"
#include "Global.h"
#include "util.h"

#include "CompositeMorph.h"
#include "MhConfig.h"
#include "MhUiMain.h"
#include "MhRender.h"
#include "Version.h"
#include "Vfs.h"

#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "Profiler.h"



#define kTimerRendering 1000


bool  init; // shows the init status
int   average           = 0;
int   n_display         = 0;
int   tickCount         = 0;
float kTimePerRaster(0.03f);
bool  oldCameraTimerTrigger = false;

const Color border_color(1.0, 0.55, 0.0, 0.8);
const Color grid_color(0.35, 0.50, 0.30, 0.50);
const Color edges_color(0.4, 0.3, 0.3, 0.5);

// ================================================================================================

static void reshape(AppState& app)
{
	int w;
	int h;
	SDL_GetWindowSizeInPixels(app.mainWindow, &w, &h);
	
	cgutils::reshape(glm::ivec2(w, h), *g_global.camera);
	g_global.camera->reshape(w, h);
}

static void timerTrigger(AppState& app)
{
	bool     tmp;
	tmp = g_global.camera->timerTrigger();

	if(!g_global.camera->isPerspective()) {
		reshape(app);
	}
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
	*appstate = new AppState;
	AppState& app = *static_cast<AppState*>(*appstate);
	
	if(argc == 2) {
		if(argv[1] && argv[1] == std::string("--debug")) {
			g_logLevel = LogLevel::debug;
		}
	}
	
	vfs::init(argv[0]);
	
	LoadConfig();
	
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		log_error("Unable to initialize SDL");
		return SDL_APP_FAILURE;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	
	std::string title = mh_app_name + std::string(" ") + prettyVersion();
	
	const auto & winRect = g_config.windowMain;
	
	app.mainWindow = SDL_CreateWindow(title.c_str(),
	                              //winRect.pos.x,
	                              //winRect.pos.y,
	                              winRect.siz.x,
	                              winRect.siz.y,
	                              SDL_WINDOW_OPENGL
	                                  | SDL_WINDOW_RESIZABLE);
	
	if (!app.mainWindow) {
		log_error("Unable to create window");
		return SDL_APP_FAILURE;
	}
	
	SDL_SetWindowMinimumSize(app.mainWindow, 800, 600);

	if(auto iconSurface = loadSurfaceFromFile("data/pixmaps/icon.png")) {
		SDL_SetWindowIcon(app.mainWindow, iconSurface->m_ptr);
		iconSurface->free();
	}

	g_global.toggleFullscreen = [&](){
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
		bool IsFullscreen = SDL_GetWindowFlags(app.mainWindow) & FullscreenFlag;
		SDL_SetWindowFullscreen(app.mainWindow, IsFullscreen ? 0 : FullscreenFlag);
	};

	//checkSDLError(__LINE__);
	
	/* Create our opengl context and attach it to our window */
	app.mainContext = SDL_GL_CreateContext(app.mainWindow);
	//checkSDLError(__LINE__);
	TracyGpuContext;
	
	log_info("GL   version: {}", (const char*)glGetString(GL_VERSION));
	log_info("GLSL version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	initDebugGl();
	
	printTracingStatus();
	
	// ===========
	
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0, 0, 0, 1);
	
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);
	
	
	g_global.camera   = new mhgui::Camera();
	g_global.autozoom = new mhgui::Autozoom();
	
	
	g_renderBody.init();
	g_renderBackground.init();
	
	bool mesh_loaded = g_mesh.loadMesh("base.vertices", "base.faces");
	bool material_loaded = g_mesh.loadMaterial("base.materials", "base.colors");
	g_mesh.loadTextureVector("base.uv");
	bool groups_loaded = g_mesh.loadGroups("base.parts");
	bool smooth_loaded = g_mesh.loadSmoothVertex("base.smooth");
	
	if(!mesh_loaded) {
		log_error("couldn't load mesh geometry");
		return SDL_APP_FAILURE;
	}
	if(!material_loaded) {
		log_error("couldn't load mesh material informations");
		return SDL_APP_FAILURE;
	}
	if(!groups_loaded) {
		log_error("couldn't load face groups");
		return SDL_APP_FAILURE;
	}
	if(!smooth_loaded) {
		log_error("couldn't load smooth info");
		return SDL_APP_FAILURE;
	}

	CreateTargetImageTextures();
	CreatePoseImageTextures();
	CreateCaractersIconTextures();

	g_renderBody.loadTexture();
	g_global.m_enableTexture = true;

	g_mesh.loadTargets(searchDataDir("targets"));
	g_mesh.loadTargets(searchDataDir("selectors"), 1, false);
	g_mesh.loadPoseTargets(searchDataDir("rotations"));
	g_mesh.loadCharacters(searchDataDir("bs_data"));

	init = false;
	
	// camera->rotate (-glm::pi<float>()/2, X_AXIS);
	g_global.camera->move(0, 0, -125.0f);
	
	::glPolygonOffset(1.0, 1.0);

	loadDefaultBodySettings();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io     = ImGui::GetIO();
	io.IniSavingRate = 60.f;
	io.IniFilename   = "MyHumanoid-config-gui.ini";
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	
	ImGui_ImplSDL3_InitForOpenGL(app.mainWindow, app.mainContext);
	{
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	
	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can
	// also load multiple fonts and use ImGui::PushFont()/PopFont() to select
	// them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you
	// need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please
	// handle those errors in your application (e.g. use an assertion, or display
	// an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored
	// into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
	// ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string
	// literal you need to write a double backslash \\ !
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	// ImFont* font =
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
	// NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	AppState& app = *static_cast<AppState*>(appstate);
	
	FrameMark;
	ZoneScoped;
	
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	timerTrigger(app);
	
	g_global.camera->applyMatrix();
	
	g_renderBody.render();
	
	g_renderBackground.render(app);
	
	DisplayMainMenu(app);
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	ExecuteDeferredActions();
	
	SDL_GL_SwapWindow(app.mainWindow);
	TracyGpuCollect;
	
	if(g_userAcceptedQuit) {
		return SDL_APP_SUCCESS;
	} else {
		return SDL_APP_CONTINUE;
	}
}

bool mouseDownLeft = false;
bool mouseDownRight = false;

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	AppState& app = *static_cast<AppState*>(appstate);
	
	ImGui_ImplSDL3_ProcessEvent(event);
	const ImGuiIO & imio = ImGui::GetIO();
	
	switch(event->type) {
		case SDL_EVENT_QUIT: {
			g_userRequestedQuit = true;
			break;
		}
		case SDL_EVENT_WINDOW_EXPOSED:
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		case SDL_EVENT_WINDOW_RESIZED: {
			reshape(app);
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			if(!imio.WantCaptureMouse) {
			    if(event->button.button == SDL_BUTTON_LEFT) {
					mouseDownLeft = true;
				    g_global.camera->mouseRotateStart(event->button.x, event->button.y);
				}
			    if(event->button.button == SDL_BUTTON_RIGHT) {
					mouseDownRight = true;
				    g_global.camera->mouseRotateStart(event->button.x, event->button.y);
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			if(!imio.WantCaptureMouse) {
				if(event->button.button == SDL_BUTTON_LEFT) {
					mouseDownLeft = false;
				}
				if(event->button.button == SDL_BUTTON_RIGHT) {
					mouseDownRight = false;
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			if(!imio.WantCaptureMouse) {
				if(mouseDownLeft) {
					g_global.camera->rotateMouse(event->motion.x, event->motion.y);
				}
				if(mouseDownRight) {
					g_global.camera->moveMouse(event->motion.x, event->motion.y);
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_WHEEL: {
			if(!imio.WantCaptureMouse) {
				if(event->wheel.y > 0) {
					g_global.camera->move(0, 0, 1);
					//					    if(!g_global.camera->isPerspective()) {
					//						    reshape(g_mainWindow.getSize().x,
					//						            mainWindow.getSize().y);
					//					    }
				} else if(event->wheel.y < 0) {
					g_global.camera->move(0, 0, -1);
					//					    if(!g_global.camera->isPerspective()) {
					//						    reshape(mainWindow.getSize().x,
					//						            mainWindow.getSize().y);
					//					    }
				}
			}
			break;
		}
		case SDL_EVENT_KEY_UP: {
			switch(event->key.key) {
				case SDLK_UP:
					g_global.camera->move(0, 1, 0);
					break;
				case SDLK_DOWN:
					g_global.camera->move(0, -1, 0);
					break;
				case SDLK_LEFT:
					g_global.camera->move(-1, 0, 0);
					break;
				case SDLK_RIGHT:
					g_global.camera->move(1, 0, 0);
					break;
				case SDLK_KP_PLUS:
					g_global.camera->move(0, 0, 1);
					break;
				case SDLK_KP_MINUS:
					g_global.camera->move(0, 0, -1);
					break;
				case SDLK_KP_8:
					g_global.camera->rotate(-glm::pi<float>() / 12, Animorph::X_AXIS);
					break;
				case SDLK_KP_2:
					g_global.camera->rotate(glm::pi<float>() / 12, Animorph::X_AXIS);
					break;
				case SDLK_KP_1:
					g_global.camera->resetRotation();
					// camera->resetPosition();
					// camera->rotate (-M_PI/2, X_AXIS);
					// camera->move (0,0,-75);
					break;
				case SDLK_KP_7:
					g_global.camera->resetRotation();
					g_global.camera->rotate(glm::pi<float>() / 2, Animorph::X_AXIS);
					break;
				case SDLK_KP_6:
					g_global.camera->rotate(-glm::pi<float>() / 12, Animorph::Y_AXIS);
					break;
				case SDLK_KP_5:
					g_global.camera->setPerspective(!g_global.camera->isPerspective());
					reshape(app);
					break;
				case SDLK_KP_4:
					g_global.camera->rotate(glm::pi<float>() / 12, Animorph::Y_AXIS);
					break;
				case SDLK_KP_3:
					g_global.camera->resetRotation();
					g_global.camera->rotate(-glm::pi<float>() / 2, Animorph::Y_AXIS);
					break;
				case SDLK_KP_COMMA:
					g_global.camera->resetPosition();
					g_global.camera->move(0, 0, -125);
					break;
				case SDLK_F11:
					g_global.toggleFullscreen();
					break;
				default:
					break;
			}
			break;
		}
	}
	
	return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	AppState& app = *static_cast<AppState*>(appstate);
	
	if(result == SDL_APP_SUCCESS) {
		log_info("Shutting down...");
	}
	
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	
	{
		auto & p = g_config.windowMain.pos;
		SDL_GetWindowPosition(app.mainWindow, &p.x, &p.y);
		
		glm::ivec2 topLeftBorder;
		SDL_GetWindowBordersSize(app.mainWindow,
		                         &topLeftBorder.y, &topLeftBorder.x,
		                         nullptr, nullptr);
		
		p -= topLeftBorder;
	}
	{
		auto & s = g_config.windowMain.siz;
		SDL_GetWindowSize(app.mainWindow, &s.x, &s.y);
	}
	
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DestroyContext(app.mainContext);
	SDL_DestroyWindow(app.mainWindow);
	SDL_Quit();
	
	SaveConfig();
	
	vfs::deinit();
	
	delete static_cast<AppState*>(appstate);
}
