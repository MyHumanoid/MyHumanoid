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
#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

#include "GlInclude.h"

#include "MhUi.h"
#include "MhUiMorph.h"
#include "MhUiPose.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <animorph/Mesh.h>
#include <animorph/util.h>
#include <animorph/ColladaExporter.h>
#include <animorph/ObjExporter.h>

#include <gui/CGUtilities.h>
#include <gui/Camera.h>

#include "Logger.h"
#include "Profiler.h"

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
#include "Vfs.h"

#include "SDL2/SDL.h"

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

glm::ivec2 g_mainWinSize;

static void reshape()
{
	int w = g_mainWinSize.x;
	int h = g_mainWinSize.y;
	
	cgutils::reshape(glm::ivec2(w, h), *g_global.camera);
	g_global.camera->reshape(w, h);
}

static void timerTrigger()
{
	bool     tmp;
	tmp = g_global.camera->timerTrigger();

	if(!g_global.camera->isPerspective()) {
		reshape();
	}
}

static void keyboard(SDL_Keycode key)
{
	switch(key) {
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
			reshape();
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
		default:
			break;
	}
}

SDL_Window *mainwindow; /* Our window handle */
SDL_GLContext maincontext; /* Our opengl context handle */

int main2(int argc, char * argv[])
{
	if(argc == 2) {
		if(argv[1] && argv[1] == std::string("--debug")) {
			g_logLevel = LogLevel::debug;
		}
	}
	
	// throw std::overflow_error("TEST exception");
	
	vfs::init(argv[0]);
	
	LoadConfig();
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error("Unable to initialize SDL");
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	
	std::string title = mh_app_name + std::string(" ") + mh_version;
	
	const auto & winRect = g_config.windowMain;
	
	mainwindow = SDL_CreateWindow(title.c_str(),
	                              winRect.pos.x,
	                              winRect.pos.y,
	                              winRect.siz.x,
	                              winRect.siz.y,
	                              SDL_WINDOW_OPENGL
	                                  | SDL_WINDOW_SHOWN
	                                  | SDL_WINDOW_RESIZABLE);
	
	if (!mainwindow) {
		throw std::runtime_error("Unable to create window");
	}
	
	SDL_SetWindowMinimumSize(mainwindow, 800, 600);
	
	//checkSDLError(__LINE__);
	
	/* Create our opengl context and attach it to our window */
	maincontext = SDL_GL_CreateContext(mainwindow);
	//checkSDLError(__LINE__);
	
	
	glewExperimental = true;
	GLenum err       = glewInit();
	if(err != GLEW_OK) {
		log_error("GLEW error: {}", glewGetErrorString(err));
	}
	
	log_info("GL   version: {}", glGetString(GL_VERSION));
	log_info("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	log_info("GLEW Version: {}.{}.{}",
	         GLEW_VERSION_MAJOR,
	         GLEW_VERSION_MINOR,
	         GLEW_VERSION_MICRO);
	
	initDebugGl();
	
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
		return 1;
	}
	if(!material_loaded) {
		log_error("couldn't load mesh material informations");
		return 1;
	}
	if(!groups_loaded) {
		log_error("couldn't load face groups");
		return 1;
	}
	if(!smooth_loaded) {
		log_error("couldn't load smooth info");
		return 1;
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
	
	ImGui_ImplSDL2_InitForOpenGL(mainwindow, maincontext);
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
	
	bool mouseDownLeft = false;
	bool mouseDownRight = false;
	
	bool runMainloop = true;
	while(runMainloop) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			const ImGuiIO & imio = ImGui::GetIO();
			
			switch(event.type) {
				case SDL_QUIT: {
					g_userRequestedQuit = true;
					break;
				}
				case SDL_WINDOWEVENT: {
					switch(event.window.event) {
//						case SDL_WINDOWEVENT_CLOSE: {
//							if(event.window.windowID == SDL_GetWindowID(mainwindow)) {
//								g_userRequestedQuit = true;
//							}
//							break;
//						}
				        case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_RESIZED: {
							g_mainWinSize = {
								event.window.data1,
								event.window.data2
							};
							reshape();
							break;
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					if(!imio.WantCaptureMouse) {
						if(event.button.button == SDL_BUTTON_LEFT) {
							mouseDownLeft = true;
							g_global.camera->mouseRotateStart(event.button.x, event.button.y);
						}
						if(event.button.button == SDL_BUTTON_RIGHT) {
							mouseDownRight = true;
							g_global.camera->mouseRotateStart(event.button.x, event.button.y);
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					if(!imio.WantCaptureMouse) {
						if(event.button.button == SDL_BUTTON_LEFT) {
							mouseDownLeft = false;
						}
						if(event.button.button == SDL_BUTTON_RIGHT) {
							mouseDownRight = false;
						}
					}
					break;
				}
				case SDL_MOUSEMOTION: {
					if(!imio.WantCaptureMouse) {
						if(mouseDownLeft) {
							g_global.camera->rotateMouse(event.motion.x, event.motion.y);
						}
						if(mouseDownRight) {
							g_global.camera->moveMouse(event.motion.x, event.motion.y);
						}
					}
					break;
				}
				case SDL_MOUSEWHEEL: {
					if(!imio.WantCaptureMouse) {
						if(event.wheel.y > 0) {
							g_global.camera->move(0, 0, 1);
		//					    if(!g_global.camera->isPerspective()) {
		//						    reshape(g_mainWindow.getSize().x,
		//						            mainWindow.getSize().y);
		//					    }
						} else if(event.wheel.y < 0) {
							g_global.camera->move(0, 0, -1);
		//					    if(!g_global.camera->isPerspective()) {
		//						    reshape(mainWindow.getSize().x,
		//						            mainWindow.getSize().y);
		//					    }
						}
					}
					break;
				}
				case SDL_KEYUP: {
					keyboard(event.key.keysym.sym);
					break;
				}
			}
		}
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mainwindow);
		ImGui::NewFrame();
		
		timerTrigger();
		
		g_global.camera->applyMatrix();
		
		g_renderBody.render();
		
		g_renderBackground.render();
		
		DisplayMainMenu();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		ExecuteDeferredActions();
		
		SDL_GL_SwapWindow(mainwindow);
		
		if(g_userAcceptedQuit) {
			runMainloop = false;
		}
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	{
		auto & p = g_config.windowMain.pos;
		SDL_GetWindowPosition(mainwindow, &p.x, &p.y);
		
		glm::ivec2 topLeftBorder;
		SDL_GetWindowBordersSize(mainwindow,
		                         &topLeftBorder.y, &topLeftBorder.x,
		                         nullptr, nullptr);
		
		p -= topLeftBorder;
	}
	{
		auto & s = g_config.windowMain.siz;
		SDL_GetWindowSize(mainwindow, &s.x, &s.y);
	}
	
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
	
	SaveConfig();
	
	vfs::deinit();
	
	return 0;
}

int main(int argc, char * argv[])
{
	try {
		return main2(argc, argv);
	} catch(...) {
		// TODO log this
		auto eptr = std::current_exception();
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		                         "MyHumanoid",
		                         "ERROR\nFailed to start.\nCheck the log.",
		                         NULL);
		
		return 1;
	} 
}
