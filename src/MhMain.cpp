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

#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

#include "GlInclude.h"
#include <GL/freeglut.h>

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
#include <gui/GLUTWrapper.h>
#include <gui/Image.h>
#include <gui/Panel.h>
#include <gui/Size.h>
#include <gui/Widget.h>
#include <gui/Window.h>

#include "log/log.h"
#include "log/Profiler.h"

#include "render/RenderUtils.h"
#include "render/Shader.h"

#include "util/StringUtils.h"

#include <time.h>

#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include "util.h"

#include "MhConfig.h"
#include "MhRender.h"
#include "Vfs.h"

#define kTimerRendering 1000


static constexpr char mh_app_name[] = "MyHumanoid";
static constexpr char mh_version[]  = "0.1.0";

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
	loadTexturesFromDir(g_charactersIconTextures, "pixmaps/bsimg/");
}

// ================================================================================================

static void saveBodySettings(const string & filename)
{
	Animorph::BodySettings bodyset = g_mesh.morphTargets();
	
	bodyset.m_comp = g_global.m_comp;
	
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
		
		g_global.m_comp = bodyset.m_comp;
		
		mhgui::g_mainWindow->characterSettingPanel->updateUi();
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
	mhgui::g_mainWindow->characterSettingPanel->resetSlidersValues();
	
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
		
		g_global.m_comp = bodyset.m_comp;
		
		CharacterSettingPanel * tmpPanel = mhgui::g_mainWindow->characterSettingPanel;
		
		tmpPanel->updateUi();
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

void calcBoundingBox()
{
	if(g_global.quotedBox) {
		for(int i = 0; i < 6; i++) {
			twopoints[i] = 0;
		}
		
		for(const auto & vertex : g_mesh.getVertexVectorRef()) {
			calcMinMax(vertex.pos);
		}
	}
}


// Display function
static void display()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	g_global.camera->applyMatrix();
	
	calcBoundingBox();
	renderMesh();

	if(g_displayAxis) {
		cgutils::drawAxis();
	}
	
	if(g_global.quotedBox) {
		RenderBoundingBox(twopoints);
	}

	drawBackground();
	mhgui::g_mainWindow->draw();

	if(g_global.drawGrid) {
		cgutils::drawGrid(mhgui::g_mainWindow->getSize(), 220, 70, grid_color, border_color, 50);
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
		
		LoadBodyShader(g_requestShaderVersion);
	}
	if(g_requestBackgroundShaderReload) {
		g_requestBackgroundShaderReload = false;
		
		LoadBackgroundShader();
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
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);
	mainWindow.reshape(Size(w, h), *g_global.camera);
	g_global.camera->reshape(w, h);
}

static void timerTrigger(int val)
{
	(void)val;

	bool     tmp;
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);

	tmp = g_global.camera->timerTrigger();

	if(!g_global.camera->isPerspective()) {
		reshape(mainWindow.getSize().getWidth(), mainWindow.getSize().getHeight());
	}
	glutTimerFunc(50, timerTrigger, 1);
}

static void motion(int x, int y)
{
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);
	mainWindow.isMouseOverPanel(glm::ivec2(x, y));
}

