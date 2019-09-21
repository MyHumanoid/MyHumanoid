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

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <imgui.h>
#include <examples/imgui_impl_glut.h>
#include <examples/imgui_impl_opengl2.h>

// mhh...maybe some of these includes are retundant
#include <animorph/Mesh.h>
#include <animorph/Vector3.h>
#include <animorph/util.h>
#include <gui/CGUtilities.h>
#include <gui/Camera.h>
#include <gui/Console.h>
#include <gui/GLUTWrapper.h>
#include <gui/Image.h>
#include <gui/ImageSlider.h>
#include <gui/Panel.h>
#include <gui/Point.h>
#include <gui/Size.h>
#include <gui/Widget.h>
#include <gui/Window.h>

#include "log/log.h"

#include <time.h>
#ifdef DEBUG
#include <StopClock/StopClock.h>
#endif

#include "Animation.h"
#include "BottomPanel.h"
#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "ConsoleCommands.h"
#include "ConsoleListener.h"
#include "FooterPanel.h"
#include "Global.h"
#include "SplashPanel.h"
#include "TargetPanel.h"
#include "ToolbarPanel.h"
#include "TooltipPanel.h"
#include "ViewPanel.h"
#include "util.h"

#define kTimerRendering 1000

using namespace std;
using namespace Animorph;

static void renderMesh();
static void renderMeshEdges();
static bool loadTextures();
static void toggleVisiblePart(const std::string &name);
static void toggleSubdivision();
static void renderSubsurf();

// console listener
static ConsoleListener *consoleListener;
static TooltipPanel *tooltipPanel;
static ToolbarPanel *toolbarPanel;
static FooterPanel *footerPanel;
static ViewPanel *viewPanel;
static BottomPanel *bottomPanel;
static SplashPanel *splashPanel;
static Console *console;
static Mesh *mesh;
static Camera *camera;
// static Texture               *headTexture;
// static Texture               *bodyTexture;
static Animation *animation;
static Autozoom *autozoom;

static CharacterSettingPanel *characterSettingPanel;

bool init; // shows the init status
int splashMotionCount;
float twopoints[6];
int average = 0;
int n_display = 0;
bool right_button_down = false;
int tickCount = 0;
float kTimePerRaster(0.03f);
bool waitDisplay = false;
bool oldCameraTimerTrigger = false;

static Vector3f cameraPos;

const Color border_color(1.0, 0.55, 0.0, 0.8);
const Color grid_color(0.35, 0.50, 0.30, 0.50);
const Color edges_color(0.4, 0.3, 0.3, 0.5);





static void saveBodySettings(const string &filename)
{
	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);
	
	BodySettings bodyset = mesh->getBodySettings();
	FaceGroup &clothesgroup(mesh->getClothesGroupRef());
	
	bool state = bodyset.save(filename);
	
	if (state) {
		state = clothesgroup.saveVisibilities(filename);
	}
	
	if (state) {
		state = saveSelectorsPositions(filename);
	}
	
	if (state) {
		log("BodySettings saved");
	} else {
		log_err("Error: couldn't save file.");
	}
}

static void loadBodySettings(const string &filename)
{
	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);
	Window &mainWindow = *g_mainWindow;
	
	FaceGroup &clothesgroup(mesh->getClothesGroupRef());
	
	BodySettings bodyset;
	bool state = bodyset.load(filename);
	if (state) {
		state = clothesgroup.loadVisibilities(filename);
	}
	
	if (state) {
		global.resetFuzzyValues();
		state = loadSelectorsPositions(filename);
		
		CharacterSettingPanel *tmpPanel =
		    (CharacterSettingPanel *)mainWindow.getPanel(
		        kComponentID_CharacterSettingPanel);
		if (tmpPanel != NULL) {
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Age);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Breast);
			tmpPanel->calcSelectorValues(
			    kComponentID_CharacterSettingPanel_MuscleSize);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Shape);
		}
	}
	
	if (state) {
		mesh->doMorph(bodyset);
		mesh->calcNormals();
		if (global.getSubdivision()) {
			mesh->calcSubsurf();
		}
		
		log("BodySettings loaded");
	} else {
		log("BodySettings load failed");
	}
}



static void savePoses(const string &filename)
{
	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);
	
	BodySettings poses = mesh->getPoses();
	
	bool state = poses.save(filename);
	
	if (state) {
		log("Poses saved");
	} else {
		log_err("Save pose failed");
	}
}

static void loadPoses(const string &filename)
{
	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);
	
	BodySettings poses;
	bool state = poses.load(filename);
	
	if (state) {
		mesh->doPose(poses);
		if (global.getSubdivision()) {
			mesh->calcSubsurf();
		}
		log("Poses loaded");
	} else {
		log_err("Poses load failed");
	}
}




void loadAnimation(const string &path)
{
	Global &global = Global::instance();
	Animation *animation = global.getAnimation();
	assert(animation);
	
	bool state = animation->load(path);
	if (state) {
		log("Animation loaded");
	} else {
		log_err("Animation load failed");
	}
}










// Our state
static bool show_demo_window = false;

static bool g_userRequestedQuit = false;

static bool g_displayCharacterSettings = false;
static bool g_displayPerformance = false;

