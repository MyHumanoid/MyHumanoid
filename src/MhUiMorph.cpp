#include "MhUiMorph.h"

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhUi.h"

#include "gui/Window.h"
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
	loadTexturesFromDir(g_targetImageTextures, "pixmaps/tgimg");
}


void XYfoobar(OptTex texture, glm::ivec2 & value)
{
	if(!texture) {
		return;
	}
	
	auto *    dl = ImGui::GetForegroundDrawList();
	ImGuiIO & io = ImGui::GetIO();

	MhGui::ImageButton(texture.value(), ImVec2(192, 104));
	vec2 pMin = ImGui::GetItemRectMin();
	vec2 size = ImGui::GetItemRectSize();
	if(ImGui::IsItemActive()) {
		vec2 relPos = vec2(io.MousePos) - pMin;
		relPos      = glm::max(relPos, vec2(0));
		relPos      = glm::min(relPos, size);
		//value       = vec2(relPos) / vec2(size);
	}

	//vec2        cursorPos    = pMin + vec2(value * vec2(size));
	
	vec2 cursorPos = pMin + vec2(value.x, 104) - vec2(0, value.y);
	
	float       radius       = 6.0f;
	const ImU32 col_white    = IM_COL32(255, 255, 255, 255);
	const ImU32 col_midgrey  = IM_COL32(128, 128, 128, 255);
	const ImU32 col_midgrey2 = IM_COL32(128, 128, 128, 128);
	dl->AddCircleFilled(cursorPos, radius, col_midgrey2, 12);
	dl->AddCircle(cursorPos, radius + 1, col_midgrey);
	dl->AddCircle(cursorPos, radius, col_white);
}

