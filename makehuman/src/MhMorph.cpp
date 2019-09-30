#include "MhMorph.h"

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhGui.h"
#include "MhGuiData.h"

#include "gui/Window.h"
#include "Global.h"
#include "ComponentID.h"


using glm::vec2;


void XYfoobar(mh::Texture texture, glm::vec2 & value)
{
	auto *    dl = ImGui::GetForegroundDrawList();
	ImGuiIO & io = ImGui::GetIO();
	
	MhGui::ImageButton(texture, ImVec2(192, 104));
	vec2 pMin = ImGui::GetItemRectMin();
	vec2 size = ImGui::GetItemRectSize();
	if(ImGui::IsItemActive()) {
		vec2 relPos = vec2(io.MousePos) - pMin;
		relPos       = glm::max(relPos, vec2(0));
		relPos       = glm::min(relPos, size);
		value        = vec2(relPos) / vec2(size);
	}
	
	vec2       cursorPos    = pMin + vec2(value * vec2(size));
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
	
	using OptTex = std::optional<mh::Texture>;
	
	struct Textures {
		OptTex ageGenderBkg  = LoadTextureFromFile("pixmaps/ui/age_selector.png");
		OptTex massBkg = LoadTextureFromFile("pixmaps/ui/muscle_size_selector.png");
		OptTex breastBkg      = LoadTextureFromFile("pixmaps/ui/breast_selector.png");
		OptTex shapeBkg      = LoadTextureFromFile("pixmaps/ui/shape_selector.png");
	};
	
	static const Textures tex;
	
	{
		vec2 foo = vec2(192, 104);
		
		Point * agePos     = g_global.getFuzzyValue(kComponentID_CharacterSettingPanel_Age);
		if(agePos)
			g_global.ageAndSex = vec2(agePos->x, agePos->y) / foo;
		
		Point * mPos = g_global.getFuzzyValue(kComponentID_CharacterSettingPanel_MuscleSize);
		if(mPos)
			g_global.bodyWeightMuscle = vec2(mPos->x, mPos->y) / foo;
		
		Point * bPos = g_global.getFuzzyValue(kComponentID_CharacterSettingPanel_Breast);
		if(bPos)
			g_global.breastSizeShape = vec2(bPos->x, bPos->y) / foo;
		
		Point * sPos = g_global.getFuzzyValue(kComponentID_CharacterSettingPanel_Shape);
		if(sPos)
			g_global.bodyShapeHeight = vec2(sPos->x, sPos->y) / foo;
	}
	
	ImGui::SetNextWindowSize(vec2(220, 800));
	ImGui::Begin("Character Setting");
	
	ImGui::Text("Age/Sex");
	XYfoobar(tex.ageGenderBkg.value(), g_global.ageAndSex);
	ImGui::Text("Bodymass Weight/Muscle");
	XYfoobar(tex.massBkg.value(), g_global.bodyWeightMuscle);
	ImGui::Text("Breast Size/Shape");
	XYfoobar(tex.breastBkg.value(), g_global.breastSizeShape);
	ImGui::Text("Bodyshape Shape/Height");
	XYfoobar(tex.shapeBkg.value(), g_global.bodyShapeHeight);
	
	//	if (ImGui::SliderFloat2("Age/Sex", ageAndSex.data(), 0.0f, 1.0f)) {
	
	//		Point p(ageAndSex[0] * 100, ageAndSex[1] * 100);
	//		characterSettingPanel->m_age->cursorPos = p;
	
	//		characterSettingPanel->selectorListener.ageDists =
	//		    characterSettingPanel->m_age->getDists();
	
	//		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	//	}
	
	//	if (ImGui::SliderFloat2("Weight/Muscle", bodyWeightMuscle.data(), 0.0f,
	//	                        1.0f)) {
	
	//		Point p(bodyWeightMuscle[0] * 100, bodyWeightMuscle[1] * 100);
	//		characterSettingPanel->m_muscleSize->cursorPos = p;
	
	//		characterSettingPanel->selectorListener.muscleSizeDists =
	//		    characterSettingPanel->m_muscleSize->getDists();
	
	//		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	//	}
	
	//	if (ImGui::SliderFloat2("Breast Size / Shape", breastSizeShape.data(), 0.0f,
	//	                        1.0f)) {
	
	//		Point p(breastSizeShape[0] * 100, breastSizeShape[1] * 100);
	//		characterSettingPanel->m_breast->cursorPos = p;
	
	//		characterSettingPanel->selectorListener.breastDists =
	//		    characterSettingPanel->m_breast->getDists();
	
	//		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	//	}
	
	//	if (ImGui::SliderFloat2("Body Shape/Height", bodyShapeHeight.data(), 0.0f,
	//	                        1.0f)) {
	
	//		Point p(bodyShapeHeight[0] * 100, bodyShapeHeight[1] * 100);
	//		characterSettingPanel->m_shape->cursorPos = p;
	
	//		characterSettingPanel->selectorListener.shapeDists =
	//		    characterSettingPanel->m_shape->getDists();
	
	//		characterSettingPanel->selectorListener.calcWidgetTargetsFOO();
	//	}
	
	ImGui::End();
}


