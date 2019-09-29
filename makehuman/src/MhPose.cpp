#include "MhPose.h"

#include "render/RenderUtils.h"

#include "MhGui.h"
#include "MhGuiData.h"

#include "gui/Window.h"
#include "Global.h"
#include "ComponentID.h"
#include "PoseTargetPanel.h"

void DisplayPoseTargetSelector() {
	
	using glm::ivec2;
	
	constexpr static ImGuiWindowFlags winFlags
	    = ImGuiWindowFlags_NoScrollbar
	      |ImGuiWindowFlags_NoSavedSettings
	      | ImGuiWindowFlags_NoResize;
	
	ImGui::SetNextWindowSize(ivec2(219, 550));
	if(!ImGui::Begin("Pose Target Groups", &g_displayWin.poseTargetFoo, winFlags)) {
		ImGui::End();
		return;
	}
	
	struct Goo {
		
		void click() const {
			Window & mainWindow = *g_mainWindow;
			
			PoseTargetPanel * targetPanel = dynamic_cast<PoseTargetPanel *>(
			    mainWindow.getPanel(kComponentID_TargetPanel));
			
			// Check if this Target Panel is not the current one?
			if((targetPanel == NULL) || (targetPanel->getCategory() != target)) {
				// No? The create it
				int x = mainWindow.getSize().getWidth() - 210;
				
				mainWindow.removePanel(targetPanel);
				delete targetPanel;
				
				targetPanel = new PoseTargetPanel(target, Rect(x, 40, 210, 517));
				
				mainWindow.addPanel(targetPanel);
				targetPanel->createWidgets();
			}
			
			targetPanel->show_all();
		}
		
		
		std::string as;
		std::string tip;
		std::string target;
		std::optional<mh::Texture> tex;
		std::optional<mh::Texture> texOver;
		Goo(const std::string & img, const std::string & _tip, const std::string & _targ)
		{
			tip = _tip;
			target = _targ;
			as = "pixmaps/ui/" + img;
			tex = LoadTextureFromFile(as + ".png");
			texOver = LoadTextureFromFile(as + "_over.png");
		}
		
		void gui() const {
			//ImGui::SetColumnWidth(-1, 32);
			
			auto p = ImGui::GetCursorPos();
			if(ImGui::InvisibleButton(as.c_str(), ivec2(32, 32))){
				click();
			}
			ImGui::SetCursorPos(p);
			if(ImGui::IsItemHovered()) {
				if(texOver)
					MhGui::Image(texOver.value(), ivec2(32, 32));
			} else {
				if(tex)
					MhGui::Image(tex.value(), ivec2(32, 32));
			}
			ImGui::NextColumn();
		}
	};
	
	    // clang-format off
	static const auto goobar = {
		Goo("rotations_01", "Right collar parameters",          "260_right_collar"),
		Goo("rotations_02", "Head parameters",                  "300_head"),
		Goo("rotations_03", "Left collar parameters",           "280_left_collar"),
		Goo("rotations_04", "Left ringfinger 3 parameters",     "070_left_ringfinger_3"),
		Goo("rotations_05", "Left middlefinger 3 parameters",   "067_left_middlefinger_3"),
		Goo("rotations_06", "Left forefinger 3 parameters",     "064_left_forefinger_3"),
		Goo("rotations_07", "Right upper arm parameters",       "220_right_upper_arm"),
		Goo("rotations_08", "Neck parameters",                  "320_neck"),
		Goo("rotations_09", "Left upper arm parameters",        "240_left_upper_arm"),
		// ==========================================
		Goo("rotations_10", "Left ringfinger 2 parameters",     "071_left_ringfinger_2"),
		Goo("rotations_11", "Left middlefinger 2 parameters",   "068_left_middlefinger_2"),
		Goo("rotations_12", "Left forefinger 2 parameters",     "065_left_forefinger_2"),
		Goo("rotations_13", "",                                 ""),
		Goo("rotations_14", "Torso parameters",                 "360_torso"),
		Goo("rotations_15", "",                                 ""),
		Goo("rotations_16", "Left littlefinger 3 parameters",   "073_left_littlefinger_3"),
		Goo("rotations_17", "Left middlefinger 1 parameters",   "069_left_middlefinger_1"),
		Goo("rotations_18", "Left forefinger 1 parameters",     "066_left_forefinger_1"),
		Goo("rotations_19", "Right lower arm parameters",       "140_right_lower_arm"),
		// ==========================================
		Goo("rotations_20", "Pivot transformation parameters",  "380_pivot"),
		Goo("rotations_21", "Left lower arm parameters",        "160_left_lower_arm"),
		Goo("rotations_22", "Left littlefinger 2 parameters",   "074_left_littlefinger_2"),
		Goo("rotations_23", "Left ringfinger 1 parameters",     "072_left_ringfinger_1"),
		Goo("rotations_24", "Left pollex 3 parameters",         "061_left_pollex_3"),
		Goo("rotations_25", "Right hand parameters",            "060_right_hand"),
		Goo("rotations_26", "",                                 ""),
		Goo("rotations_27", "Left hand parameters",             "080_left_hand"),
		Goo("rotations_28", "Left littlefinger 1 parameters",   "075_left_littlefinger_1"),
		Goo("rotations_29", "Left pollex 1 parameters",         "063_left_pollex_1"),
		// ==========================================
		Goo("rotations_30", "Left pollex 2 parameters",         "062_left_pollex_2"),
		Goo("rotations_31", "Right upper leg parameters",       "180_right_upper_leg"),
		Goo("rotations_32", "",                                 ""),
		Goo("rotations_33", "Left upper leg parameters",        "200_left_upper_leg"),
		Goo("rotations_34", "Right forefinger 3 parameters",    "044_right_forefinger_3"),
		Goo("rotations_35", "Right middlefinger 3 parameters",  "047_right_middlefinger_3"),
		Goo("rotations_36", "Right ringfinger 3 parameters",    "050_right_ringfinger_3"),
		Goo("rotations_37", "Right lower leg parameters",       "100_right_lower_leg"),
		Goo("rotations_38", "",                                 ""),
		Goo("rotations_39", "Left lower leg parameters",        "120_left_lower_leg"),
		// ==========================================
		Goo("rotations_40", "Right forefinger 2 parameters",    "045_right_forefinger_2"),
		Goo("rotations_41", "Right middlefinger 2 parameters",  "048_right_middlefinger_2"),
		Goo("rotations_42", "Right ringfinger 2 parameters",    "051_right_ringfinger_2"),
		Goo("rotations_43", "",                                 ""),
		Goo("rotations_44", "",                                 ""),
		Goo("rotations_45", "",                                 ""),
		Goo("rotations_46", "Right forefinger 1 parameters",    "046_right_forefinger_1"),
		Goo("rotations_47", "Right middlefinger 1 parameters",  "049_right_middlefinger_1"),
		Goo("rotations_48", "Right littlefinger 3 parameters",  "053_right_littlefinger_3"),
		Goo("rotations_49", "Right foot parameters",            "020_right_foot"),
		// ==========================================
		Goo("rotations_50", "",                                 ""),
		Goo("rotations_51", "Left foot parameters",             "040_left_foot"),
		Goo("rotations_52", "Right pollex 3 parameters",        "041_right_pollex_3"),
		Goo("rotations_53", "Right ringfinger 1 parameters",    "052_right_ringfinger_1"),
		Goo("rotations_54", "Right littlefinger 2 parameters",  "054_right_littlefinger_2"),
		Goo("rotations_55", "",                                 ""),
		Goo("rotations_56", "",                                 ""),
		Goo("rotations_57", "",                                 ""),
		Goo("rotations_58", "Right pollex 2 parameters",        "042_right_pollex_2"),
		Goo("rotations_59", "Right pollex 1 parameters",        "043_right_pollex_1"),
		// ==========================================
		Goo("rotations_60", "Right littlefinger 1 parameters",  "055_right_littlefinger_1"),
		Goo("rotations_61", "Right footfinger 4_2 parameters",  "002_right_footfinger_4_2"),
		Goo("rotations_62", "Right footfinger 5_2 parameters",  "000_right_footfinger_5_2"),
		Goo("rotations_63", "Right footfinger 5_1 parameters",  "001_right_footfinger_5_1"),
		Goo("rotations_64", "Left footfinger 5_1 parameters",   "022_left_footfinger_5_1"),
		Goo("rotations_65", "Left footfinger 5_2 parameters",   "021_left_footfinger_5_2"),
		Goo("rotations_66", "Left footfinger 4_2 parameters",   "023_left_footfinger_4_2"),
		Goo("rotations_67", "Right footfinger 3_2 parameters",  "004_right_footfinger_3_2"),
		Goo("rotations_68", "Right footfinger 3_1 parameters",  "005_right_footfinger_3_1"),
		Goo("rotations_69", "Right footfinger 4_1 parameters",  "003_right_footfinger_4_1"),
		// ==========================================
		Goo("rotations_70", "Left footfinger 4_1 parameters",   "024_left_footfinger_4_1"),
		Goo("rotations_71", "Left footfinger 3_1 parameters",   "026_left_footfinger_3_1"),
		Goo("rotations_72", "Left footfinger 3_2 parameters",   "025_left_footfinger_3_2"),
		Goo("rotations_73", "Right footfinger 2_2 parameters",  "006_right_footfinger_2_2"),
		Goo("rotations_74", "Right footfinger 2_1 parameters",  "007_right_footfinger_2_1"),
		Goo("rotations_75", "Right footfinger 1_1 parameters",  "009_right_footfinger_1_1"),
		Goo("rotations_76", "Left footfinger 1_1 parameters",   "030_left_footfinger_1_1"),
		Goo("rotations_77", "Left footfinger 2_1 parameters",   "028_left_footfinger_2_1"),
		Goo("rotations_78", "Left footfinger 2_2 parameters",   "027_left_footfinger_2_2"),
		Goo("rotations_79", "Right footfinger 1_2 parameters",  "008_right_footfinger_1_2"),
		// ==========================================
		Goo("rotations_80", "",                                 ""),
		Goo("rotations_81", "",                                 ""),
		Goo("rotations_82", "",                                 ""),
		Goo("rotations_83", "",                                 ""),
		Goo("rotations_84", "Left footfinger 1_2 parameters",   "029_left_footfinger_1_2"),
	};
	// clang-format on
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ivec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ivec2(0, 0));
	ImGui::Columns(6, NULL, false);
	for(const auto & tile: goobar) {
		tile.gui();
	}
	ImGui::PopStyleVar(2);
	ImGui::End();
}

