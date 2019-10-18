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
#include "MhRender.h"
#include "Vfs.h"

#include "physfs.h"

#include "SDL2/SDL.h"

#define kTimerRendering 1000


static constexpr char mh_app_name[] = "MyHumanoid";
static constexpr char mh_version[]  = "0.1.0";

bool  init; // shows the init status
int   average           = 0;
int   n_display         = 0;
int   tickCount         = 0;
float kTimePerRaster(0.03f);
bool  oldCameraTimerTrigger = false;

const Color border_color(1.0, 0.55, 0.0, 0.8);
const Color grid_color(0.35, 0.50, 0.30, 0.50);
const Color edges_color(0.4, 0.3, 0.3, 0.5);

#include <nfd.h>

void openFileDialog()
{
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
	
	if ( result == NFD_OKAY ) {
		puts("Success!");
		puts(outPath);
		free(outPath);
	}
	else if ( result == NFD_CANCEL ) {
		puts("User pressed cancel.");
	}
	else {
		printf("Error: %s\n", NFD_GetError() );
	}
	
	//return 0;
}

std::optional<std::string> saveFileDialog(const std::string & ending) {
	const nfdchar_t * filterList = ending.c_str();
	const nfdchar_t * defaultPath = "./";
	nfdchar_t * outPath = NULL;
	
	auto res = NFD_SaveDialog(filterList, defaultPath, &outPath);
	switch(res) {
	case NFD_OKAY: {
		std::string path(outPath);
		free(outPath);
		if(!endsWithCaseInsensitive(path, "." + ending)) {
			return path + "." + ending;
		} else {
			return path;
		}
	}
	case NFD_CANCEL:
		return std::nullopt;
	default:
		log_error("Save Dialog failed: {}", NFD_GetError());
		return std::nullopt;
	}
}



// ================================================================================================

static IconMap g_charactersIconTextures;

static void CreateCaractersIconTextures()
{
	loadTexturesFromDir(g_charactersIconTextures, "data/pixmaps/bsimg");
}

// ================================================================================================

static void saveBodySettings(const string & filename)
{
	Animorph::BodySettings bodyset = g_mesh.morphTargets();
	
	//bodyset.m_comp = g_global.m_comp;
	g_grids.toSavedPositions(bodyset.m_comp);
	
	bool state = bodyset.save(filename);

	if(state) {
		log_info("BodySettings saved");
	} else {
		log_error("Error: couldn't save file.");
	}
}

static void loadBodySettings(const string & filename)
{
	Animorph::BodySettings bodyset;
	bool         state = bodyset.load(filename);

	if(state) {
		g_global.clearFuzzy();
		
		//g_global.m_comp = bodyset.m_comp;
		g_grids.fromSavedPositions(bodyset.m_comp);
		
		//mhgui::g_mainWindow->characterSettingPanel->updateUi();
	}

	if(state) {
		g_mesh.doMorph(bodyset);
		g_mesh.calcNormals();
		log_info("BodySettings loaded");
	} else {
		log_info("BodySettings load failed");
	}
}



static void savePoses(const string & filename)
{
	Animorph::BodySettings poses = g_mesh.poseTargets();

	bool state = poses.save(filename);

	if(state) {
		log_info("Poses saved");
	} else {
		log_error("Save pose failed");
	}
}

static void loadPoses(const string & filename)
{
	Animorph::BodySettings poses;
	bool         state = poses.load(filename);

	if(state) {
		g_mesh.doPose(poses);
		log_info("Poses loaded");
	} else {
		log_error("Poses load failed");
	}
}


static void exportMeshObj(string & fileName)
{
	log_info("Saving obj to: {}", fileName);
	
	Animorph::ObjExporter exporter(g_mesh);

	bool state = exporter.exportFile(fileName);

	if(state) {
		log_info("OBJ exported");
	} else {
		log_error("OBJ export failed");
	}
}

static void exportCollada(string & filename)
{
	Animorph::ColladaExporter collada_export(g_mesh);

	if(filename.substr(filename.size() - 1, 1) != "/") {
		filename.append("/");
	}

	//fs::create_directories(filename);

	bool state = collada_export.exportFile(filename);

	if(state) {
		log_info("Collada exported");
	} else {
		log_error("Collada export failed");
	}
}