void DisplayQuitPopup() {
	if(ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you really want to quit? (press ENTER to quit)\n");
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
		if (ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("SaveBodysettings")) {
				auto & global = Global::instance();
				
				if (global.getAppMode() == BODY_DETAILS ||
				    global.getAppMode() == CHARACTER_SETTING ||
				    global.getAppMode() == CLOTHES) {
					
					
					saveBodySettings("foo-BodySettings");
					
//					mainWindow.getConsole()->openWithCommand(
//					    kConsoleCommand_Save_Bodysettings,
//					    kConsoleMessage_Save_Bodysettings, getMyBodysettingsPath());
				} else if (global.getAppMode() == POSES) {
					
					savePoses("foo-Poses");
					
//					mainWindow.getConsole()->openWithCommand(kConsoleCommand_Save_Poses,
//					                                         kConsoleMessage_Save_Poses,
//					                                         getMyPosesPath());
				} else if (global.getAppMode() == ANIMATIONS) {
					// mainWindow.getConsole()->openWithCommand(kConsoleCommand_Save_Animations,
					// kConsoleMessage_Save_Animations, getMyPosesPath());
				}
			}
			if(ImGui::MenuItem("Load bodysetting", "Ctrl+O")) {
				
				auto & global = Global::instance();
				
				if (global.getAppMode() == BODY_DETAILS ||
				    global.getAppMode() == CHARACTER_SETTING ||
				    global.getAppMode() == CLOTHES) {
					
					loadBodySettings("foo-BodySettings");
				} else if (global.getAppMode() == POSES) {
					
					loadPoses("foo-Poses");
				} else if (global.getAppMode() == ANIMATIONS) {

					loadAnimation("foo-Animation");
				}
			}
			if(ImGui::MenuItem("Save As..")) {
				
			}
			ImGui::Separator();
			if(ImGui::Button("Quit...")) {
				ImGui::OpenPopup("Quit?");
			}
			DisplayQuitPopup();
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("View")) {
			ImGui::Checkbox("Grid", &Global::instance().drawGrid);
			if(ImGui::Checkbox("Subsurfaces", &Global::instance().subdivision)) {
				if(Global::instance().subdivision) {
					Mesh *mesh = Global::instance().getMesh();
					assert(mesh);
					mesh->calcSubsurf();
				}
			}
			ImGui::Checkbox("Quoted box", &Global::instance().quotedBox);
			ImGui::Checkbox("Texture", &Global::instance().m_enableTexture);
			
			ImGui::Separator();
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Edit")) {
			ImGui::Checkbox("CharacterSettings", &g_displayCharacterSettings);
			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if(ImGui::BeginMenu("Help")) {
			ImGui::Checkbox("Performance", &g_displayPerformance);
			ImGui::Separator();
			if(ImGui::Button("About")) {
				SplashPanel *splashScreen =
				    (SplashPanel *)g_mainWindow->getPanel(kComponentID_SplashPanel);
				
				if (splashScreen == NULL) {
					Global::instance().setDrawGrid(false);
					splashScreen = new SplashPanel(g_mainWindow->getSize());
					g_mainWindow->addPanel(splashScreen);
					splashScreen->createWidgets();
					splashScreen->show_all();
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}




void DisplayCharacterSettings()
{
	if(!g_displayCharacterSettings) {
		return;
	}
	
	Global &global = Global::instance();

	static std::array<float, 2> ageAndSex = {0.f, 0.f};
	static std::array<float, 2> bodyWeightMuscle = {0.f, 0.f};
	static std::array<float, 2> breastSizeShape = {0.f, 0.f};
	static std::array<float, 2> bodyShapeHeight = {0.f, 0.f};

	ImGui::Begin("Character Setting");

	if (ImGui::Button("Reset")) {
		ageAndSex = {0.f, 0.f};
		bodyWeightMuscle = {0.f, 0.f};
		breastSizeShape = {0.f, 0.f};
		bodyShapeHeight = {0.f, 0.f};

		characterSettingPanel->selectorListener.ageDists.clear();
		characterSettingPanel->selectorListener.muscleSizeDists.clear();
		characterSettingPanel->selectorListener.breastDists.clear();
		characterSettingPanel->selectorListener.shapeDists.clear();

		Mesh *mesh = global.getMesh();
		assert(mesh);
		mesh->resetMorph();
		loadDefaultBodySettings();
		Global::instance().resetFuzzyValues();
	}

	if (ImGui::SliderFloat2("Age/Sex", ageAndSex.data(), 0.0f, 1.0f)) {

		Point p(ageAndSex[0] * 100, ageAndSex[1] * 100);
		characterSettingPanel->m_age->cursorPos = p;

		characterSettingPanel->selectorListener.ageDists =
		    characterSettingPanel->m_age->getDists();

		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	}

	if (ImGui::SliderFloat2("Weight/Muscle", bodyWeightMuscle.data(), 0.0f,
	                        1.0f)) {

		Point p(bodyWeightMuscle[0] * 100, bodyWeightMuscle[1] * 100);
		characterSettingPanel->m_muscleSize->cursorPos = p;

		characterSettingPanel->selectorListener.muscleSizeDists =
		    characterSettingPanel->m_muscleSize->getDists();

		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	}

	if (ImGui::SliderFloat2("Breast Size / Shape", breastSizeShape.data(), 0.0f,
	                        1.0f)) {

		Point p(breastSizeShape[0] * 100, breastSizeShape[1] * 100);
		characterSettingPanel->m_breast->cursorPos = p;

		characterSettingPanel->selectorListener.breastDists =
		    characterSettingPanel->m_breast->getDists();

		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	}

	if (ImGui::SliderFloat2("Body Shape/Height", bodyShapeHeight.data(), 0.0f,
	                        1.0f)) {

		Point p(bodyShapeHeight[0] * 100, bodyShapeHeight[1] * 100);
		characterSettingPanel->m_shape->cursorPos = p;

		characterSettingPanel->selectorListener.shapeDists =
		    characterSettingPanel->m_shape->getDists();

		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	}

	ImGui::End();
}



void DisplayPerformance()
{
	if(!g_displayPerformance) {
		return;
	}
	
	ImGui::Begin("Performance");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
	            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Demo Window", &show_demo_window);
	ImGui::End();
}








int g_mainWindowPosX;
int g_mainWindowPosY;

// Display function
static void display()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	camera->applyMatrix();

	// cgutils::drawAxis ();
	if (Global::instance().getSubdivision() &&
	    !Global::instance().getLightMesh()) {
		renderSubsurf();
	} else {
		renderMesh();
	}

	/*
	  if(Global::instance().getFlatShading())
	    renderMeshEdges();
	*/

	if (Global::instance().getQuotedBox())
		cgutils::mhWireCube(twopoints);

	Window &mainWindow(*g_mainWindow);
	mainWindow.draw();

	if (Global::instance().getDrawGrid()) {
		cgutils::drawGrid(mainWindow.getSize(), 220, 70, grid_color, border_color,
		                  50);
	} else {
		cgutils::drawSquare(Rect(0, 0, 1, 1), border_color);
	}

	// cgutils::swapBuffers();

	// TODO: put this in a own function
	//  if (!init)
	//  {
	//	  //mesh->loadTargetsFactory (searchDataDir ("targets"));
	//	  init = true;
	//  }

	if (Global::instance().isRendering()) {
		waitDisplay = true;
	}

	{
		DisplayMainMenu();
		DisplayPerformance();
		DisplayCharacterSettings();
		DisplayQuitPopup();

		if (show_demo_window) {
			ImGui::ShowDemoWindow(&show_demo_window);
		}
	}

	// Rendering
	ImGui::Render();
	// ImGuiIO& io = ImGui::GetIO();
	// glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	// glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	// glClear(GL_COLOR_BUFFER_BIT);
	// glUseProgram(0); // You may want this if using this code in an OpenGL 3+
	// context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
	
	// TODO this is a hack
	g_mainWindowPosX = glutGet(GLUT_WINDOW_X);
	g_mainWindowPosY = glutGet(GLUT_WINDOW_Y);
	
	
	if(g_userRequestedQuit) {
		glutLeaveMainLoop();
	}
};

static void reshape(int w, int h)
{
	Window &mainWindow(*g_mainWindow);
	mainWindow.reshape(Size(w, h), *camera);
	camera->reshape(w, h);
}

static void timerTrigger(int val)
{
	(void)val;

	bool tmp;
	Window &mainWindow(*g_mainWindow);

	tmp = camera->timerTrigger();
	if (Global::instance().getSubdivision()) {
		if (!tmp && oldCameraTimerTrigger) {
			Global::instance().setLightMesh(false);
			cgutils::redisplay();
		} else if (tmp && !oldCameraTimerTrigger) {
			Global::instance().setLightMesh(true);
		}
		oldCameraTimerTrigger = tmp;
	}

	if (!camera->isPerspective()) {
		reshape(mainWindow.getSize().getWidth(), mainWindow.getSize().getHeight());
	}
	mainWindow.setTimerCallback(50, timerTrigger, 1);
}

static void motion(int x, int y)
{
	Window &mainWindow(*g_mainWindow);
	splashPanel = (SplashPanel *)mainWindow.getPanel(kComponentID_SplashPanel);
	if (splashPanel != NULL && splashMotionCount++ >= 5) {
		mainWindow.removePanel(splashPanel);
		delete splashPanel;
		splashPanel = NULL;

		splashMotionCount = 0;
		cgutils::redisplay();
	}

	mainWindow.isMouseOverPanel(Point(x, y));
}

static void timerRendering(int value)
{
	Window &mainWindow(*g_mainWindow);

	if (Global::instance().isRendering() && waitDisplay) {
		waitDisplay = false;
		renderingStep();
		mainWindow.setTimerCallback(kTimerRendering, timerRendering,
		                            0); // Rendering
	} else {
		mainWindow.setTimerCallback(kTimerRendering, timerRendering,
		                            0); // Rendering
	}
}

static void timer(int value)
{
	Window &mainWindow(*g_mainWindow);

	if (animation->isStarted()) {
		if (tickCount++ >=
		    (int)((1000 / kTimerCallback) / animation->getFrameRate())) {
			animation->nextFrame();
			tickCount = 0;
			cgutils::redisplay();
		}
	}
	mainWindow.setTimerCallback(kTimerCallback, timer, value);
}

static void special(int key)
{
	Window &mainWindow(*g_mainWindow);
	if (!mainWindow.isKeyTypePanel(key)) {
		switch (key) {
		case GLUT_KEY_UP:
			camera->move(0, 1, 0);
			break;
		case GLUT_KEY_DOWN:
			camera->move(0, -1, 0);
			break;
		case GLUT_KEY_LEFT:
			camera->move(-1, 0, 0);
			break;
		case GLUT_KEY_RIGHT:
			camera->move(1, 0, 0);
			break;
		}
	}
	cgutils::redisplay();
}

static void keyboard(unsigned char key)
{
	Window &mainWindow(*g_mainWindow);
	if (!mainWindow.isKeyTypePanel(key)) {
		switch (toupper(key)) {
		case '+':
			camera->move(0, 0, 1);
			break;
		case '-':
			camera->move(0, 0, -1);
			break;
		case '8':
			camera->rotate(-M_PI / 12, X_AXIS);
			break;
		case '2':
			camera->rotate(M_PI / 12, X_AXIS);
			break;
		case '1':
			camera->resetRotation();
			// camera->resetPosition();
			// camera->rotate (-M_PI/2, X_AXIS);
			// camera->move (0,0,-75);
			break;
		case '7':
			camera->resetRotation();
			camera->rotate(M_PI / 2, X_AXIS);
			break;
		case '6':
			camera->rotate(-M_PI / 12, Y_AXIS);
			break;
		case '5':
			camera->setPerspective(!camera->isPerspective());
			reshape(mainWindow.getSize().getWidth(),
			        mainWindow.getSize().getHeight());
			break;
		case '4':
			camera->rotate(M_PI / 12, Y_AXIS);
			break;
		case '3':
			camera->resetRotation();
			camera->rotate(-M_PI / 2, Y_AXIS);
			break;
		case '.':
			camera->resetPosition();
			camera->move(0, 0, -125);
			break;
		case 'O':
			console->open();
			break;
		case 'S':
			toggleSubdivision();
			break;
		case 'T':
			rendering(mainWindow, TOON);
			break;
		case 'H':
			toggleVisiblePart("head");
			break;
		case 'B':
			toggleVisiblePart("body");
			break;
		case 'E':
			toggleVisiblePart("zeyebrows");
			break;
		case 'K': {
			int ret;
			if ((ret = KillProcessList()) == 0) {
				mainWindow.getConsole()->open();
				mainWindow.getConsole()->printMessage("No process killed");
				cgutils::redisplay();
			} else {
				std::stringstream kk;
				kk << ret;
				mainWindow.getConsole()->open();
				mainWindow.getConsole()->printMessage("Killed  " + kk.str() +
				                                      " process ");
				cgutils::redisplay();
			}
		} break;
		default:
			break;
		}
	}
	cgutils::redisplay();
}

static void mouse(int button, int state, int x, int y)
{
	Window &mainWindow(*g_mainWindow);
	splashPanel = (SplashPanel *)mainWindow.getPanel(kComponentID_SplashPanel);
	if (splashPanel != NULL) {
		mainWindow.removePanel(splashPanel);
		delete splashPanel;
		splashPanel = NULL;

		splashMotionCount = 0;
		// cgutils::redisplay();
	}

	if (console->isActive()) {
		console->isMouseClick(Point(x, y), button, state);
		return;
	}

	// cout << "mouse: " << button << endl;
	camera->mouseRotateStart(x, y);

	if (!mainWindow.isMouseClickPanel(Point(x, y), button, state)) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state != GLUT_DOWN) {
				if (Global::instance().getSubdivision()) {
					Global::instance().setLightMesh(false);
				}
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				right_button_down = true;
			} else {
				right_button_down = false;
				if (Global::instance().getSubdivision()) {
					Global::instance().setLightMesh(false);
				}
			}
			break;

#ifdef GLUT_WHEEL_UP
		case GLUT_WHEEL_UP: // 3
			camera->move(0, 0, 1);
			if (!camera->isPerspective()) {
				reshape(mainWindow.getSize().getWidth(),
				        mainWindow.getSize().getHeight());
			}
			break;
#endif // GLUT_WHEEL_UP

#ifdef GLUT_WHEEL_DOWN
		case GLUT_WHEEL_DOWN: // 4
			camera->move(0, 0, -1);
			if (!camera->isPerspective()) {
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

	cgutils::redisplay();
}

static void activeMotion(int x, int y)
{
	if (console->isActive()) {
		console->isMouseDragged(Point(x, y));
		return;
	}

	Window &mainWindow(*g_mainWindow);
	if (!mainWindow.isMouseDraggedPanel(Point(x, y))) {
		if (right_button_down) {
			camera->moveMouse(x, y);
		} else {
			camera->rotateMouse(x, y);
		}

		if (Global::instance().getSubdivision()) {
			Global::instance().setLightMesh(true);
		}

		cgutils::redisplay();
	}
}

#include <experimental/filesystem>
#include <json.hpp>

namespace fs = std::experimental::filesystem;
using json = nlohmann::json;

json g_jsonConfig;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	fs::path configFilePath = "my-humanoid.config.json";
	
	if(fs::exists(configFilePath)) {
		std::ifstream i(configFilePath);
		i >> g_jsonConfig;
	} else {
		g_jsonConfig["mainWindow"]["pos"] = {0, 0};
		g_jsonConfig["mainWindow"]["size"] = {800, 600};
	}
	
	int mainWinPosX  = g_jsonConfig["mainWindow"]["pos"][0];
	int mainWinPosY  = g_jsonConfig["mainWindow"]["pos"][1];
	
	int mainWinSizeX = g_jsonConfig["mainWindow"]["size"][0];// glutGet(GLUT_SCREEN_WIDTH);
	int mainWinSizeY = g_jsonConfig["mainWindow"]["size"][1];// glutGet(GLUT_SCREEN_HEIGHT);
	
	Rect mainWinRect = Rect(mainWinPosX, mainWinPosY, mainWinSizeX, mainWinSizeY);
	
	g_mainWindow = new mhgui::Window(mainWinRect,
	                                 "MakeHuman 0.9.1 RC1", Color(0, 0, 0));
	
	Window &mainWindow(*g_mainWindow);
	
	mainWindow.setPosition(mhgui::Point(
		mainWinPosX,
		mainWinPosY
	));
	
	splashMotionCount = 0;
	createWorkingDirs();

	ParseConfigurationXML();
	ExportConfigurationXML();
	tooltipPanel = new TooltipPanel(mainWindow.getSize().getHeight());
	toolbarPanel = new ToolbarPanel();
	footerPanel = new FooterPanel(mainWindow.getSize().getHeight());
	viewPanel = new ViewPanel(mainWindow.getSize().getHeight());
	bottomPanel = new BottomPanel();
	splashPanel = new SplashPanel(mainWindow.getSize());
	mesh = new Mesh();
	camera = new Camera();
	// headTexture           = new Texture();
	// bodyTexture           = new Texture();
	animation = new Animation();
	autozoom = new Autozoom();

	characterSettingPanel = new CharacterSettingPanel();
	mainWindow.initWindow();

	// TODO: put animorph mesh loading stuff in extra function

	// load mesh with factory function
	bool mesh_loaded = mesh->loadMeshFactory(searchDataFile("base.vertices"),
	                                         searchDataFile("base.faces"));
	if (!mesh_loaded) {
		cerr << "couldn't load mesh geometry" << endl;
		return 1;
	}

	// load material for mesh
	bool material_loaded = mesh->loadMaterialFactory(
	    searchDataFile("base.materials"), searchDataFile("base.colors"));
	if (!material_loaded) {
		cerr << "couldn't load mesh material informations" << endl;
		return 1;
	}

	// load UV texture data, currently no Factory function
	TextureVector &texturevector = mesh->getTextureVectorRef();
	texturevector.load(searchDataFile("base.uv"));

	// load face groups with factory function
	bool groups_loaded = mesh->loadGroupsFactory(searchDataFile("base.parts"));
	if (!groups_loaded) {
		cerr << "couldn't load face groups" << endl;
		return 1;
	}

	// load subdivided face groups with factory function
	bool subd_groups_loaded =
	    mesh->loadSubdGroupsFactory(searchDataFile("subd.parts"));
	if (!subd_groups_loaded) {
		cerr << "couldn't load subdivided face groups" << endl;
		return 1;
	}

	// load skin info with factory function
	bool skin_loaded = mesh->loadSkinFactory(searchDataFile("base.skin"));
	if (!skin_loaded) {
		cerr << "couldn't load skin info" << endl;
		return 1;
	}

	// load skin info with factory function
	bool clothes_loaded =
	    mesh->loadClothesFactory(searchDataFile("base.clothes"));
	if (!clothes_loaded) {
		cerr << "couldn't load clothes info" << endl;
		return 1;
	}

	// load skin info with factory function
	bool edges_loaded = mesh->loadEdgeStripFactory(searchDataFile("base.strips"));
	if (!edges_loaded) {
		cerr << "couldn't load edges info" << endl;
		return 1;
	}

	// load smooth info with factory function
	bool smooth_loaded =
	    mesh->loadSmoothVertexFactory(searchDataFile("base.smooth"));
	if (!smooth_loaded) {
		cerr << "couldn't load smooth info" << endl;
		return 1;
	}

	// load subdivision surfaces info with factory function
	bool subd_loaded = mesh->loadSubdFactory(searchDataFile("base.subde"),
	                                         searchDataFile("base.subdo"),
	                                         searchDataFile("base.subdf"));
	if (!subd_loaded) {
		cerr << "couldn't load subsurf info" << endl;
		return 1;
	}

	// put mesh container into the Global Singleton
	Global::instance().setMesh(mesh);

	// put camera into the Global Singleton
	Global::instance().setCamera(camera);

	// put meshTexture into the Global Singleton
	// Global::instance().setBodyTexture (bodyTexture);
	// Global::instance().setHeadTexture (headTexture);

	// put animation into the Global Singleton
	Global::instance().setAnimation(animation);

	// put autozoom into the Global Singleton
	Global::instance().setAutozoom(autozoom);

	mesh->loadTargetsFactory(searchDataDir("targets"));
	mesh->loadTargetsFactory(searchDataDir("selectors"), 1, true, false);
	mesh->loadPoseTargetsFactory(searchDataDir("rotations"));
	mesh->loadCharactersFactory(searchDataDir("bs_data"));
	// mesh->loadCharactersFactory(searchDataDir ("my"));

	init = false;

	consoleListener = new ConsoleListener();
	// Add console
	console = new Console(FOUR_CHAR_CONST('C', 'O', 'N', '1'));
	console->setListener(consoleListener);

	console->loadPNG(searchPixmapFile("ui/console.png"));
	console->addSplashLine("MakeHuman operating console");
	console->addSplashLine("version 0.1");
	console->addSplashLine("2005-2007");
	console->addSplashLine("(press ESC to exit)");
	console->addSplashLine("- - - - - - - - - - - - - -");
	console->setActive(false);
	mainWindow.setConsole(console);
	mainWindow.setCamera(camera);

	// Add panels to mainwindow
	mainWindow.addPanel(tooltipPanel);
	mainWindow.addPanel(toolbarPanel);
	mainWindow.addPanel(footerPanel);
	mainWindow.addPanel(viewPanel);
	mainWindow.addPanel(bottomPanel);
	mainWindow.addPanel(characterSettingPanel);
	mainWindow.addPanel(splashPanel);

	// set initial camera position
	// camera->rotate (-M_PI/2, X_AXIS);
	camera->move(0, 0, -125.0f);

	// create after adding
	tooltipPanel->createWidgets();
	toolbarPanel->createWidgets();
	footerPanel->createWidgets();
	viewPanel->createWidgets();
	bottomPanel->createWidgets();
	characterSettingPanel->createWidgets();
	splashPanel->createWidgets();

	mainWindow.loadPNG(searchPixmapFile("ui/background.png"));

	// Activate the images textures
	// Note it's after context creation
	toolbarPanel->show_all();
	footerPanel->show_all();
	viewPanel->show_all();
	tooltipPanel->show_all();
	bottomPanel->show_all();
	characterSettingPanel->show_all();
	splashPanel->show_all();

	if (loadTextures()) {
		Global::instance().setTexture(true);
		Global::instance().setCanTexture(true);
	}

	// Glut callbacks
	mainWindow.setDisplayCallback(display);
	mainWindow.setMouseCallback(mouse);
	mainWindow.setTimerCallback(kTimerCallback, timer, 0);           // Animation
	mainWindow.setTimerCallback(1000, timerTrigger, 1);              // Autozoom
	mainWindow.setTimerCallback(kTimerRendering, timerRendering, 0); // Rendering
	mainWindow.setCloseCallback([]()->void{
		// TODO glut does not let us prevent closing in a sane way
		// just let it happen for now :/
		
//		g_userRequestedQuit = true;
//		Window &mainWindow(*g_mainWindow);
//		if (console->isActive()) {
//			console->close();
//		}
//		console->openWithCommand(kConsoleCommand_Exit, kConsoleMessage_ConfirmExit,
//		                         "");
		// TODO WHY?
		// mainWindow.mainLoop();
	});

	console->show();
	mainWindow.show();

	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	::glPolygonOffset(1.0, 1.0);

	loadDefaultBodySettings();
	mesh->calcSubsurf();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
	// Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGLUT_Init();

	{
		glutReshapeFunc([](int w, int h) -> void {
			ImGuiIO &io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)w, (float)h);

			reshape(w, h);
		});

		glutMotionFunc([](int x, int y) -> void {
			ImGuiIO &io = ImGui::GetIO();
			io.MousePos = ImVec2((float)x, (float)y);

			if (!io.WantCaptureMouse) {
				activeMotion(x, y);
			}
		});

		glutPassiveMotionFunc([](int x, int y) -> void {
			ImGuiIO &io = ImGui::GetIO();
			io.MousePos = ImVec2((float)x, (float)y);

			if (!io.WantCaptureMouse) {
				motion(x, y);
			}
		});

		glutMouseFunc([](int glut_button, int state, int x, int y) -> void {
			ImGui_ImplGLUT_MouseFunc(glut_button, state, x, y);

			ImGuiIO &io = ImGui::GetIO();
			if (!io.WantCaptureMouse) {
				mouseCallbackWrapper(glut_button, state, x, y);
			}
		});

		glutMouseWheelFunc(ImGui_ImplGLUT_MouseWheelFunc);

		glutKeyboardFunc([](unsigned char c, int x, int y) -> void {
			ImGui_ImplGLUT_KeyboardFunc(c, x, y);

			ImGuiIO &io = ImGui::GetIO();
			if (!io.WantCaptureKeyboard) {
				keyboard(c);
			}
		});

		glutKeyboardUpFunc(ImGui_ImplGLUT_KeyboardUpFunc);

		glutSpecialFunc([](int key, int x, int y) -> void {
			ImGui_ImplGLUT_SpecialFunc(key, x, y);

			ImGuiIO &io = ImGui::GetIO();
			if (!io.WantCaptureKeyboard) {
				special(key);
			}
		});

		glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
	}

	// ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

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
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
	
	{
		Window &w = *g_mainWindow;

		g_jsonConfig["mainWindow"]["pos"] = {
			g_mainWindowPosX,
			g_mainWindowPosY
		};
		g_jsonConfig["mainWindow"]["size"] = {
			w.getSize().getWidth(),
			w.getSize().getHeight()
		};
	}
	
	{
		std::ofstream o(configFilePath);
		o << std::setw(4) << g_jsonConfig << std::endl;
	}
	
	return 0;
}