void doPoseFromGui(std::string targetName, float value)
{
	
	Mesh * mesh = g_global.getMesh();
	mesh->setPose(targetName, value);
}

void DisplayPoseTargetRow(const std::string & target_name, const PoseTarget * poseTarget,
                          float & target_value)
{
	fs::path targetImageName = target_name;
	targetImageName.replace_extension();
	
	const auto & texIdIt = g_poseImageTextures.find(targetImageName);
	if(texIdIt != g_poseImageTextures.end()) {
		auto texId = texIdIt->second;
		
		MhGui::Image(texId, ImVec2(16, 16));
		if(ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			MhGui::Image(texId, ImVec2(128, 128));
			ImGui::EndTooltip();
		}
	} else {
		ImGui::Dummy(ImVec2(16, 16));
	}
	ImGui::SameLine(0, 4);
	
	// FIXME only the button in the first line is working
	if(ImGui::Button("X", ImVec2(16, 16))) {
		doPoseFromGui(target_name, 0.f);
	}
	ImGui::SameLine(0, 4);
	
	if(ImGui::SliderFloat(target_name.c_str(), &target_value, poseTarget->getMinAngle(),
	                       poseTarget->getMaxAngle())) {
		// TODO used min so that rotation does not vanish
		if(target_value != 0.f)
			doPoseFromGui(target_name, target_value);
	}
}


