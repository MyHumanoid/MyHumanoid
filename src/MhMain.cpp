// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2

#include <fmt/format.h>

#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "adapter/imgui_adapter.h"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "animorph/Mesh.h"
#include "animorph/util.h"
#include "gui/CGUtilities.h"
#include "gui/Camera.h"
#include "render/RenderUtils.h"
#include "Global.h"
#include "Logger.h"
#include "MhConfig.h"
#include "MhUiMain.h"
#include "MhRender.h"
#include "MhUiMorph.h"
#include "MhUiPose.h"
#include "Profiler.h"
#include "util.h"
#include "Version.h"
#include "Vfs.h"




static void parseArgs(AppState& app, int argc, char **argv)
{
	if(argc > 100) {
		log_error("Too many arguments");
		return;
	}
	
	const std::vector<std::string_view> args(argv + 1, argv + argc);
	for(const auto& arg : args) {
		if(arg == "--debug") {
			app.args.debug = true;
			continue;
		}
		if(arg == "--viewer") {
			app.args.editor = false;
			continue;
		}
		if(arg == "--no-background") {
			app.args.background = false;
			continue;
		}
	}
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
	*appstate = new AppState;
	AppState& app = *static_cast<AppState*>(*appstate);
	
	parseArgs(app, argc, argv);
	
	if(app.args.debug) {
		g_logLevel = LogLevel::debug;
	}
	
	vfs::init(argv[0]);
	
	LoadConfig(app);
	
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		log_error("Unable to initialize SDL");
		return SDL_APP_FAILURE;
	}
	
	agl::setWindowAttributes();
	
	std::string title = mh_app_name + std::string(" ") + prettyVersion();
	
	const auto & winRect = g_config.windowMain;
	
	SDL_WindowFlags winFlags = 0;
	winFlags |= SDL_WINDOW_OPENGL;
	winFlags |= SDL_WINDOW_RESIZABLE;
	if(!app.args.background) {
		winFlags |= SDL_WINDOW_TRANSPARENT;
	}
	
	app.mainWindow = SDL_CreateWindow(title.c_str(), winRect.siz.x, winRect.siz.y, winFlags);
	
	if (!app.mainWindow) {
		log_error("Unable to create window");
		return SDL_APP_FAILURE;
	}
	
	if(!app.args.background) {
		SDL_SetWindowOpacity(app.mainWindow, 0.f);
	}
	
	if(app.args.editor) {
		SDL_SetWindowMinimumSize(app.mainWindow, 800, 600);
	}

	if(auto iconSurface = loadSurfaceFromFile("data/pixmaps/icon.png")) {
		SDL_SetWindowIcon(app.mainWindow, iconSurface->m_ptr);
		iconSurface->free();
	}
	
	//checkSDLError(__LINE__);
	
	/* Create our opengl context and attach it to our window */
	app.mainContext = SDL_GL_CreateContext(app.mainWindow);
	if(!agl::init()) {
		printf("Failed to initialize OpenGL context\n");
		return SDL_APP_FAILURE;
	}
	
	TracyGpuContext;
	
	printTracingStatus();
	
	// ===========
	
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0, 0, 0, 0.f);
	
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);
	
	
	app.camera   = std::make_unique<mhgui::Camera>();
	app.autozoom = std::make_unique<mhgui::Autozoom>();
	
	
	g_renderBody.init();
	
	if(app.args.background) {
		g_renderBackground.init();
	}
	
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

	g_mesh.loadTargets(searchDataDir("targets"));
	g_mesh.loadTargets(searchDataDir("selectors"), 1, false);
	g_mesh.loadPoseTargets(searchDataDir("rotations"));
	g_mesh.loadCharacters(searchDataDir("bs_data"));

	// camera->rotate (-glm::pi<float>()/2, X_AXIS);
	app.camera->move(0, 0, -125.0f);
	
	::glPolygonOffset(1.0, 1.0);
	
	loadCharacter("characters1/_default.bs");
	
	if(app.args.editor) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io     = ImGui::GetIO();
		io.IniSavingRate = 60.f;
		io.IniFilename   = "MyHumanoid-config-gui.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
		ImGui::StyleColorsDark();
		
		ImGui_ImplSDL3_InitForOpenGL(app.mainWindow, app.mainContext);
		#ifdef AGL_USE_GLES
			const char* glsl_version = "#version 300 es";
		#else
			const char* glsl_version = "#version 130";
		#endif
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

static void reshape(AppState& app)
{
	int w;
	int h;
	SDL_GetWindowSizeInPixels(app.mainWindow, &w, &h);
	
	cgutils::reshape(glm::ivec2(w, h), *app.camera);
	app.camera->reshape(w, h);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	AppState& app = *static_cast<AppState*>(appstate);
	
	FrameMark;
	ZoneScoped;
	
	if(app.args.editor) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}
	
	app.camera->timerTrigger();
	
	if(!app.camera->isPerspective()) {
		reshape(app);
	}
	
	app.camera->applyMatrix();
	
	g_renderBody.render();
	
	if(app.args.background) {
		g_renderBackground.render(app);
	}
	
	if(app.args.editor) {
		DisplayMainMenu(app);
	}
	
	if(app.args.editor) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	ExecuteDeferredActions();
	
	SDL_GL_SwapWindow(app.mainWindow);
	TracyGpuCollect;
	
	if(app.ui.request.quitAccept) {
		return SDL_APP_SUCCESS;
	} else {
		return SDL_APP_CONTINUE;
	}
}

