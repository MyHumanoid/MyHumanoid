#include "MhUiMorph.h"

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhUi.h"

#include "Global.h"
#include "ComponentID.h"
#include "CompositeMorph.h"
#include "MhConfig.h"
#include "MhUiCommon.h"

#include "render/RenderUtils.h"
#include "util.h"

using glm::vec2;


static IconMap g_targetImageTextures;

void CreateTargetImageTextures()
{
	loadTexturesFromDir(g_targetImageTextures, "data/pixmaps/tgimg");
}


void XYfoobar(const char* str_id, OptTex texture, glm::vec2 & value)
{
	if(!texture) {
		return;
	}
	
	ImGuiIO & io = ImGui::GetIO();
	
	MhGui::ImageButton(str_id, texture.value(), ImVec2(192, 104));
	vec2 pMin = ImGui::GetItemRectMin();
	vec2 size = ImGui::GetItemRectSize();
	if(ImGui::IsItemActive()) {
		vec2 relPos = vec2(io.MousePos) - pMin;
		relPos      = glm::max(relPos, vec2(0));
		relPos      = glm::min(relPos, size);
		
		value = glm::vec2(relPos.x, size.y - relPos.y) / foobarScale;
		
		g_grids.applyCompositeMorphTargets(g_mesh);
		
		//value       = vec2(relPos) / vec2(size);
	}
	//vec2        cursorPos    = pMin + vec2(value * vec2(size));
	
	vec2 scaled = value * foobarScale;
	vec2 cursorPos = pMin + vec2(scaled.x, 104) - vec2(0, scaled.y);
	
	float       radius       = 5.0f;
	const ImU32 col_white    = IM_COL32(255, 255, 255, 255);
	const ImU32 col_midgrey  = IM_COL32(128, 128, 128, 255);
	const ImU32 col_midgrey2 = IM_COL32(255,   0,  24, 128);
	
	auto * dl = ImGui::GetWindowDrawList();
	dl->AddCircleFilled(cursorPos, radius, col_midgrey2, 12);
	dl->AddCircle(cursorPos, radius + 1, col_midgrey);
	dl->AddCircle(cursorPos, radius, col_white);
}

void DisplayCharacterSettings()
{
	using glm::vec2;

	struct Textures {
		OptTex ageGenderBkg = LoadTextureFromFile("data/pixmaps/ui/age_selector.png");
		OptTex massBkg      = LoadTextureFromFile("data/pixmaps/ui/muscle_size_selector.png");
		OptTex breastBkg    = LoadTextureFromFile("data/pixmaps/ui/breast_selector.png");
		OptTex shapeBkg     = LoadTextureFromFile("data/pixmaps/ui/shape_selector.png");
	};

	static const Textures tex;

	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vec2(10, 10));
	
	ImGui::SetNextWindowSize(vec2(212, 492));
	ImGui::Begin("Character Setting",
	             &g_config.characterSettings.visible,
	             ImGuiWindowFlags_NoResize);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, vec2(0, 12));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, vec2(0));
	
	//ImGui::Text("Age/Sex");
	XYfoobar("Age/Sex", tex.ageGenderBkg, g_grids.agePos);
	//ImGui::Text("Bodymass Weight/Muscle");
	XYfoobar("Bodymass", tex.massBkg, g_grids.muscleSizePos);
	//ImGui::Text("Breast Size/Shape");
	XYfoobar("Breast", tex.breastBkg, g_grids.breastPos);
	//ImGui::Text("Bodyshape Shape/Height");
	XYfoobar("Bodyshape", tex.shapeBkg, g_grids.shapePos);
	
	ImGui::PopStyleVar(3);
	
	ImGui::End();
}

bool isCompositeMorphTarget(const std::string & target_name)
{
	if(target_name.find("ages", 0) != string::npos ||
	   target_name.find("breast", 0) != string::npos ||
	   target_name.find("muscleSize", 0) != string::npos ||
	   target_name.find("shapes", 0) != string::npos) {
		return true;
	}
	return false;
}

struct MorphGroupWin : public TileGroupChildWindow<MorphGroupWin> {

