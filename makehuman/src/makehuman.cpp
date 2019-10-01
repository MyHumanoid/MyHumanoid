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

// TODO: create makehuman.h
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <experimental/filesystem>

#include <json.hpp>

#include "MhGui.h"
#include "MhGuiData.h"
#include "MhMorph.h"
#include "MhPose.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <animorph/Mesh.h>
#include <glm/vec3.hpp>
#include <animorph/util.h>
#include <animorph/ColladaExporter.h>
#include <animorph/ObjExporter.h>

#include <gui/CGUtilities.h>
#include <gui/Camera.h>
#include <gui/GLUTWrapper.h>
#include <gui/Image.h>
#include <gui/ImageSlider.h>
#include <gui/Panel.h>
#include <gui/Point.h>
#include <gui/Size.h>
#include <gui/Widget.h>
#include <gui/Window.h>

#include "log/log.h"

#include "render/RenderUtils.h"
#include "render/Shader.h"

#include <time.h>

#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include "TargetPanel.h"
#include "ToolbarPanel.h"
#include "TooltipPanel.h"
#include "util.h"

#define kTimerRendering 1000

namespace fs = std::experimental::filesystem;
using json   = nlohmann::json;

using namespace std;
using namespace Animorph;

static constexpr char mh_app_name[] = "MyHumanoid";
static constexpr char mh_version[]  = "0.1.0";


static void loadTextures();
static void drawBackground();
static void renderMesh();

json g_jsonConfig;

static TooltipPanel * tooltipPanel;
static ToolbarPanel * toolbarPanel;

static CharacterSettingPanel * characterSettingPanel;

bool  init; // shows the init status
float twopoints[6];
int   average           = 0;
int   n_display         = 0;
bool  right_button_down = false;
int   tickCount         = 0;
float kTimePerRaster(0.03f);
bool  oldCameraTimerTrigger = false;

const Color border_color(1.0, 0.55, 0.0, 0.8);
const Color grid_color(0.35, 0.50, 0.30, 0.50);
const Color edges_color(0.4, 0.3, 0.3, 0.5);



// ================================================================================================

static IconMap g_charactersIconTextures;

static void CreateCaractersIconTextures()
{
	fs::path baseDir = "pixmaps/bsimg/";
	loadTexturesFromDir(g_charactersIconTextures, baseDir);
}

// ================================================================================================

static void saveBodySettings(const string & filename)
{
	BodySettings bodyset = g_mesh.morphTargets();

	bool state = bodyset.save(filename);

	if(state) {
		state = saveSelectorsPositions(filename);
	}

	if(state) {
		logger("BodySettings saved");
	} else {
		logger_err("Error: couldn't save file.");
	}
}

static void loadBodySettings(const string & filename)
{
	Window & mainWindow = *g_mainWindow;

	BodySettings bodyset;
	bool         state = bodyset.load(filename);

	if(state) {
		g_global.resetFuzzyValues();
		state = loadSelectorsPositions(filename);

		CharacterSettingPanel * tmpPanel = (CharacterSettingPanel *)mainWindow.getPanel(
		        kComponentID_CharacterSettingPanel);
		if(tmpPanel != NULL) {
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Age);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Breast);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_MuscleSize);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Shape);
		}
	}

	if(state) {
		g_mesh.doMorph(bodyset);
		g_mesh.calcNormals();
		logger("BodySettings loaded");
	} else {
		logger("BodySettings load failed");
	}
}



static void savePoses(const string & filename)
{
	BodySettings poses = g_mesh.poseTargets();

	bool state = poses.save(filename);

	if(state) {
		logger("Poses saved");
	} else {
		logger_err("Save pose failed");
	}
}

static void loadPoses(const string & filename)
{
	BodySettings poses;
	bool         state = poses.load(filename);

	if(state) {
		g_mesh.doPose(poses);
		logger("Poses loaded");
	} else {
		logger_err("Poses load failed");
	}
}


static void exportBodySettings(string & directory, bool full)
{
	ObjExporter obj_export(g_mesh);

	if(directory.substr(directory.size() - 1, 1) != "/") {
		directory.append("/");
	}

	fs::create_directories(directory);

	bool state = obj_export.exportFile(directory, full);

	if(state) {
		logger("OBJ exported");
	} else {
		logger_err("OBJ export failed");
	}
}

