#include "MhMorph.h"

#include <glm/glm.hpp>

#include "render/RenderUtils.h"

#include "MhGui.h"
#include "MhGuiData.h"

#include "gui/Window.h"
#include "Global.h"
#include "ComponentID.h"
#include "PoseTargetPanel.h"


void doMorphFromGui(std::string morphTarget, float value)
{
	Mesh * mesh = g_global.getMesh();
	
	mesh->doMorph(morphTarget, value);
	
	//	mesh->doMorph(imgSliderSource->getTargetName(),
	//	              imgSliderSource->getSliderValue());
	
	mesh->calcNormals();
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
	
	BodySettings bodyset = g_global.getMesh()->getBodySettings();
	
	for(const auto & targetEntry : g_global.getMesh()->getTargetMapRef()) {
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
	
	Mesh * mesh = g_global.getMesh();
	assert(mesh);
	
	for(const auto & bodyset_it : mesh->getBodySettings()) {
		
		string target_name(bodyset_it.first);
		
		if(isCompositeMorphTarget(target_name)) {
			continue;
		}
		
		float target_value = bodyset_it.second;
		
		DisplayMorphTargetRow(target_name, target_value, true);
	}
	ImGui::End();
}