void DisplayPoseTargets()
{
	if(!ImGui::Begin("Pose Rotations", &g_displayWin.poseTargets)) {
		ImGui::End();
		return;
	}
	
	Mesh * mesh = g_global.getMesh();
	assert(mesh);
	BodySettings bodyset = mesh->getBodySettings();
	
	for(const auto & posemap_it : mesh->getPoseMapRef()) {
		const string & target_name(posemap_it.first);
		
		PoseTarget * poseTarget = mesh->getPoseTargetForName(target_name);
		assert(poseTarget);
		
		BodySettings::const_iterator bodyset_it = bodyset.find(target_name);
		
		// FIX: Make sure that a bodyset with the given name really exists!
		float target_value = (bodyset_it != bodyset.end()) ? bodyset_it->second : 0.0f;
		
		DisplayPoseTargetRow(target_name, poseTarget, target_value);
	}
	
	ImGui::End();
}


void DisplayPoseTargetsApplied()
{
	if(!ImGui::Begin("Applied Pose Rotations", &g_displayWin.poseTargetsApplied)) {
		ImGui::End();
		return;
	}
	
	Mesh * mesh = g_global.getMesh();
	assert(mesh);
	
	for(const auto & bodyset_it : mesh->getPoses()) {
		
		const string & target_name(bodyset_it.first);
		float          target_value = bodyset_it.second;
		
		PoseTarget * poseTarget = mesh->getPoseTargetForName(target_name);
		assert(poseTarget);
		
		DisplayPoseTargetRow(target_name, poseTarget, target_value);
	}
	
	ImGui::End();
}