// function to find the xyz values of the bounding box
void calcMinMax(const Vector3f &coords)
{
	if (coords.x < twopoints[0]) {
		twopoints[0] = coords.x;
	} else if (coords.x > twopoints[3]) {
		twopoints[3] = coords.x;
	}

	if (coords.y < twopoints[1]) {
		twopoints[1] = coords.y;
	} else if (coords.y > twopoints[4]) {
		twopoints[4] = coords.y;
	}

	if (coords.z < twopoints[2]) {
		twopoints[2] = coords.z;
	} else if (coords.z > twopoints[5]) {
		twopoints[5] = coords.z;
	}
}

void renderMeshEdges()
{
	const EdgeStrip &edgestrip(mesh->getEdgeStripRef());
	const VertexVector &vertexvector(
	    animation->isLoaded() ? animation->getMesh().getVertexVectorRef()
	                          : mesh->getVertexVectorRef());

	EdgeStrip::const_iterator i;

	glColor4f(edges_color.red(), edges_color.green(), edges_color.blue(),
	          edges_color.alpha());
	// the same as above, but ~5fps faster:
	// GLfloat edgescolor[]={0.75, 0.75, 0.75, 1.0};
	// glColor4fv(edgescolor);
	// not used for "consistency of the code"

	cgutils::enableBlend();
	cgutils::enableLineSmoothing();
	glDisable(GL_LIGHTING);

	::glEnable(GL_POLYGON_OFFSET_FILL);

	for (i = edgestrip.begin(); i != edgestrip.end(); ++i) {
		const StripData &stripdata(*i);

		StripData::const_iterator j;

		::glBegin(GL_LINE_STRIP);
		for (j = stripdata.begin(); j != stripdata.end(); ++j) {
			const Vertex &vertex = vertexvector[*j];
			::glVertex3fv(vertex.co.getAsOpenGLVector());
		}
		::glEnd();
	} //   for (i = facevector.begin(); i != facevector.end(); ++i)

	::glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_LIGHTING);
	cgutils::disableLineSmoothing();
	cgutils::disableBlend();
}