static void saveAutozoom(const string & filename)
{
	bool state = g_global.autozoom->save(filename, *g_global.camera);

	if(state) {
		log_info("Autozoom saved");
	} else {
		log_error("Autozoom save failed");
	}
}


static void ResetMeshMorph()
{
	g_grids.clearPos();
	
	Animorph::BodySettings bodyset_empty;
	g_mesh.doMorph(bodyset_empty);

	loadDefaultBodySettings();
	
	g_global.clearFuzzy();
}

// ================================================================================================
// ================================================================================================

static bool g_userRequestedQuit = false;
static bool g_displayAxis       = false;

static bool g_requestShaderReload  = false;
static int  g_requestShaderVersion = 1;

static bool g_requestBackgroundShaderReload = false;

// ================================================================================================
// ================================================================================================
// ================================================================================================
// ================================================================================================

void loadCharacter(const std::string & character_name)
{
	const Animorph::CharactersMap & charactersmap = g_mesh.characters();
	
	auto f = charactersmap.find(character_name);
	if(f != charactersmap.end()) {
		g_mesh.doMorph(f->second);
		g_mesh.calcNormals();
		
		auto & bodyset = f->second;
		
		//g_global.m_comp = bodyset.m_comp;
		g_grids.fromSavedPositions(bodyset.m_comp);
		
		//mhgui::g_mainWindow->characterSettingPanel->updateUi();
	}
}

void DisplayLibraryCharacters()
{

	const static string kFilePrefixTarget(".bs");
	const static string kFilePrefixPNG(".png");

	for(const auto & charactersmap_it : g_mesh.characters()) {
		const string & character_name(charactersmap_it.first);

		string::size_type loc = character_name.find("/", 0);
		if(loc == string::npos)
			continue;

		string sub = character_name.substr(0, loc);

		if(sub != "characters1")
			continue;
		
		
		std::string foobar = removeExtension(character_name);

		// remove ".bs"
		string character_image(character_name);
		character_image.replace(character_image.length() - kFilePrefixTarget.length(),
		                        kFilePrefixTarget.length(), kFilePrefixPNG);

		// logger("asd {}", std::string(foobar));

		IconMap::iterator icon = g_charactersIconTextures.find(foobar);
		if(icon != g_charactersIconTextures.end()) {
			const auto & tex = icon->second;
			if(ImGui::ImageButton((void *)(intptr_t)tex.handle, ImVec2(48, 48))) {
				loadCharacter(character_name);
			}
			ImGui::Text("%s", foobar.c_str());
		} else {
			if(ImGui::Button(foobar.c_str())) {
				loadCharacter(character_name);
			}
		}
	}
}

void loadPose(const std::string & character_name)
{
	const Animorph::CharactersMap & charactersmap = g_mesh.characters();
	
	auto f = charactersmap.find(character_name);
	if(f != charactersmap.end()) {
		g_mesh.doPose(f->second, 1.0, true);
		// mesh->doPose
		// (charactersmap[imgSource->getTargetName ()],
		// false);
		g_mesh.calcNormals();
	} else {
		log_info("Character {} not found", character_name);
	}
}

void DisplayLibraryPoses()
{

	const static string kFilePrefixTarget(".bs");
	const static string kFilePrefixPNG(".png");

	for(const auto & charactersmap_it : g_mesh.characters()) {
		const string & character_name(charactersmap_it.first);

		string::size_type loc = character_name.find("/", 0);
		if(loc == string::npos)
			continue;

		string sub = character_name.substr(0, loc);

		if(sub != "poses1")
			continue;
		
		
		std::string foobar = removeExtension(character_name);

		// remove ".bs"
		string character_image(character_name);
		character_image.replace(character_image.length() - kFilePrefixTarget.length(),
		                        kFilePrefixTarget.length(), kFilePrefixPNG);

		// logger("asd {}", std::string(foobar));

		IconMap::iterator icon = g_charactersIconTextures.find(foobar);
		if(icon != g_charactersIconTextures.end()) {
			const auto & tex = icon->second;
			if(ImGui::ImageButton((void *)(intptr_t)tex.handle, ImVec2(48, 48))) {
				
				loadPose(character_name);
			}
		} else {
			if(ImGui::Button(foobar.c_str())) {
				loadPose(character_name);
			}
		}
	}
}