	// clang-format off
	const std::array<const Tile, 60> tiles = {
		Tile("body_01",  "Torso",         "torso"),
		Tile("body_02",  "Head",          "head"),
		Tile("face_01",  "",              ""),
		Tile("face_02",  "",              ""),
		Tile("face_03",  "",              ""),
		Tile("face_04",  "",              ""),
		// ==========================================
		Tile("body_03",  "Shoulders",     "shoulders"),
		Tile("body_04",  "Neck",          "neck"),
		Tile("face_05",  "",              ""),
		Tile("face_06",  "Forehead",      "forehead"),
		Tile("face_07",  "",              ""),
		Tile("face_08",  "",              ""),
		// ==========================================
		Tile("body_05",  "Upper arms",    "upper_arms"),
		Tile("body_06",  "",              ""),
		Tile("face_09",  "",              ""),
		Tile("face_10",  "Eyes",          "eyes"),
		Tile("face_11",  "",              ""),
		Tile("face_12",  "",              ""),
		// ==========================================
		Tile("body_07",  "Lower arms",    "lower_arms"),
		Tile("body_08",  "Abdomen",       "abdomen"),
		Tile("face_13",  "Nose",          "nose"),
		Tile("face_14",  "Cheek",         "cheek"),
		Tile("face_15",  "Ears",          "ears"),
		Tile("face_16",  "",              ""),
		// ==========================================
		Tile("body_09",  "Hands",         "hands"),
		Tile("body_10",  "Pelvis",        "pelvis"),
		Tile("face_17",  "Mouth",         "mouth"),
		Tile("face_18",  "",              ""),
		Tile("face_19",  "",              ""),
		Tile("face_20",  "",              ""),
		// ==========================================
		Tile("body_11",  "Upper legs",    "upper_legs"),
		Tile("body_12",  "",              ""),
		Tile("face_21",  "Chin Jaw",      "chin_jaw"),
		Tile("face_22",  "",              ""),
		Tile("face_23",  "",              ""),
		Tile("face_24",  "",              ""),
		// ==========================================
		Tile("body_13",  "",              ""),
		Tile("body_14",  "",              ""),
		Tile("hands_01", "Forefinger",    "forefinger"),
		Tile("hands_02", "Middlefinger",  "middlefinger"),
		Tile("hands_03", "Ringfinger",    "ringfinger"),
		Tile("hands_04", "",              ""),
		// ==========================================
		Tile("body_15",  "Lower legs",    "lower_legs"),
		Tile("body_16",  "",              ""),
		Tile("hands_05", "Pollex",        "pollex"),
		Tile("hands_06", "",              ""),
		Tile("hands_07", "Littlefinger",  "littlefinger"),
		Tile("hands_08", "",              ""),
		// ==========================================
		Tile("body_17",  "Feet",          "feet"),
		Tile("body_18",  "",              ""),
		Tile("hands_09", "",              ""),
		Tile("hands_10", "",     ""),
		Tile("hands_11", "",     ""),
		Tile("hands_12", "",     ""),
		// ==========================================
		Tile("teeth_01", "Tongue",        "tongue"),
		Tile("teeth_02", "Teeth",         "teeth"),
		Tile("teeth_03", "",     ""),
		Tile("teeth_04", "",     ""),
		Tile("teeth_05", "",     ""),
		Tile("teeth_06", "",     ""),
	};
	// clang-format on
};

auto applier = [](const std::string & name, const float & value, const bool deleteOnZero) {
	g_mesh.setMorphTarget(name, value);
	g_mesh.calcNormals();
};

auto nopApplier = [](const std::string & name, const float & value, const bool deleteOnZero) {

};


void DisplayMorphTargets()
{
	ImGui::SetNextWindowSize(vec2(380, 540));
	if(!ImGui::Begin("Morph Targets",
	                 &g_config.morphTargets.visible,
	                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)
	) {
		ImGui::End();
		return;
	}

	static MorphGroupWin tileWin;

	tileWin.m_tooltip = "";
	tileWin.m_categoryRight = "";
	tileWin.DisplayGroupTiles();
	
	if(tileWin.m_categoryRight != "") {
		auto & target = tileWin.m_categoryRight;
		std::string pathAutozoom_data =
		    searchDataDir("targets") + "/" + target + "/" + target + ".camera";
		if(g_global.autozoom->lazyLoadData(pathAutozoom_data)) {
			g_global.camera->moveCameraAnimated(
			    pathAutozoom_data,
			    g_global.autozoom->getAutozoomData(pathAutozoom_data),
			    g_mesh.getVertexVectorRef());
		}
	}
	
	bool groupTooltip = false;
	if(tileWin.m_tooltip != "") {
		groupTooltip = true;
	}
	
	
	ImGui::SameLine();
	{
		ImGui::BeginChild("Morph Targets", vec2(140, 450), false);

		Animorph::BodySettings bodyset = g_mesh.morphTargets();

		for(const auto & [name, value] : g_mesh.targets()) {

			if(isCompositeMorphTarget(name)) {
				continue;
			}

			if(!pathStartsWith(name, tileWin.m_category)) {
				continue;
			}

			float target_value = bodyset.m_targets[name];


			const auto & minmax = std::make_pair(0, 1);

			DrawTargetRow(g_targetImageTextures, minmax, name, target_value,
			              tileWin.m_tooltip, applier);
		}
		ImGui::EndChild();
	}
	ImGui::Text("%s", tileWin.m_tooltip.c_str());
	if(groupTooltip) {
		ImGui::Text("Left Mouse: select / Right Mouse: zoom");
	}
	
	ImGui::End();
}

void DisplayMorphTargetsApplied()
{
	if(!ImGui::Begin("Applied Morph Targets", &g_config.morphTargetsApplied.visible)) {
		ImGui::End();
		return;
	}
	
	if(ImGui::CollapsingHeader("Composite Targets")) {
		for(const auto & [name, value] : g_mesh.morphTargets().m_targets) {
			
			if(!isCompositeMorphTarget(name)) {
				continue;
			}
			
			DrawAppliedRow(g_targetImageTextures,
			               std::make_pair(0.f, 1.f),
			               name, value, nopApplier);
		}
	}
	
	for(const auto & [name, value] : g_mesh.morphTargets().m_targets) {

		if(isCompositeMorphTarget(name)) {
			continue;
		}
		
		DrawAppliedRow(g_targetImageTextures,
		               std::make_pair(0.f, 1.f),
		               name, value, applier);
	}
	
	ImGui::End();
}