void renderFace(const Face &face, const MaterialVector &materialvector,
                const VertexVector &vertexvector,
                const TextureFace &texture_face)
{
	unsigned int facesize = face.getSize();

	size_t j;

	int material_index = face.getMaterialIndex();
	if (material_index != -1) {
		const Material &material(materialvector[material_index]);
		const Color &color(material.getRGBCol());

		// Set the color for these vertices
		::glColor4fv(color.getAsOpenGLVector());
	}

	for (j = 0; j != facesize; ++j) {
		const Vertex &vertex = vertexvector[face.getVertexAtIndex(j)];
		const Vector2f &uv = texture_face[j];
		/*
		    if(Global::instance().getFlatShading())
		    {
		      ::glNormal3fv (face.no.getAsOpenGLVector());
		    }
		    else
		    {
		*/
		::glNormal3fv(vertex.no.getAsOpenGLVector());
		//    }

		if(Global::instance().m_canTexture && Global::instance().getTexture() && !Global::instance().getLightMesh()) {
			::glTexCoord2f(uv.x, uv.y);
		}
		::glVertex3fv(vertex.co.getAsOpenGLVector());

		if (Global::instance().getQuotedBox()) {
			calcMinMax(vertex.co);
		}
	} // for (j = 0; j != facesize; ++j)
}

