#include "MhUiPose.h"

#include <array>

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhUi.h"

#include "Global.h"
#include "ComponentID.h"

#include "MhConfig.h"
#include "MhUiCommon.h"

using glm::vec2;


static IconMap g_poseImageTextures;

void CreatePoseImageTextures()
{
	loadTexturesFromDir(g_poseImageTextures, "data/pixmaps/rotimg");
}

struct PoseGroupWin : public TileGroupChildWindow<PoseGroupWin> {

	// clang-format off
	const std::array<const Tile, 84> tiles = {
		Tile("rotations_01", "Right collar",          "260_right_collar"),
		Tile("rotations_02", "Head",                  "300_head"),
		Tile("rotations_03", "Left collar",           "280_left_collar"),
		Tile("rotations_04", "Left ringfinger 3",     "070_left_ringfinger_3"),
		Tile("rotations_05", "Left middlefinger 3",   "067_left_middlefinger_3"),
		Tile("rotations_06", "Left forefinger 3",     "064_left_forefinger_3"),
		// ==========================================
		Tile("rotations_07", "Right upper arm",       "220_right_upper_arm"),
		Tile("rotations_08", "Neck",                  "320_neck"),
		Tile("rotations_09", "Left upper arm",        "240_left_upper_arm"),
		Tile("rotations_10", "Left ringfinger 2",     "071_left_ringfinger_2"),
		Tile("rotations_11", "Left middlefinger 2",   "068_left_middlefinger_2"),
		Tile("rotations_12", "Left forefinger 2",     "065_left_forefinger_2"),
		// ==========================================
		Tile("rotations_13", "",                      ""),
		Tile("rotations_14", "Torso",                 "360_torso"),
		Tile("rotations_15", "",                      ""),
		Tile("rotations_16", "Left littlefinger 3",   "073_left_littlefinger_3"),
		Tile("rotations_17", "Left middlefinger 1",   "069_left_middlefinger_1"),
		Tile("rotations_18", "Left forefinger 1",     "066_left_forefinger_1"),
		// ==========================================
		Tile("rotations_19", "Right lower arm",       "140_right_lower_arm"),
		Tile("rotations_20", "Pivot transformation",  "380_pivot"),
		Tile("rotations_21", "Left lower arm",        "160_left_lower_arm"),
		Tile("rotations_22", "Left littlefinger 2",   "074_left_littlefinger_2"),
		Tile("rotations_23", "Left ringfinger 1",     "072_left_ringfinger_1"),
		Tile("rotations_24", "Left pollex 3",         "061_left_pollex_3"),
		// ==========================================
		Tile("rotations_25", "Right hand",            "060_right_hand"),
		Tile("rotations_26", "",                      ""),
		Tile("rotations_27", "Left hand",             "080_left_hand"),
		Tile("rotations_28", "Left littlefinger 1",   "075_left_littlefinger_1"),
		Tile("rotations_29", "Left pollex 1",         "063_left_pollex_1"),
		Tile("rotations_30", "Left pollex 2",         "062_left_pollex_2"),
		// ==========================================
		Tile("rotations_31", "Right upper leg",       "180_right_upper_leg"),
		Tile("rotations_32", "",                      ""),
		Tile("rotations_33", "Left upper leg",        "200_left_upper_leg"),
		Tile("rotations_34", "Right forefinger 3",    "044_right_forefinger_3"),
		Tile("rotations_35", "Right middlefinger 3",  "047_right_middlefinger_3"),
		Tile("rotations_36", "Right ringfinger 3",    "050_right_ringfinger_3"),
		// ==========================================
		Tile("rotations_37", "Right lower leg",       "100_right_lower_leg"),
		Tile("rotations_38", "",                      ""),
		Tile("rotations_39", "Left lower leg",        "120_left_lower_leg"),
		Tile("rotations_40", "Right forefinger 2",    "045_right_forefinger_2"),
		Tile("rotations_41", "Right middlefinger 2",  "048_right_middlefinger_2"),
		Tile("rotations_42", "Right ringfinger 2",    "051_right_ringfinger_2"),
		// ==========================================
		Tile("rotations_43", "",                      ""),
		Tile("rotations_44", "",                      ""),
		Tile("rotations_45", "",                      ""),
		Tile("rotations_46", "Right forefinger 1",    "046_right_forefinger_1"),
		Tile("rotations_47", "Right middlefinger 1",  "049_right_middlefinger_1"),
		Tile("rotations_48", "Right littlefinger 3",  "053_right_littlefinger_3"),
		// ==========================================
		Tile("rotations_49", "Right foot",            "020_right_foot"),
		Tile("rotations_50", "",                      ""),
		Tile("rotations_51", "Left foot",             "040_left_foot"),
		Tile("rotations_52", "Right pollex 3",        "041_right_pollex_3"),
		Tile("rotations_53", "Right ringfinger 1",    "052_right_ringfinger_1"),
		Tile("rotations_54", "Right littlefinger 2",  "054_right_littlefinger_2"),
		// ==========================================
		Tile("rotations_55", "",                      ""),
		Tile("rotations_56", "",                      ""),
		Tile("rotations_57", "",                      ""),
		Tile("rotations_58", "Right pollex 2",        "042_right_pollex_2"),
		Tile("rotations_59", "Right pollex 1",        "043_right_pollex_1"),
		Tile("rotations_60", "Right littlefinger 1",  "055_right_littlefinger_1"),
		// ==========================================
		Tile("rotations_61", "Right footfinger 4_2",  "002_right_footfinger_4_2"),
		Tile("rotations_62", "Right footfinger 5_2",  "000_right_footfinger_5_2"),
		Tile("rotations_63", "Right footfinger 5_1",  "001_right_footfinger_5_1"),
		Tile("rotations_64", "Left footfinger 5_1",   "022_left_footfinger_5_1"),
		Tile("rotations_65", "Left footfinger 5_2",   "021_left_footfinger_5_2"),
		Tile("rotations_66", "Left footfinger 4_2",   "023_left_footfinger_4_2"),
		// ==========================================
		Tile("rotations_67", "Right footfinger 3_2",  "004_right_footfinger_3_2"),
		Tile("rotations_68", "Right footfinger 3_1",  "005_right_footfinger_3_1"),
		Tile("rotations_69", "Right footfinger 4_1",  "003_right_footfinger_4_1"),
		Tile("rotations_70", "Left footfinger 4_1",   "024_left_footfinger_4_1"),
		Tile("rotations_71", "Left footfinger 3_1",   "026_left_footfinger_3_1"),
		Tile("rotations_72", "Left footfinger 3_2",   "025_left_footfinger_3_2"),
		// ==========================================
		Tile("rotations_73", "Right footfinger 2_2",  "006_right_footfinger_2_2"),
		Tile("rotations_74", "Right footfinger 2_1",  "007_right_footfinger_2_1"),
		Tile("rotations_75", "Right footfinger 1_1",  "009_right_footfinger_1_1"),
		Tile("rotations_76", "Left footfinger 1_1",   "030_left_footfinger_1_1"),
		Tile("rotations_77", "Left footfinger 2_1",   "028_left_footfinger_2_1"),
		Tile("rotations_78", "Left footfinger 2_2",   "027_left_footfinger_2_2"),
		// ==========================================
		Tile("rotations_79", "Right footfinger 1_2",  "008_right_footfinger_1_2"),
		Tile("rotations_80", "",                      ""),
		Tile("rotations_81", "",                      ""),
		Tile("rotations_82", "",                      ""),
		Tile("rotations_83", "",                      ""),
		Tile("rotations_84", "Left footfinger 1_2",   "029_left_footfinger_1_2"),
	};
	// clang-format on
};