static void special(int key)
{
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);
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
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);
	if(!mainWindow.isKeyTypePanel(key)) {
		switch(toupper(key)) {
		case '+':
			g_global.camera->move(0, 0, 1);
			break;
		case '-':
			g_global.camera->move(0, 0, -1);
			break;
		case '8':
			g_global.camera->rotate(-glm::pi<float>() / 12, Animorph::X_AXIS);
			break;
		case '2':
			g_global.camera->rotate(glm::pi<float>() / 12, Animorph::X_AXIS);
			break;
		case '1':
			g_global.camera->resetRotation();
			// camera->resetPosition();
			// camera->rotate (-M_PI/2, X_AXIS);
			// camera->move (0,0,-75);
			break;
		case '7':
			g_global.camera->resetRotation();
			g_global.camera->rotate(glm::pi<float>() / 2, Animorph::X_AXIS);
			break;
		case '6':
			g_global.camera->rotate(-glm::pi<float>() / 12, Animorph::Y_AXIS);
			break;
		case '5':
			g_global.camera->setPerspective(!g_global.camera->isPerspective());
			reshape(mainWindow.getSize().getWidth(), mainWindow.getSize().getHeight());
			break;
		case '4':
			g_global.camera->rotate(glm::pi<float>() / 12, Animorph::Y_AXIS);
			break;
		case '3':
			g_global.camera->resetRotation();
			g_global.camera->rotate(-glm::pi<float>() / 2, Animorph::Y_AXIS);
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
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);

	// cout << "mouse: " << button << endl;
	g_global.camera->mouseRotateStart(x, y);

	if(!mainWindow.isMouseClickPanel(glm::ivec2(x, y), button, state)) {
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
	mhgui::Window & mainWindow(*mhgui::g_mainWindow);
	if(!mainWindow.isMouseDraggedPanel(glm::ivec2(x, y))) {
		if(right_button_down) {
			g_global.camera->moveMouse(x, y);
		} else {
			g_global.camera->rotateMouse(x, y);
		}
		glutPostRedisplay();
	}
}

#include "physfs.h"

int main(int argc, char ** argv)
{
	PHYSFS_init(argv[0]);
	vfs::init();
	
	glutInit(&argc, argv);
	
	LoadConfig();
	
	// FIXME WTF why does it move ? window decoration ?
	g_config.windowMain.pos -= glm::ivec2(2, 21);

	Rect mainWinRect = Rect(g_config.windowMain.pos.x, g_config.windowMain.pos.y,
	                        g_config.windowMain.siz.x, g_config.windowMain.siz.y);
	
	std::string winTitle = mh_app_name + std::string(" ") + mh_version;
	mhgui::g_mainWindow         = new mhgui::Window(mainWinRect, winTitle);

	// g_global.mesh     = new Mesh();
	g_global.camera   = new mhgui::Camera();
	g_global.autozoom = new mhgui::Autozoom();
	
	
	mhgui::g_mainWindow->characterSettingPanel = new CharacterSettingPanel();
	mhgui::g_mainWindow->initWindow();
	
	LoadBodyShader(0);
	LoadBackgroundShader();
	
	bool mesh_loaded =
	        g_mesh.loadMesh(searchDataFile("base.vertices"), searchDataFile("base.faces"));
	if(!mesh_loaded) {
		cerr << "couldn't load mesh geometry" << endl;
		return 1;
	}

	bool material_loaded = g_mesh.loadMaterial(searchDataFile("base.materials"),
	                                           searchDataFile("base.colors"));
	if(!material_loaded) {
		log_error("couldn't load mesh material informations");
		return 1;
	}

	g_mesh.loadTextureVector(searchDataFile("base.uv"));

	// load face groups with factory function
	bool groups_loaded = g_mesh.loadGroups(searchDataFile("base.parts"));
	if(!groups_loaded) {
		log_error("couldn't load face groups");
		return 1;
	}

	bool smooth_loaded = g_mesh.loadSmoothVertex(searchDataFile("base.smooth"));
	if(!smooth_loaded) {
		log_error("couldn't load smooth info");
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
	mhgui::g_mainWindow->setCamera(g_global.camera);
	mhgui::g_mainWindow->addPanel(mhgui::g_mainWindow->characterSettingPanel);
	mhgui::g_mainWindow->characterSettingPanel->createWidgets();
	mhgui::g_mainWindow->characterSettingPanel->show_all();
	
	// camera->rotate (-glm::pi<float>()/2, X_AXIS);
	g_global.camera->move(0, 0, -125.0f);
	
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

	mhgui::g_mainWindow->show();
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
	
	g_config.windowMain.pos = g_mainWinRect.pos;
	g_config.windowMain.siz = g_mainWinRect.size;
	SaveConfig();
	
	vfs::deinit();
	
	return 0;
}