void renderClothes()
{
	const MaterialVector &materialvector(mesh->getMaterialVectorRef());
	const TextureVector &texturevector(mesh->getTextureVectorRef());

	const FaceVector &facevector(animation->isLoaded()
	                                 ? animation->getMesh().getFaceVectorRef()
	                                 : mesh->getFaceVectorRef());

	const VertexVector &vertexvector(
	    animation->isLoaded() ? animation->getMesh().getVertexVectorRef()
	                          : mesh->getVertexVectorRef());

	FaceGroup &clothesgroup(animation->isLoaded()
	                            ? animation->getMesh().getClothesGroupRef()
	                            : mesh->getClothesGroupRef());

	clothesgroup.calcVertexes(facevector);

	int facesize;
	int istri = -1; // to remember which type was the latest drawn geometry and
	                // avoid too many glBegin

	for (FaceGroup::const_iterator clothesgroup_it = clothesgroup.begin();
	     clothesgroup_it != clothesgroup.end(); clothesgroup_it++) {
		FGroup clothes = (*clothesgroup_it).second;
		if (!clothes.visible)
			continue;

		FGroupData &groupdata = clothes.facesIndexes;
		for (unsigned int i = 0; i < groupdata.size(); i++) {
			const Face &face(facevector[clothes.facesIndexes[i]]);
			const TextureFace &texture_face = texturevector[clothes.facesIndexes[i]];

			facesize = face.getSize();
			if (istri != facesize) {
				if (istri != -1) {
					::glEnd();
				}

				::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
				istri = facesize;
			}
			renderFace(face, materialvector, vertexvector, texture_face);
		} //   for (i = facevector.begin(); i != facevector.end(); ++i)
		::glEnd();
	}
}