auto applier = [](const std::string & name, const float & value, const bool deleteOnZero) {
	g_mesh.setPose(name, value, deleteOnZero);
	g_mesh.calcNormals();
};


void DisplayPoseTargets()
{

	constexpr static ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoScrollbar
	                                             //      |ImGuiWindowFlags_NoSavedSettings
	                                             | ImGuiWindowFlags_NoResize;

	struct WinStack {
		WinStack() { ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vec2(1, 1)); }
		~WinStack() { ImGui::PopStyleVar(1); }
	};

	ImGui::SetNextWindowSize(vec2(380, 510));
	WinStack winStack;
	if(!ImGui::Begin("Pose Targets", &g_config.poseTargets.visible, winFlags)) {
		ImGui::End();
		return;
	}

	static PoseGroupWin tileWin;

	tileWin.m_tooltip = "";
	tileWin.DisplayGroupTiles();
	ImGui::SameLine();
	{
		ImGui::BeginChild("Pose Targets", vec2(140, 440), false);
		
		Animorph::BodySettings bodyset = g_mesh.poseTargets();

		for(const auto & [target_name, tarVal] : g_mesh.posemap()) {
			Animorph::PoseTarget * poseTarget = g_mesh.getPoseTargetForName(target_name);
			assert(poseTarget);

			auto bodyset_it = bodyset.m_targets.find(target_name);

			// FIX: Make sure that a bodyset with the given name really exists!
			float target_value =
			        (bodyset_it != bodyset.m_targets.end()) ? bodyset_it->second : 0.0f;

			if(!pathStartsWith(target_name, tileWin.m_category)) {
				continue;
			}

			const auto & minmax = std::make_pair(poseTarget->getMinAngle(),
			                                     poseTarget->getMaxAngle());

			DrawTargetRow(g_poseImageTextures, minmax, target_name, target_value,
			              tileWin.m_tooltip, applier);
		}
		ImGui::EndChild();
	}
	ImGui::Text("%s", tileWin.m_tooltip.c_str());
	ImGui::End();
}

void DisplayPoseTargetsApplied()
{
	if(!ImGui::Begin("Applied Pose Targets", &g_config.poseTargetsApplied.visible)) {
		ImGui::End();
		return;
	}

	for(const auto & [target_name, target_value] : g_mesh.poseTargets().m_targets) {

		Animorph::PoseTarget * poseTarget = g_mesh.getPoseTargetForName(target_name);
		assert(poseTarget);

		const auto & minmax =
		        std::make_pair(poseTarget->getMinAngle(), poseTarget->getMaxAngle());

		DrawAppliedRow(g_poseImageTextures, minmax, target_name, target_value, applier);
	}
	ImGui::End();
}