void DisplayCharacterSettings()
{
	using glm::vec2;

	struct Textures {
		OptTex ageGenderBkg = LoadTextureFromFile("pixmaps/ui/age_selector.png");
		OptTex massBkg      = LoadTextureFromFile("pixmaps/ui/muscle_size_selector.png");
		OptTex breastBkg    = LoadTextureFromFile("pixmaps/ui/breast_selector.png");
		OptTex shapeBkg     = LoadTextureFromFile("pixmaps/ui/shape_selector.png");
	};

	static const Textures tex;

	Grids foo;
	foo.fromSavedPositions(g_global.m_comp);
	
	
	ImGui::SetNextWindowSize(vec2(220, 800));
	ImGui::Begin("Character Setting");

	ImGui::Text("Age/Sex");
	XYfoobar(tex.ageGenderBkg, foo.agePos);
	ImGui::Text("Bodymass Weight/Muscle");
	XYfoobar(tex.massBkg, foo.muscleSizePos);
	ImGui::Text("Breast Size/Shape");
	XYfoobar(tex.breastBkg, foo.breastPos);
	ImGui::Text("Bodyshape Shape/Height");
	XYfoobar(tex.shapeBkg, foo.shapePos);

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
		Tile("body_01",  "Torso parameters",         "torso"),
		Tile("body_02",  "Head parameters",          "head"),
		Tile("face_01",  "X",                        ""),
		Tile("face_02",  "X",                        ""),
		Tile("face_03",  "X",                        ""),
		Tile("face_04",  "X",                        ""),
		// ==========================================
		Tile("body_03",  "Shoulders parameters",     "shoulders"),
		Tile("body_04",  "Neck parameters",          "neck"),
		Tile("face_05",  "X",                        ""),
		Tile("face_06",  "Forehead parameters",      "forehead"),
		Tile("face_07",  "X",                        ""),
		Tile("face_08",  "X",                        ""),
		// ==========================================
		Tile("body_05",  "Upper arms parameters",    "upper_arms"),
		Tile("body_06",  "X",                        ""),
		Tile("face_09",  "X",                        ""),
		Tile("face_10",  "Eyes parameters",          "eyes"),
		Tile("face_11",  "X",                        ""),
		Tile("face_12",  "X",                        ""),
		// ==========================================
		Tile("body_07",  "Lower arms parameters",    "lower_arms"),
		Tile("body_08",  "Abdomen parameters",       "abdomen"),
		Tile("face_13",  "Nose parameters",          "nose"),
		Tile("face_14",  "Cheek parameters",         "cheek"),
		Tile("face_15",  "Ears parameters",          "ears"),
		Tile("face_16",  "X",                        ""),
		// ==========================================
		Tile("body_09",  "Hands parameters",         "hands"),
		Tile("body_10",  "Pelvis parameters",        "pelvis"),
		Tile("face_17",  "Mouth parameters",         "mouth"),
		Tile("face_18",  "X",                        ""),
		Tile("face_19",  "X",                        ""),
		Tile("face_20",  "X",                        ""),
		// ==========================================
		Tile("body_11",  "Upper legs parameters",    "upper_legs"),
		Tile("body_12",  "X",                        ""),
		Tile("face_21",  "Chin Jaw parameters",      "chin_jaw"),
		Tile("face_22",  "X",                        ""),
		Tile("face_23",  "X",                        ""),
		Tile("face_24",  "X",                        ""),
		// ==========================================
		Tile("body_13",  "X",                        ""),
		Tile("body_14",  "X",                        ""),
		Tile("hands_01", "Forefinger parameters",    "forefinger"),
		Tile("hands_02", "Middlefinger parameters",  "middlefinger"),
		Tile("hands_03", "Ringfinger parameters",    "ringfinger"),
		Tile("hands_04", "X",                        ""),
		// ==========================================
		Tile("body_15",  "Lower legs parameters",    "lower_legs"),
		Tile("body_16",  "X",                        ""),
		Tile("hands_05", "Pollex parameters",        "pollex"),
		Tile("hands_06", "X",                        ""),
		Tile("hands_07", "Littlefinger parameters",  "littlefinger"),
		Tile("hands_08", "X",                        ""),
		// ==========================================
		Tile("body_17",  "Feet parameters",          "feet"),
		Tile("body_18",  "X",                        ""),
		Tile("hands_09", "X",                        ""),
		Tile("hands_10", "X",     ""),
		Tile("hands_11", "X",     ""),
		Tile("hands_12", "X",     ""),
		// ==========================================
		Tile("teeth_01", "Tongue parameters",        "tongue"),
		Tile("teeth_02", "Teeth parameters",         "teeth"),
		Tile("teeth_03", "X",     ""),
		Tile("teeth_04", "X",     ""),
		Tile("teeth_05", "X",     ""),
		Tile("teeth_06", "X",     ""),
	};
	// clang-format on
};

auto applier = [](const std::string & name, const float & value, const bool deleteOnZero) {
	g_mesh.setMorphTarget(name, value);
	g_mesh.calcNormals();
};

void DisplayMorphTargets()
{
	constexpr static ImGuiWindowFlags winFlags =
	        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(vec2(380, 510));
	if(!ImGui::Begin("Morph Targets", &g_config.morphTargets.visible, winFlags)) {
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

		for(const auto & targetEntry : g_mesh.targets()) {
			const string & target_name(targetEntry.first);

			if(isCompositeMorphTarget(target_name)) {
				continue;
			}

			if(!pathStartsWith(target_name, tileWin.m_category)) {
				continue;
			}

			float target_value = bodyset.m_targets[target_name];


			const auto & minmax = std::make_pair(0, 1);

			DrawTargetRow(g_targetImageTextures, minmax, target_name, target_value,
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

	for(const auto & bodyset_it : g_mesh.morphTargets().m_targets) {

		string target_name(bodyset_it.first);

		if(isCompositeMorphTarget(target_name)) {
			continue;
		}

		float target_value = bodyset_it.second;

		DrawAppliedRow(g_targetImageTextures, std::make_pair(0.f, 1.f), target_name,
		               target_value, applier);
	}
	ImGui::End();
}