void toggleVisiblePart(const std::string &name)
{
	FaceGroup &facegroup(mesh->getFaceGroupRef());
	facegroup.toggleVisible(name);

	FaceGroup &subd_facegroup(mesh->getSubdFaceGroupRef());
	subd_facegroup.toggleVisible(name);
}

void toggleSubdivision()
{
	Global::instance().setSubdivision(!Global::instance().getSubdivision());

	if (Global::instance().getSubdivision()) {
		mesh->calcSubsurf();
	}
}

void renderMesh()
{
	const MaterialVector &materialvector(mesh->getMaterialVectorRef());
	const TextureVector &texturevector(mesh->getTextureVectorRef());

	const FaceVector &facevector(animation->isLoaded()
	                                 ? animation->getMesh().getFaceVectorRef()
	                                 : mesh->getFaceVectorRef());

	const VertexVector &vertexvector(
	    animation->isLoaded() ? animation->getMesh().getVertexVectorRef()
	                          : mesh->getVertexVectorRef());

	FaceGroup &facegroup(mesh->getFaceGroupRef());

	int facesize;
	int istri = -1; // to remember which type was the latest drawn geometry and
	                // avoid too many glBegin

	if (Global::instance().getQuotedBox()) {
		for (int i = 0; i < 6; i++) {
			twopoints[i] = 0;
		}
	}

	if(Global::instance().m_canTexture && Global::instance().getTexture() && !Global::instance().getLightMesh()) {
		::glEnable(GL_TEXTURE_2D);
	}

	cgutils::enableBlend();

	for (FaceGroup::iterator facegroup_it = facegroup.begin();
	     facegroup_it != facegroup.end(); facegroup_it++) {

		if ((*facegroup_it).second.visible == false)
			continue;

		if(Global::instance().m_canTexture && Global::instance().getTexture() && !Global::instance().getLightMesh()) {
			::glBindTexture(GL_TEXTURE_2D, Global::instance()
			                                   .getTexture((*facegroup_it).first)
			                                   ->getTextureIdOfXY(0, 0));
		}

		FGroupData &groupdata = (*facegroup_it).second.facesIndexes;

		for (unsigned int i = 0; i < groupdata.size(); i++) {
			const Face &face(facevector[groupdata[i]]);
			const TextureFace &texture_face = texturevector[groupdata[i]];

			facesize = face.getSize();

			if (istri != facesize) {
				if (istri != -1) {
					::glEnd();
				}

				::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
				istri = facesize;
			}
			renderFace(face, materialvector, vertexvector, texture_face);
		}

		::glEnd();
		istri = -1;
	}

	renderClothes();

	if(Global::instance().m_canTexture && Global::instance().m_enableTexture && !Global::instance().getLightMesh()) {
		::glDisable(GL_TEXTURE_2D);
	}
	cgutils::disableBlend();
}

