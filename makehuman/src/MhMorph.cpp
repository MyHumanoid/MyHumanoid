#include "MhMorph.h"

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhGui.h"
#include "MhGuiData.h"

#include "gui/Window.h"
#include "Global.h"
#include "ComponentID.h"
#include "PoseTargetPanel.h"


void XYfoobar(mh::Texture texture, glm::vec2 & value)
{
	
	using glm::vec2;
	using glm::ivec2;
	
	auto *    dl = ImGui::GetForegroundDrawList();
	ImGuiIO & io = ImGui::GetIO();
	
	MhGui::ImageButton(texture, ImVec2(192, 104));
	ivec2 pMin = ImGui::GetItemRectMin();
	ivec2 size = ImGui::GetItemRectSize();
	if(ImGui::IsItemActive()) {
		ivec2 relPos = ivec2(io.MousePos) - pMin;
		relPos       = glm::max(relPos, ivec2(0));
		relPos       = glm::min(relPos, size);
		value        = vec2(relPos) / vec2(size);
	}
	
	ivec2       cursorPos    = pMin + ivec2(value * vec2(size));
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
	using glm::ivec2;
	
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
	
	ImGui::SetNextWindowSize(ivec2(220, 800));
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
	
	if(!ImGui::Begin("Morph Targets", &g_displayWin.morphTargets)) {
		ImGui::End();
		return;
	}
	
	BodySettings bodyset = g_global.mesh->getBodySettings();
	
	for(const auto & targetEntry : g_global.mesh->getTargetMapRef()) {
		const string & target_name(targetEntry.first);
		
		string::size_type loc = target_name.find("/", 0);
		if(loc == string::npos)
			continue;
		else {
			string sub = target_name.substr(0, loc);
			
			if(isCompositeMorphTarget(target_name)) {
				continue;
			}
			
			float target_value = bodyset[target_name];
			
			DisplayMorphTargetRow(target_name, target_value, false);
		}
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