static void exportCollada(string & filename)
{
	ColladaExporter collada_export(g_mesh);

	if(filename.substr(filename.size() - 1, 1) != "/") {
		filename.append("/");
	}

	fs::create_directories(filename);

	bool state = collada_export.exportFile(filename);

	if(state) {
		logger("Collada exported");
	} else {
		logger_err("Collada export failed");
	}
}

static void saveAutozoom(const string & filename)
{
	bool state = g_global.autozoom->save(filename, *g_global.camera);

	if(state) {
		logger("Autozoom saved");
	} else {
		logger_err("Autozoom save failed");
	}
}



static void ResetMeshPose()
{
	g_mesh.resetPose();
}

static void ResetMeshMorph()
{
	g_mesh.resetMorph();

	loadDefaultBodySettings();

	g_global.resetFuzzyValues();
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

void DisplayLibraryCharacters()
{

	const static string kFilePrefixTarget(".bs");
	const static string kFilePrefixPNG(".png");

	const CharactersMap & charactersmap = g_mesh.characters();
	for(CharactersMap::const_iterator charactersmap_it = charactersmap.begin();
	    charactersmap_it != charactersmap.end(); charactersmap_it++) {
		const string & character_name((*charactersmap_it).first);

		string::size_type loc = character_name.find("/", 0);
		if(loc == string::npos)
			continue;

		string sub = character_name.substr(0, loc);

		if(sub != "characters1")
			continue;


		fs::path foobar = character_name;
		foobar.replace_extension();

		// remove ".bs"
		string character_image(character_name);
		character_image.replace(character_image.length() - kFilePrefixTarget.length(),
		                        kFilePrefixTarget.length(), kFilePrefixPNG);

		// logger("asd {}", std::string(foobar));

		IconMap::iterator icon = g_charactersIconTextures.find(foobar);
		if(icon != g_charactersIconTextures.end()) {
			const auto & tex = icon->second;
			if(ImGui::ImageButton((void *)(intptr_t)tex.handle, ImVec2(48, 48))) {


				{ // not copy-paste
					//					if(g_global.getAppMode()
					//== POSES) {
					// g_global.setAppMode(BODY_SETTINGS);
					//						g_mesh.bodyDetailsMode();
					//					}

					const CharactersMap & charactersmap = g_mesh.characters();

					auto f = charactersmap.find(character_name);
					if(f != charactersmap.end()) {
						g_mesh.doMorph(f->second, 1.0, true);
						g_mesh.calcNormals();
						loadSelectorsPositions(f->second.cursorPositions);
					}
				}
			}
		}
	}
}

void DisplayLibraryPoses()
{

	const static string kFilePrefixTarget(".bs");
	const static string kFilePrefixPNG(".png");

	const CharactersMap & charactersmap = g_mesh.characters();
	for(CharactersMap::const_iterator charactersmap_it = charactersmap.begin();
	    charactersmap_it != charactersmap.end(); charactersmap_it++) {
		const string & character_name((*charactersmap_it).first);

		string::size_type loc = character_name.find("/", 0);
		if(loc == string::npos)
			continue;

		string sub = character_name.substr(0, loc);

		if(sub != "poses1")
			continue;


		fs::path foobar = character_name;
		foobar.replace_extension();

		// remove ".bs"
		string character_image(character_name);
		character_image.replace(character_image.length() - kFilePrefixTarget.length(),
		                        kFilePrefixTarget.length(), kFilePrefixPNG);

		// logger("asd {}", std::string(foobar));

		IconMap::iterator icon = g_charactersIconTextures.find(foobar);
		if(icon != g_charactersIconTextures.end()) {
			const auto & tex = icon->second;
			if(ImGui::ImageButton((void *)(intptr_t)tex.handle, ImVec2(48, 48))) {

				{ // not copy-paste
					const CharactersMap & charactersmap = g_mesh.characters();

					auto f = charactersmap.find(character_name);
					if(f != charactersmap.end()) {
						g_mesh.doPose(f->second, 1.0, true);
						// mesh->doPose
						// (charactersmap[imgSource->getTargetName ()],
						// false);
						g_mesh.calcNormals();
					} else {
						logger("Character {} not found", character_name);
					}
				}
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
				std::string filename = "foo-ObjExport";
				exportBodySettings(filename, false);
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

				Window &      mainWindow(*g_mainWindow);
				TargetPanel * targetPanel = dynamic_cast<TargetPanel *>(
				        mainWindow.getPanel(kComponentID_TargetPanel));

				if(targetPanel) {
					string      target            = targetPanel->getCategory();
					std::string pathAutozoom_data = fooAutozoom + "/" + target +
					                                "/" + target + ".camera";

					saveAutozoom(pathAutozoom_data);
				} else {
					saveAutozoom("foo-Default.camera");
				}
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
			ImGui::Checkbox("Composite Morph", &g_displayWin.characterSettings);
			ImGui::Checkbox("Morph Targets", &g_displayWin.morphTargets);
			ImGui::Checkbox("Applied Morph Targets", &g_displayWin.morphTargetsApplied);
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
					CharacterSettingPanel * cs_p = dynamic_cast<
					        CharacterSettingPanel *>(g_mainWindow->getPanel(
					        kComponentID_CharacterSettingPanel));
					if(cs_p != NULL) {
						cs_p->resetSlidersValues();
					}
					TargetPanel * tg_p = dynamic_cast<TargetPanel *>(
					        g_mainWindow->getPanel(kComponentID_TargetPanel));
					if(tg_p != NULL) {
						tg_p->resetTargetValues();
					}
					ResetMeshMorph();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Pose", !g_morphMode)) {
			ImGui::Checkbox("Pose Targets", &g_displayWin.poseTargets);
			ImGui::Checkbox("Applied Pose Targets", &g_displayWin.poseTargetsApplied);
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
					ResetMeshPose();
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
		if(g_displayWin.characterSettings) {
			DisplayCharacterSettings();
		}
		if(g_displayWin.morphTargets) {
			DisplayMorphTargets();
		}
		if(g_displayWin.morphTargetsApplied) {
			DisplayMorphTargetsApplied();
		}
	} else {
		if(g_displayWin.poseTargets) {
			DisplayPoseTargets();
		}
		if(g_displayWin.poseTargetsApplied) {
			DisplayPoseTargetsApplied();
		}
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

struct FooRect {
	glm::ivec2 pos;
	glm::ivec2 size;
};

FooRect g_mainWinRect;

std::optional<mh::Shader> g_bodyShader;
std::optional<mh::Shader> g_backgroundShader;

// Display function
static void display()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	g_global.camera->applyMatrix();

	renderMesh();

	if(g_displayAxis) {
		cgutils::drawAxis();
	}

	if(g_global.quotedBox)
		cgutils::mhWireCube(twopoints);

	drawBackground();
	g_mainWindow->draw();

	if(g_global.drawGrid) {
		cgutils::drawGrid(g_mainWindow->getSize(), 220, 70, grid_color, border_color, 50);
	} else {
		cgutils::drawSquare(Rect(0, 0, 1, 1), border_color);
	}

	DisplayMainMenu();

	ImGui::Render();
	// ImGuiIO& io = ImGui::GetIO();
	// glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	// glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	// glClear(GL_COLOR_BUFFER_BIT);
	// glUseProgram(0); // You may want this if using this code in an OpenGL 3+
	// context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();


	if(g_requestShaderReload) {
		g_requestShaderReload = false;

		logger("Loading Shader set {}", g_requestShaderVersion);

		std::optional<mh::Shader> shader;
		if(g_requestShaderVersion == 1) {
			shader = LoadShader("shader/body.vert", "shader/body.frag");
		} else {
			shader = LoadShader("shader/body_2.vert", "shader/body_2.frag");
		}

		if(shader) {
			glDeleteProgram(g_bodyShader->handle);
			g_bodyShader = shader.value();
		}
	}
	if(g_requestBackgroundShaderReload) {
		g_requestBackgroundShaderReload = false;
		logger("Loading Background Shader");

		auto shader = LoadShader("shader/background.vert", "shader/background.frag");

		if(shader) {
			glDeleteProgram(g_backgroundShader->handle);
			g_backgroundShader = shader.value();
		}
	}

	// TODO this is a hack
	g_mainWinRect.pos.x  = glutGet(GLUT_WINDOW_X);
	g_mainWinRect.pos.y  = glutGet(GLUT_WINDOW_Y);
	g_mainWinRect.size.x = glutGet(GLUT_WINDOW_WIDTH);
	g_mainWinRect.size.y = glutGet(GLUT_WINDOW_HEIGHT);

	if(g_userRequestedQuit) {
		glutLeaveMainLoop();
	}
};

static void reshape(int w, int h)
{
	Window & mainWindow(*g_mainWindow);
	mainWindow.reshape(Size(w, h), *g_global.camera);
	g_global.camera->reshape(w, h);
}

static void timerTrigger(int val)
{
	(void)val;

	bool     tmp;
	Window & mainWindow(*g_mainWindow);

	tmp = g_global.camera->timerTrigger();

	if(!g_global.camera->isPerspective()) {
		reshape(mainWindow.getSize().getWidth(), mainWindow.getSize().getHeight());
	}
	glutTimerFunc(50, timerTrigger, 1);
}

static void motion(int x, int y)
{
	Window & mainWindow(*g_mainWindow);
	mainWindow.isMouseOverPanel(Point(x, y));
}

static void special(int key)
{
	Window & mainWindow(*g_mainWindow);
	if(!mainWindow.isKeyTypePanel(key)) {
		switch(key) {
		case GLUT_KEY_UP:
			g_global.camera->move(0, 1, 0);
			break;
		case GLUT_KEY_DOWN:
			g_global.camera->move(0, -1, 0);
			break;
		case GLUT_KEY_LEFT:
			g_global.camera->move(-1, 0, 0);
			break;
		case GLUT_KEY_RIGHT:
			g_global.camera->move(1, 0, 0);
			break;
		}
	}
	glutPostRedisplay();
}

static void keyboard(unsigned char key)
{
	Window & mainWindow(*g_mainWindow);
	if(!mainWindow.isKeyTypePanel(key)) {
		switch(toupper(key)) {
		case '+':
			g_global.camera->move(0, 0, 1);
			break;
		case '-':
			g_global.camera->move(0, 0, -1);
			break;
		case '8':
			g_global.camera->rotate(-M_PI / 12, X_AXIS);
			break;
		case '2':
			g_global.camera->rotate(M_PI / 12, X_AXIS);
			break;
		case '1':
			g_global.camera->resetRotation();
			// camera->resetPosition();
			// camera->rotate (-M_PI/2, X_AXIS);
			// camera->move (0,0,-75);
			break;
		case '7':
			g_global.camera->resetRotation();
			g_global.camera->rotate(M_PI / 2, X_AXIS);
			break;
		case '6':
			g_global.camera->rotate(-M_PI / 12, Y_AXIS);
			break;
		case '5':
			g_global.camera->setPerspective(!g_global.camera->isPerspective());
			reshape(mainWindow.getSize().getWidth(), mainWindow.getSize().getHeight());
			break;
		case '4':
			g_global.camera->rotate(M_PI / 12, Y_AXIS);
			break;
		case '3':
			g_global.camera->resetRotation();
			g_global.camera->rotate(-M_PI / 2, Y_AXIS);
			break;
		case '.':
			g_global.camera->resetPosition();
			g_global.camera->move(0, 0, -125);
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
	Window & mainWindow(*g_mainWindow);

	// cout << "mouse: " << button << endl;
	g_global.camera->mouseRotateStart(x, y);

	if(!mainWindow.isMouseClickPanel(Point(x, y), button, state)) {
		switch(button) {
		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) {
				right_button_down = true;
			} else {
				right_button_down = false;
			}
			break;

#ifdef GLUT_WHEEL_UP
		case GLUT_WHEEL_UP: // 3
			g_global.camera->move(0, 0, 1);
			if(!g_global.camera->isPerspective()) {
				reshape(mainWindow.getSize().getWidth(),
				        mainWindow.getSize().getHeight());
			}
			break;
#endif // GLUT_WHEEL_UP

#ifdef GLUT_WHEEL_DOWN
		case GLUT_WHEEL_DOWN: // 4
			g_global.camera->move(0, 0, -1);
			if(!g_global.camera->isPerspective()) {
				reshape(mainWindow.getSize().getWidth(),
				        mainWindow.getSize().getHeight());
			}
			break;
#endif // GLUT_WHEEL_DOWN

#ifdef GLUT_WHEEL_RIGHT
		case GLUT_WHEEL_RIGHT: // 5
			break;
#endif // GLUT_WHEEL_RIGHT

#ifdef GLUT_WHEEL_LEFT
		case GLUT_WHEEL_LEFT: // 6
			break;
#endif // GLUT_WHEEL_LEFT
		}
	}
	// mainWindow.isMouseClickPanel  (Point(x, y), button, state);

	glutPostRedisplay();
}

static void activeMotion(int x, int y)
{
	Window & mainWindow(*g_mainWindow);
	if(!mainWindow.isMouseDraggedPanel(Point(x, y))) {
		if(right_button_down) {
			g_global.camera->moveMouse(x, y);
		} else {
			g_global.camera->rotateMouse(x, y);
		}
		glutPostRedisplay();
	}
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	fs::path configFilePath = "my-humanoid.config.json";

	if(fs::exists(configFilePath)) {
		std::ifstream i(configFilePath);
		i >> g_jsonConfig;
	} else {
		g_jsonConfig["mainWindow"]["pos"]  = {0, 0};
		g_jsonConfig["mainWindow"]["size"] = {800, 600};
	}

	int mainWinPosX = g_jsonConfig["mainWindow"]["pos"][0];
	int mainWinPosY = g_jsonConfig["mainWindow"]["pos"][1];

	int mainWinSizeX = g_jsonConfig["mainWindow"]["size"][0]; // glutGet(GLUT_SCREEN_WIDTH);
	int mainWinSizeY = g_jsonConfig["mainWindow"]["size"][1]; // glutGet(GLUT_SCREEN_HEIGHT);

	{
#define FOO(XX)                                                                                    \
	do {                                                                                       \
		g_displayWin.XX = g_jsonConfig["windowsOpen"][#XX];                                \
	} while(false)
		FOO(characterSettings);
		FOO(morphTargets);
		FOO(morphTargetsApplied);
		FOO(poseTargets);
		FOO(poseTargetsApplied);
#undef FOO
	}

	// FIXME WTF why does it move ? window decoration ?
	mainWinPosX -= 2;
	mainWinPosY -= 21;

	Rect mainWinRect = Rect(mainWinPosX, mainWinPosY, mainWinSizeX, mainWinSizeY);


	std::string winTitle = mh_app_name + " "s + mh_version;
	g_mainWindow         = new mhgui::Window(mainWinRect, winTitle);

	tooltipPanel = new TooltipPanel(g_mainWindow->getSize().getHeight());
	toolbarPanel = new ToolbarPanel();
	// g_global.mesh     = new Mesh();
	g_global.camera   = new Camera();
	g_global.autozoom = new Autozoom();

	characterSettingPanel = new CharacterSettingPanel();
	g_mainWindow->initWindow();

	g_bodyShader       = LoadShader("shader/body.vert", "shader/body.frag");
	g_backgroundShader = LoadShader("shader/background.vert", "shader/background.frag");

	bool mesh_loaded =
	        g_mesh.loadMesh(searchDataFile("base.vertices"), searchDataFile("base.faces"));
	if(!mesh_loaded) {
		cerr << "couldn't load mesh geometry" << endl;
		return 1;
	}

	bool material_loaded = g_mesh.loadMaterial(searchDataFile("base.materials"),
	                                           searchDataFile("base.colors"));
	if(!material_loaded) {
		cerr << "couldn't load mesh material informations" << endl;
		return 1;
	}

	g_mesh.loadTextureVector(searchDataFile("base.uv"));

	// load face groups with factory function
	bool groups_loaded = g_mesh.loadGroups(searchDataFile("base.parts"));
	if(!groups_loaded) {
		cerr << "couldn't load face groups" << endl;
		return 1;
	}

	bool smooth_loaded = g_mesh.loadSmoothVertex(searchDataFile("base.smooth"));
	if(!smooth_loaded) {
		cerr << "couldn't load smooth info" << endl;
		return 1;
	}

	CreateTargetImageTextures();
	CreatePoseImageTextures();
	CreateCaractersIconTextures();

	loadTextures();
	g_global.m_enableTexture = true;

	g_mesh.loadTargets(searchDataDir("targets"));
	g_mesh.loadTargets(searchDataDir("selectors"), 1, false);
	g_mesh.loadPoseTargets(searchDataDir("rotations"));
	g_mesh.loadCharacters(searchDataDir("bs_data"));

	init = false;
	g_mainWindow->setCamera(g_global.camera);

	// Add panels to mainwindow
	g_mainWindow->addPanel(tooltipPanel);
	g_mainWindow->addPanel(toolbarPanel);
	g_mainWindow->addPanel(characterSettingPanel);

	// camera->rotate (-M_PI/2, X_AXIS);
	g_global.camera->move(0, 0, -125.0f);

	tooltipPanel->createWidgets();
	toolbarPanel->createWidgets();
	characterSettingPanel->createWidgets();
	toolbarPanel->show_all();
	tooltipPanel->show_all();
	characterSettingPanel->show_all();

	glutDisplayFunc(display);
	glutTimerFunc(1000, timerTrigger, 1); // Autozoom
	glutCloseFunc([]() -> void {
		// TODO glut does not let us prevent closing in a sane way
		// just let it happen for now :/

		//		g_userRequestedQuit = true;
		//		Window &mainWindow(*g_mainWindow);

		// TODO WHY?
		// mainWindow.mainLoop();
	});

	g_mainWindow->show();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

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
	ImGui_ImplGLUT_Init();

	{
		glutReshapeFunc([](int w, int h) -> void {
			ImGuiIO & io   = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)w, (float)h);

			reshape(w, h);
		});

		glutMotionFunc([](int x, int y) -> void {
			ImGuiIO & io = ImGui::GetIO();
			io.MousePos  = ImVec2((float)x, (float)y);

			if(!io.WantCaptureMouse) {
				activeMotion(x, y);
			}
		});

		glutPassiveMotionFunc([](int x, int y) -> void {
			ImGuiIO & io = ImGui::GetIO();
			io.MousePos  = ImVec2((float)x, (float)y);

			if(!io.WantCaptureMouse) {
				motion(x, y);
			}
		});

		glutMouseFunc([](int glut_button, int state, int x, int y) -> void {
			ImGui_ImplGLUT_MouseFunc(glut_button, state, x, y);

			ImGuiIO & io = ImGui::GetIO();
			if(!io.WantCaptureMouse) {
				mouse(glut_button, state, x, y);
			}
		});

		glutMouseWheelFunc(ImGui_ImplGLUT_MouseWheelFunc);

		glutKeyboardFunc([](unsigned char c, int x, int y) -> void {
			ImGui_ImplGLUT_KeyboardFunc(c, x, y);

			ImGuiIO & io = ImGui::GetIO();
			if(!io.WantCaptureKeyboard) {
				keyboard(c);
			}
		});

		glutKeyboardUpFunc(ImGui_ImplGLUT_KeyboardUpFunc);

		glutSpecialFunc([](int key, int x, int y) -> void {
			ImGui_ImplGLUT_SpecialFunc(key, x, y);

			ImGuiIO & io = ImGui::GetIO();
			if(!io.WantCaptureKeyboard) {
				special(key);
			}
		});

		glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
	}

	ImGui_ImplOpenGL3_Init();

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

	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	g_jsonConfig["mainWindow"]["pos"]  = {g_mainWinRect.pos.x, g_mainWinRect.pos.y};
	g_jsonConfig["mainWindow"]["size"] = {g_mainWinRect.size.x, g_mainWinRect.size.y};

	{
		auto & windows                 = g_jsonConfig["windowsOpen"];
		windows["characterSettings"]   = g_displayWin.characterSettings;
		windows["morphTargets"]        = g_displayWin.morphTargets;
		windows["morphTargetsApplied"] = g_displayWin.morphTargetsApplied;
		windows["poseTargets"]         = g_displayWin.poseTargets;
		windows["poseTargetsApplied"]  = g_displayWin.poseTargetsApplied;
	}

	{
		std::ofstream o(configFilePath);
		o << std::setw(4) << g_jsonConfig << std::endl;
	}

	return 0;
}

// function to find the xyz values of the bounding box
void calcMinMax(const glm::vec3 & coords)
{
	if(coords.x < twopoints[0]) {
		twopoints[0] = coords.x;
	} else if(coords.x > twopoints[3]) {
		twopoints[3] = coords.x;
	}

	if(coords.y < twopoints[1]) {
		twopoints[1] = coords.y;
	} else if(coords.y > twopoints[4]) {
		twopoints[4] = coords.y;
	}

	if(coords.z < twopoints[2]) {
		twopoints[2] = coords.z;
	} else if(coords.z > twopoints[5]) {
		twopoints[5] = coords.z;
	}
}

void loadTextures()
{
	for(auto & [name, value] : g_mesh.facegroup().m_groups) {

		std::string fileName = "pixmaps/ui/" + name + "_color.png";
		value.texture        = LoadTextureFromFile(fileName.c_str());
		if(!value.texture) {
			cerr << "couldn't load base skin_color Texture Data " << name
			     << "_color.png" << endl;
		}


		{
			std::string dir = "data/rib_data/textures_data/";
			value.specular  = LoadTextureFromFile(dir + name + "_specular.png");
			value.bump      = LoadTextureFromFile(dir + name + "_bump.png");
		}
	}
}

void drawBackground()
{
	auto inSize = g_mainWindow->getSize();

	glUseProgram(g_backgroundShader->handle);
	GLint myLoc = glGetUniformLocation(g_backgroundShader->handle, "u_viewportResolution");
	if(myLoc != -1) {
		glProgramUniform2f(g_backgroundShader->handle, myLoc, inSize.getWidth(),
		                   inSize.getHeight());
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	// bottom
	glVertex2f(-1.0, -1.0);
	glVertex2f(1.0, -1.0);
	// top
	glVertex2f(1.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glUseProgram(0);
}

void renderMesh()
{
	const MaterialVector & materialvector(g_mesh.materials());
	const TextureVector &  texturevector(g_mesh.textureVector());

	const FaceVector & facevector(g_mesh.faces());

	const VertexVector & vertexvector(g_mesh.getVertexVectorRef());

	int facesize;
	int istri = -1; // to remember which type was the latest drawn geometry and
	                // avoid too many glBegin

	if(g_global.quotedBox) {
		for(int i = 0; i < 6; i++) {
			twopoints[i] = 0;
		}
	}

	cgutils::enableBlend();

	glUseProgram(g_bodyShader->handle);

	for(auto & [goupName, groupValue] : g_mesh.facegroup().m_groups) {

		if(groupValue.visible == false)
			continue;

		if(g_global.m_enableTexture) {
			if(groupValue.texture) {
				glActiveTexture(GL_TEXTURE0);
				::glBindTexture(GL_TEXTURE_2D, groupValue.texture.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture0"),
				            0);
			}
			if(groupValue.specular) {
				glActiveTexture(GL_TEXTURE1);
				::glBindTexture(GL_TEXTURE_2D, groupValue.specular.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture1"),
				            1);
			}
			if(groupValue.bump) {
				glActiveTexture(GL_TEXTURE2);
				::glBindTexture(GL_TEXTURE_2D, groupValue.bump.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture2"),
				            2);
			}
		}

		for(const auto & faceIndex : groupValue.facesIndexes) {
			const Face &        face         = facevector[faceIndex];
			const TextureFace & texture_face = texturevector[faceIndex];

			facesize = face.getSize();

			if(istri != facesize) {
				if(istri != -1) {
					::glEnd();
				}

				::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
				istri = facesize;
			}

			{
				unsigned int facesize = face.getSize();

				size_t j;

				int material_index = face.getMaterialIndex();
				if(material_index != -1) {
					const Material & material(materialvector[material_index]);
					const Color &    color(material.getRGBCol());

					// Set the color for these vertices
					::glColor4fv(color.getAsOpenGLVector());
				}

				for(j = 0; j != facesize; ++j) {
					const Vertex & vertex =
					        vertexvector[face.getVertexAtIndex(j)];
					const glm::vec2 & uv = texture_face[j];

					::glNormal3fv(glm::value_ptr(vertex.no));
					::glTexCoord2f(uv.x, uv.y);
					::glVertex3fv(glm::value_ptr(vertex.co));

					if(g_global.quotedBox) {
						calcMinMax(vertex.co);
					}
				}
			}
		}

		::glEnd();
		istri = -1;
	}

	glUseProgram(0);

	//	glColor3ub( 255, 255, 255 );
	//	for(const auto & strip: mesh->edgestrip) {
	//		glBegin(GL_LINE_STRIP);
	//		for(const auto & index: strip) {
	//			const Vertex &vertex = vertexvector[index];
	//			::glVertex3fv(glm::value_ptr(vertex.co));
	//		}
	//		glEnd();
	//	}

	glDisable(GL_BLEND);
}