bool loadTextures()
{
	FaceGroup &facegroup(mesh->getFaceGroupRef());

	for (FaceGroup::iterator facegroup_it = facegroup.begin();
	     facegroup_it != facegroup.end(); facegroup_it++) {
		string name((*facegroup_it).first);
		Global::instance().setTexture(name, new Texture());
		if (!Global::instance().getTexture(name)->load(
		        searchPixmapFile("ui/" + name + "_color.png"))) {
			cerr << "couldn't load base skin_color Texture Data " << name
			     << "_color.png" << endl;
			return false;
		}
	}

	return true;
}

void renderSubsurf()
{
	const FaceVector &facevector_subd(mesh->getFaceVectorSubdRef());
	const subdVertexVector &vertexvector_subd_f(mesh->getVertexVectorSubdFRef());
	const subdVertexVector &vertexvector_subd_e(mesh->getVertexVectorSubdERef());
	const origVertexVector &vertexvector_subd_o(mesh->getVertexVectorSubdORef());
	const MaterialVector &materialvector(mesh->getMaterialVectorRef());

	FaceGroup &facegroup(mesh->getSubdFaceGroupRef());

	if (Global::instance().getQuotedBox()) {
		for (int i = 0; i < 6; i++) {
			twopoints[i] = 0;
		}
	}

	::glBegin(Global::instance().getLightMesh() ? GL_POINTS : GL_QUADS);

	for (FaceGroup::iterator facegroup_it = facegroup.begin();
	     facegroup_it != facegroup.end(); facegroup_it++) {

		if ((*facegroup_it).second.visible == false)
			continue;

		FGroupData &groupdata = (*facegroup_it).second.facesIndexes;

		for (unsigned int i = 0; i < groupdata.size(); i++) {
			// if(Global::instance().getLightMesh() && i % 10 != 0) continue;
			const Face &face(facevector_subd[groupdata[i]]);

			int material_index = face.getMaterialIndex();
			if (material_index != -1) {
				const Material &material(materialvector[material_index]);
				const Color &color(material.getRGBCol());

				// Set the color for these vertices
				::glColor4fv(color.getAsOpenGLVector());
			}

			//::glNormal3fv ((*f_it).no.getAsOpenGLVector());

			::glNormal3fv(
			    vertexvector_subd_o[face.getVertexAtIndex(0)].no.getAsOpenGLVector());
			::glVertex3fv(
			    vertexvector_subd_o[face.getVertexAtIndex(0)].co.getAsOpenGLVector());

			::glNormal3fv(
			    vertexvector_subd_e[face.getVertexAtIndex(1)].no.getAsOpenGLVector());
			::glVertex3fv(
			    vertexvector_subd_e[face.getVertexAtIndex(1)].co.getAsOpenGLVector());

			::glNormal3fv(
			    vertexvector_subd_f[face.getVertexAtIndex(2)].no.getAsOpenGLVector());
			::glVertex3fv(
			    vertexvector_subd_f[face.getVertexAtIndex(2)].co.getAsOpenGLVector());

			::glNormal3fv(
			    vertexvector_subd_e[face.getVertexAtIndex(3)].no.getAsOpenGLVector());
			::glVertex3fv(
			    vertexvector_subd_e[face.getVertexAtIndex(3)].co.getAsOpenGLVector());

			if (Global::instance().getQuotedBox()) {
				calcMinMax(vertexvector_subd_o[face.getVertexAtIndex(0)].co);
			}
		}
	}

	::glEnd();
}