void DisplayPerformance()
{
	ImGui::Begin("Performance");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
	            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Demo Window", &g_displayWin.show_demo_window);
	ImGui::End();
}


void DisplayGlInfo()
{
	if(!ImGui::Begin("Gl info", &g_displayWin.glInfo)) {
		ImGui::End();
		return;
	}
	
	ImGui::Text("Vendor:   %s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text("Version:  %s", glGetString(GL_VERSION));
	ImGui::Text("GLSL:     %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	if (ImGui::CollapsingHeader("Extensions")) {
		static ImGuiTextFilter filter;
		filter.Draw();
		
		GLint count;
		glGetIntegerv(GL_NUM_EXTENSIONS, &count);
		for(GLint i = 0; i < count; ++i) {
			auto extName = (const char *)(glGetStringi(GL_EXTENSIONS, i));
			if(extName != nullptr && filter.PassFilter(extName)) {
				ImGui::Text("%s", extName);
			}
		}
	}
	
	ImGui::End();
}

void DisplayAbout()
{

	constexpr static ImGuiWindowFlags winFlags =
	        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
	        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

	if(!ImGui::Begin("About", &g_displayWin.about, winFlags)) {
		ImGui::End();
		return;
	}

	ImGui::Text(mh_app_name);
	ImGui::SameLine();
	ImGui::Text(mh_version);

	ImGui::Text("Developer:");
	ImGui::Text("  Eli2");
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Text("Based on:");
	ImGui::Separator();
	ImGui::Text("Make Human 0.9.1 RC1");
	ImGui::Text("Core Team:");
	ImGui::Text("  Manuel Bastioni: project admin");
	ImGui::Text("  Simone Re from Ninibelabs: main coder");
	ImGui::Text("  Letizia Beriozza from Ninibelabs: webmaster");
	ImGui::Text("  Alessandro Proglio: coder");
	ImGui::Text("  Emanuele Di Mattia: coder");
	ImGui::Text("  Giovanni Lanza: blogosphere");
	ImGui::Text("Maintainers team:");
	ImGui::Text("  Andreas Volz: Linux version");
	ImGui::Text("  Hans-Peter Dusel: OSX version");
	ImGui::Text("  Romaln Behar: Linux/OSX version");
	ImGui::Text("Doc/Translations team:");
	ImGui::Text("  Martin Mackinlay");
	ImGui::Text("Artists:");
	ImGui::Text("  Luca Miragoli");
	ImGui::Text("  Cicca: targets");
	ImGui::Text("  Laura Sclavi: logo concept");

	ImGui::End();
}

// ================================================================================================

void DisplayQuitPopup()
{
	if(ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you really want to quit?\n");
		ImGui::Separator();
		if(ImGui::Button("YES", ImVec2(120, 0))) {
			g_userRequestedQuit = true;
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if(ImGui::Button("NO", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void DisplayMainMenu()
{
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			ImGui::Separator();
			if(ImGui::MenuItem("Export wavefront (.obj)")) {
				auto path = saveFileDialog("obj");
				if(path) {
					exportMeshObj(path.value());
				}
			}
			if(ImGui::MenuItem("Export Collada (.dae)")) {
				std::string filename = "foo-ColladaExport";
				exportCollada(filename);
			}
			ImGui::Separator();
			if(ImGui::Button("Quit...")) {
				ImGui::OpenPopup("Quit?");
			}
			DisplayQuitPopup();
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("View")) {
			ImGui::Checkbox("Grid", &g_global.drawGrid);
			ImGui::Checkbox("Quoted box", &g_global.quotedBox);
			ImGui::Checkbox("Texture", &g_global.m_enableTexture);
			ImGui::Checkbox("Axis", &g_displayAxis);

			ImGui::Separator();

			if(ImGui::BeginMenu("Toggle Visibility")) {
				for(auto & g : g_mesh.facegroup().m_groups) {
					if(ImGui::Button(g.first.c_str())) {
						g_mesh.facegroup().toggleVisible(g.first);
					}
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Save autozoom")) {

				// searchDataDir("targets")
				std::string fooAutozoom = "foo-Autozoom";

//				if(targetPanel) {
//					string      target            = targetPanel->getCategory();
//					std::string pathAutozoom_data = fooAutozoom + "/" + target +
//					                                "/" + target + ".camera";

//					saveAutozoom(pathAutozoom_data);
//				} else {
					saveAutozoom("foo-Default.camera");
//				}
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();
		if(ImGui::Checkbox("MorphMode", &g_morphMode)) {
			if(g_morphMode) {
				g_mesh.bodyDetailsMode();
			} else {
				g_mesh.poseMode();
			}
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Morph", g_morphMode)) {
			ImGui::Checkbox("Composite Morph", &g_config.characterSettings.visible);
			ImGui::Checkbox("Morph Targets", &g_config.morphTargets.visible);
			ImGui::Checkbox("Applied Morph Targets", &g_config.morphTargetsApplied.visible);
			ImGui::Separator();
			if(ImGui::MenuItem("Load")) {
				loadBodySettings("foo-BodySettings");
			}
			if(ImGui::MenuItem("Save")) {
				saveBodySettings("foo-BodySettings");
			}
			ImGui::Separator();
			if(ImGui::BeginMenu("Library")) {
				DisplayLibraryCharacters();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if(ImGui::BeginMenu("Reset Morph? ...")) {
				if(ImGui::MenuItem("YES")) {
					ResetMeshMorph();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Pose", !g_morphMode)) {
			ImGui::Checkbox("Pose Targets", &g_config.poseTargets.visible);
			ImGui::Checkbox("Applied Pose Targets", &g_config.poseTargetsApplied.visible);
			ImGui::Separator();
			if(ImGui::MenuItem("Load")) {
				loadPoses("foo-Poses");
			}
			if(ImGui::MenuItem("Save")) {
				savePoses("foo-Poses");
			}
			ImGui::Separator();
			if(ImGui::BeginMenu("Library")) {
				DisplayLibraryPoses();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if(ImGui::BeginMenu("Reset Pose? ...")) {
				if(ImGui::MenuItem("YES")) {
					g_mesh.resetPose();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Shading")) {
			if(ImGui::MenuItem("Load Shader 1")) {
				g_requestShaderVersion = 1;
				g_requestShaderReload  = true;
			}
			if(ImGui::MenuItem("Load Shader 2")) {
				g_requestShaderVersion = 2;
				g_requestShaderReload  = true;
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Load Background Shader")) {
				g_requestBackgroundShaderReload = true;
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Broken stuff")) {
			if(ImGui::Button("CreateWeightsFile")) {
				CreateWeightsFile();
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Help")) {
#if PROFILER_ENABLED
			if(ImGui::MenuItem("Print Profiler Data")) {
				ProfilerPrint();
			}
			if(ImGui::MenuItem("Clear Profiler Data")) {
				ProfilerClear();
			}
#endif
			if(ImGui::MenuItem("Gl Info")) {
				g_displayWin.glInfo = true;
			}
			ImGui::Checkbox("Performance", &g_displayWin.performance);
			ImGui::Separator();
			if(ImGui::MenuItem("About ...")) {
				g_displayWin.about = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if(g_morphMode) {
		if(g_config.characterSettings.visible) {
			DisplayCharacterSettings();
		}
		if(g_config.morphTargets.visible) {
			DisplayMorphTargets();
		}
		if(g_config.morphTargetsApplied.visible) {
			DisplayMorphTargetsApplied();
		}
	} else {
		if(g_config.poseTargets.visible) {
			DisplayPoseTargets();
		}
		if(g_config.poseTargetsApplied.visible) {
			DisplayPoseTargetsApplied();
		}
	}
	
	if(g_displayWin.glInfo) {
		DisplayGlInfo();
	}
	
	if(g_displayWin.performance) {
		DisplayPerformance();
		if(g_displayWin.show_demo_window) {
			ImGui::ShowDemoWindow(&g_displayWin.show_demo_window);
		}
	}
	if(g_displayWin.about) {
		DisplayAbout();
	}
}

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

//static void special(int key)
//{
//		switch(key) {
//		case GLUT_KEY_UP:
//			g_global.camera->move(0, 1, 0);
//			break;
//		case GLUT_KEY_DOWN:
//			g_global.camera->move(0, -1, 0);
//			break;
//		case GLUT_KEY_LEFT:
//			g_global.camera->move(-1, 0, 0);
//			break;
//		case GLUT_KEY_RIGHT:
//			g_global.camera->move(1, 0, 0);
//			break;
//		}
//}

static void keyboard(SDL_Keycode key)
{
	switch(key) {
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

struct SDLWindow {
	
	SDL_Window *mainwindow; /* Our window handle */
	SDL_GLContext maincontext; /* Our opengl context handle */
	
	SDLWindow(const Rect & rect, const std::string & title)
	{
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			throw std::runtime_error("Unable to initialize SDL");
		}
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		
		/* Create our window centered at 512x512 resolution */
		mainwindow = SDL_CreateWindow(title.c_str(),
		                              rect.pos.x,
		                              rect.pos.y,
		                              rect.size.x,
		                              rect.size.y,
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
	}
	
	void swap()
	{
		SDL_GL_SwapWindow(mainwindow);
	}
	
	~SDLWindow()
	{
		/* Delete our opengl context, destroy our window, and shutdown SDL */
		SDL_GL_DeleteContext(maincontext);
		SDL_DestroyWindow(mainwindow);
		SDL_Quit();
	}
};

int main(int argc, char ** argv)
{
	if(argc == 2) {
		if(argv[1] && argv[1] == std::string("--debug")) {
			g_logLevel = LogLevel::debug;
		}
	}
	
	PHYSFS_init(argv[0]);
	vfs::init();
	
	LoadConfig();
	
	// FIXME WTF why does it move ? window decoration ?
	g_config.windowMain.pos -= glm::ivec2(2, 21);

	Rect mainWinRect = Rect(g_config.windowMain.pos.x, g_config.windowMain.pos.y,
	                        g_config.windowMain.siz.x, g_config.windowMain.siz.y);
	
	std::string winTitle = mh_app_name + std::string(" ") + mh_version;

	// g_global.mesh     = new Mesh();
	g_global.camera   = new mhgui::Camera();
	g_global.autozoom = new mhgui::Autozoom();
	
	
	SDLWindow sdlWindow(mainWinRect, winTitle);
	
	
	//mhgui::g_mainWindow->initWindow();
	
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
	
//	glutCloseFunc([]() -> void {
//		// TODO glut does not let us prevent closing in a sane way
//		// just let it happen for now :/

//		//		g_userRequestedQuit = true;
//		//		Window &mainWindow(*g_mainWindow);

//		// TODO WHY?
//		// mainWindow.mainLoop();
//	});

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
	
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow.mainwindow, sdlWindow.maincontext);
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
					runMainloop = false;
					break;
				}
				case SDL_WINDOWEVENT: {
					switch(event.window.event) {
						case SDL_WINDOWEVENT_CLOSE: {
							if(event.window.windowID == SDL_GetWindowID(sdlWindow.mainwindow)) {
								runMainloop = false;
							}
							break;
						}
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
		ImGui_ImplSDL2_NewFrame(sdlWindow.mainwindow);
		ImGui::NewFrame();
		
		timerTrigger();
		
		g_global.camera->applyMatrix();
		
		g_renderBody.render();
		
		g_renderBackground.render();
		
		DisplayMainMenu();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if(g_requestShaderReload) {
			g_requestShaderReload = false;
			
			g_renderBody.loadShader(g_requestShaderVersion);
		}
		if(g_requestBackgroundShaderReload) {
			g_requestBackgroundShaderReload = false;
			
			g_renderBackground.loadShader();
		}
		
		sdlWindow.swap();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	{
		auto & p = g_config.windowMain.pos;
		SDL_GetWindowPosition(sdlWindow.mainwindow, &p.x, &p.y);
	}
	{
		auto & s = g_config.windowMain.siz;
		SDL_GetWindowSize(sdlWindow.mainwindow, &s.x, &s.y);
	}

	SaveConfig();
	
	vfs::deinit();
	
	return 0;
}