void doMorphFromGui(std::string morphTarget, float value)
{
	g_global.mesh->doMorph(morphTarget, value);
	
	//	mesh->doMorph(imgSliderSource->getTargetName(),
	//	              imgSliderSource->getSliderValue());
	
	g_global.mesh->calcNormals();
}

void DisplayMorphTargetRow(const string & target_name, float & target_value, bool xBtn)
{
	fs::path targetImageName = target_name;
	targetImageName.replace_extension();
	
	const auto & texIdIt = g_targetImageTextures.find(targetImageName);
	if(texIdIt != g_targetImageTextures.end()) {
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
	
	if(xBtn) {
		// FIXME only the button in the first line is working
		if(ImGui::Button("X", ImVec2(16, 16))) {
			doMorphFromGui(target_name, 0.f);
		}
		ImGui::SameLine(0, 4);
	}
	
	ImGui::PushItemWidth(-400);
	// TODO used min so that morph does not vanish
	if(ImGui::SliderFloat(target_name.c_str(), &target_value, 0.001f, 1.f)) {
		doMorphFromGui(target_name, target_value);
	}
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

void DisplayMorphTargets()
{
	
//	if(!ImGui::Begin("Morph Targets", &g_displayWin.morphTargets)) {
//		ImGui::End();
//		return;
//	}
	
//	BodySettings bodyset = g_global.mesh->getBodySettings();
	
//	for(const auto & targetEntry : g_global.mesh->getTargetMapRef()) {
//		const string & target_name(targetEntry.first);
		
//		string::size_type loc = target_name.find("/", 0);
//		if(loc == string::npos)
//			continue;
//		else {
//			string sub = target_name.substr(0, loc);
			
//			if(isCompositeMorphTarget(target_name)) {
//				continue;
//			}
			
//			float target_value = bodyset[target_name];
			
//			DisplayMorphTargetRow(target_name, target_value, false);
//		}
//	}
//	ImGui::End();

	using glm::vec2;
	
	constexpr static ImGuiWindowFlags winFlags
	    = ImGuiWindowFlags_NoScrollbar
	    | ImGuiWindowFlags_NoResize;
	
	ImGui::SetNextWindowSize(vec2(380, 484));
	if(!ImGui::Begin("Morph Targets", &g_displayWin.morphTargets, winFlags)) {
		ImGui::End();
		return;
	}
	
	static std::string category = "";
	
	
	struct Tile {
		
		void click() const {
		}
		
		
		std::string as;
		std::string tip;
		std::string target;
		std::optional<mh::Texture> tex;
		std::optional<mh::Texture> texOver;
		Tile(const std::string & img, const std::string & _tip, const std::string & _targ)
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
			if(ImGui::InvisibleButton(as.c_str(), vec2(32, 32))){
				category = target;
				click();
			}
			ImGui::SetCursorPos(p);
			if(ImGui::IsItemHovered()) {
				if(texOver)
					MhGui::Image(texOver.value(), vec2(32, 32));
			} else {
				if(tex)
					MhGui::Image(tex.value(), vec2(32, 32));
			}
			ImGui::NextColumn();
		}
	};

	// clang-format off
	static const auto goobar = {
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
	
	//static_assert (goobar.size() == 84, "asd");
	
	{
		ImGui::BeginChild("Pose Groups", vec2(195, 460), false);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, vec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, vec2(0, 0));
		ImGui::Columns(6, NULL, false);
		for(const auto & tile: goobar) {
			tile.gui();
		}
		ImGui::PopStyleVar(2);
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginChild("Pose Targets", vec2(140, 440), false);
		
		BodySettings bodyset = g_global.mesh->getPoses();
		
		
		
		for(const auto & targetEntry : g_global.mesh->getTargetMapRef()) {
			const string & target_name(targetEntry.first);
	
			string::size_type loc = target_name.find("/", 0);
			if(loc == string::npos)
				continue;
			
			if(isCompositeMorphTarget(target_name)) {
				continue;
			}
			
			string sub = target_name.substr(0, loc);



			float target_value = bodyset[target_name];
			
			
			
			if(sub != category)
				continue;
			
			//ImGuiIO& io = ImGui::GetIO();
			
			
			fs::path targetImageName = target_name;
			targetImageName.replace_extension();
			
			const auto & texIdIt = g_targetImageTextures.find(targetImageName);
			if(texIdIt != g_targetImageTextures.end()) {
				auto texId = texIdIt->second;
				MhGui::ImageButton(texId, ImVec2(64, 64));
			} else {
				ImGui::Text("%s", target_name.c_str());
				//ImGui::InvisibleButton(target_name.c_str(), ImVec2(48, 48));
			}
			
			//DisplayMorphTargetRow(target_name, target_value, false);
		}
		
		
		
		
		ImGui::EndChild();
	}
	ImGui::End();
}

void DisplayMorphTargetsApplied()
{
	if(!ImGui::Begin("Applied Morph Targets", &g_displayWin.morphTargetsApplied)) {
		ImGui::End();
		return;
	}
	
	for(const auto & bodyset_it : g_global.mesh->getBodySettings()) {
		
		string target_name(bodyset_it.first);
		
		if(isCompositeMorphTarget(target_name)) {
			continue;
		}
		
		float target_value = bodyset_it.second;
		
		DisplayMorphTargetRow(target_name, target_value, true);
	}
	ImGui::End();
}
