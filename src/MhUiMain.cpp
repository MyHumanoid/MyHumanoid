#include "MhUiMain.h"

#include <array>
#include <mutex>
#include <optional>
#include <imgui.h>

#include "animorph/BodySettings.h"

#include "io/ColladaExporter.h"
#include "io/Gltf.h"
#include "io/ObjExporter.h"

#include "render/GlTypes.h"
#include "render/RenderUtils.h"

#include "CompositeMorph.h"
#include "GlInclude.h"
#include "Global.h"
#include "MhConfig.h"
#include "MhRender.h"
#include "MhUi.h"
#include "MhUiMorph.h"
#include "MhUiPose.h"
#include "Logger.h"
#include "StringUtils.h"
#include "util.h"
#include "Version.h"

extern SDL_Window * g_mainWindow;


// ================================================================================================

static IconMap g_charactersIconTextures;

void CreateCaractersIconTextures()
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
	
//	if(filename.substr(filename.size() - 1, 1) != "/") {
//		filename.append("/");
//	}
	
	//fs::create_directories(filename);
	
	bool state = collada_export.exportFile(filename);
	
	if(state) {
		log_info("Collada exported");
	} else {
		log_error("Collada export failed");
	}
}

static void exportGltf(const string & fileName)
{
	IoGltfWrite(g_mesh, fileName);
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

bool g_userRequestedQuit = false;
bool g_userAcceptedQuit = false;

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
			if(ImGui::ImageButton("Load Character", (intptr_t)tex.handle, ImVec2(48, 48))) {
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

class FileDialog {
	std::recursive_mutex m_mutex;
	std::string m_toLoad;
	
public:
	void openFileDialog()
	{
		SDL_ShowOpenFileDialog([](void *userdata, const char * const *filelist, int filter) {
			auto c = static_cast<FileDialog*>(userdata);
			
			if(filelist == nullptr) {
				printf("Error: %s\n", SDL_GetError());
				return;
			}
			if(*filelist == nullptr) {
				puts("User pressed cancel.");
				return;
			}
			
			for(; *filelist != NULL; filelist++) {
				printf("File %s\n", *filelist);
			}
			
		}, static_cast<void*>(this), g_mainWindow, nullptr, 0, nullptr, false);
	}
	
	void saveFileDialog()
	{
		std::array<SDL_DialogFileFilter, 3> filters = {
			(SDL_DialogFileFilter) {"Wavefront Obj", "obj"},
			(SDL_DialogFileFilter) {"GlTF", "glb"},
			(SDL_DialogFileFilter) {"Collada", "dae"}
		};
		
		SDL_ShowSaveFileDialog([](void *userdata, const char * const *filelist, int filter) {
			auto c = static_cast<FileDialog*>(userdata);
			
			if(filelist == nullptr) {
				log_error("{}", SDL_GetError());
				return;
			}
			if(*filelist == nullptr) {
				log_info("User pressed cancel.");
				return;
			}
			
			for(; *filelist != NULL; filelist++) {
				std::string path(*filelist);
				{
					std::lock_guard<std::recursive_mutex> lk(c->m_mutex);
					c->m_toLoad = path;
				}
				break;
			}
			
		}, static_cast<void*>(this), g_mainWindow, filters.data(), filters.size(), nullptr);
	}
	
	void process() {
		std::lock_guard<std::recursive_mutex> lk(m_mutex);
		
		if(m_toLoad.size() == 0) {
			return;
		}
		
		if(m_toLoad.ends_with("obj")) {
			exportMeshObj(m_toLoad);
		} else if(m_toLoad.ends_with("glb")) {
			exportGltf(m_toLoad);
		} else if(m_toLoad.ends_with("dae")) {
			exportCollada(m_toLoad);
		} else {
			log_error("Unknown filetype: {}", m_toLoad);
		}
		m_toLoad = "";
	}
};

FileDialog g_fileDialog;

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
			if(ImGui::ImageButton("Load Pose", (intptr_t)tex.handle, ImVec2(48, 48))) {
				
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
	ImGui::Text("%s", prettyVersion().c_str());
	
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
			g_userAcceptedQuit = true;
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
			if(ImGui::MenuItem("Export model")) {
				g_fileDialog.saveFileDialog();
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Quit...")) {
				g_userRequestedQuit = true;
			}
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

			if(ImGui::Button("Toggle Fullscreen (F11)")) {
				g_global.toggleFullscreen();
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
	
	if(g_userRequestedQuit) {
		g_userRequestedQuit = false;
		ImGui::OpenPopup("Quit?");
	}
	
	DisplayQuitPopup();
}

void ExecuteDeferredActions()
{
	if(g_requestShaderReload) {
		g_requestShaderReload = false;
		
		g_renderBody.loadShader(g_requestShaderVersion);
	}
	if(g_requestBackgroundShaderReload) {
		g_requestBackgroundShaderReload = false;
		
		g_renderBackground.loadShader();
	}
	
	g_fileDialog.process();
}