bool mouseDownLeft = false;
bool mouseDownRight = false;

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	AppState& app = *static_cast<AppState*>(appstate);
	
	bool guiWantCaptureMouse = false;
	bool guiWantCaptureKeyboard = false;
	if(app.args.editor) {
		ImGui_ImplSDL3_ProcessEvent(event);
		const ImGuiIO & imio = ImGui::GetIO();
		guiWantCaptureMouse = imio.WantCaptureMouse;
		guiWantCaptureKeyboard = imio.WantCaptureKeyboard;
	}
	
	switch(event->type) {
		case SDL_EVENT_QUIT: {
			if(app.args.editor) {
				app.ui.request.quit = true;
			} else {
				app.ui.request.quitAccept = true;
			}
			break;
		}
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		case SDL_EVENT_WINDOW_RESIZED: {
			reshape(app);
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			if(guiWantCaptureMouse) {
				break;
			}
			if(event->button.button == SDL_BUTTON_LEFT) {
				mouseDownLeft = true;
				app.camera->mouseRotateStart(event->button.x, event->button.y);
			}
			if(event->button.button == SDL_BUTTON_RIGHT) {
				mouseDownRight = true;
				app.camera->mouseRotateStart(event->button.x, event->button.y);
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			if(guiWantCaptureMouse) {
				break;
			}
			if(event->button.button == SDL_BUTTON_LEFT) {
				mouseDownLeft = false;
			}
			if(event->button.button == SDL_BUTTON_RIGHT) {
				mouseDownRight = false;
			}
			break;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			if(guiWantCaptureMouse) {
				break;
			}
			if(mouseDownLeft) {
				app.camera->rotateMouse(event->motion.x, event->motion.y);
			}
			if(mouseDownRight) {
				app.camera->moveMouse(event->motion.x, event->motion.y);
			}
			break;
		}
		case SDL_EVENT_MOUSE_WHEEL: {
			if(guiWantCaptureMouse) {
				break;
			}
			if(event->wheel.y > 0) {
				app.camera->move(0, 0, 1);
				//					    if(!g_global.camera->isPerspective()) {
				//						    reshape(g_mainWindow.getSize().x,
				//						            mainWindow.getSize().y);
				//					    }
			} else if(event->wheel.y < 0) {
				app.camera->move(0, 0, -1);
				//					    if(!g_global.camera->isPerspective()) {
				//						    reshape(mainWindow.getSize().x,
				//						            mainWindow.getSize().y);
				//					    }
			}
			break;
		}
		case SDL_EVENT_KEY_UP: {
			if(guiWantCaptureKeyboard) {
				break;
			}
			
			switch(event->key.key) {
				case SDLK_UP:
					app.camera->move(0, 1, 0);
					break;
				case SDLK_DOWN:
					app.camera->move(0, -1, 0);
					break;
				case SDLK_LEFT:
					app.camera->move(-1, 0, 0);
					break;
				case SDLK_RIGHT:
					app.camera->move(1, 0, 0);
					break;
				case SDLK_KP_PLUS:
					app.camera->move(0, 0, 1);
					break;
				case SDLK_KP_MINUS:
					app.camera->move(0, 0, -1);
					break;
				case SDLK_KP_8:
					app.camera->rotate(-glm::pi<float>() / 12, Animorph::X_AXIS);
					break;
				case SDLK_KP_2:
					app.camera->rotate(glm::pi<float>() / 12, Animorph::X_AXIS);
					break;
				case SDLK_KP_1:
					app.camera->resetRotation();
					// camera->resetPosition();
					// camera->rotate (-M_PI/2, X_AXIS);
					// camera->move (0,0,-75);
					break;
				case SDLK_KP_7:
					app.camera->resetRotation();
					app.camera->rotate(glm::pi<float>() / 2, Animorph::X_AXIS);
					break;
				case SDLK_KP_6:
					app.camera->rotate(-glm::pi<float>() / 12, Animorph::Y_AXIS);
					break;
				case SDLK_KP_5:
					app.camera->setPerspective(!app.camera->isPerspective());
					reshape(app);
					break;
				case SDLK_KP_4:
					app.camera->rotate(glm::pi<float>() / 12, Animorph::Y_AXIS);
					break;
				case SDLK_KP_3:
					app.camera->resetRotation();
					app.camera->rotate(-glm::pi<float>() / 2, Animorph::Y_AXIS);
					break;
				case SDLK_KP_COMMA:
					app.camera->resetPosition();
					app.camera->move(0, 0, -125);
					break;
				case SDLK_F11:
					toggleFullscreen(app);
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
	if(app.args.editor) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}
	
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
	
	SaveConfig(app);
	
	vfs::deinit();
	
	delete static_cast<AppState*>(appstate);
}
