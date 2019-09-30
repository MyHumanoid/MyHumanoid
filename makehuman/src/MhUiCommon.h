#ifndef MHUICOMMON_H
#define MHUICOMMON_H

#include <array>
#include <string>
#include <optional>
#include <experimental/filesystem>

#include "MhGui.h"
#include "MhGuiData.h"

#include "render/RenderUtils.h"

#include "animorph/PoseTarget.h"


using vec2 = glm::vec2;

//template <typename V, typename... T>
//constexpr auto array_of(T&&... t) -> std::array<V, sizeof...(T)>
//{
//	return {{ std::forward<T>(t)... }};
//}

struct Tile {
	
	void click() const {
	}
	
	glm::vec2 tileSize = glm::vec2(32, 32);
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
	
	void gui(std::string & category, std::string & tooltip) const {
		auto p = ImGui::GetCursorPos();
		if(ImGui::InvisibleButton(as.c_str(), tileSize)){
			category = target;
			//poseTargetCategory = target;
			click();
		}
		ImGui::SetCursorPos(p);
		if(ImGui::IsItemHovered()) {
			if(texOver) {
				MhGui::Image(texOver.value(), tileSize);
				tooltip = tip;
				//poseTargetTooltip = tip;
			}
		} else {
			if(tex)
				MhGui::Image(tex.value(), tileSize, vec2(0), vec2(1));
		}
	}
};

template <typename Derived>
struct TileGroupChildWindow {
	
	std::string poseTargetCategory;
	std::string poseTargetTooltip;
	
	void DisplayGroupTiles() {
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, vec2(0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, vec2(0));
		ImGui::BeginChild("Pose Groups", vec2(192 + 5, 460), false);
		ImGui::Columns(6, "Pose Groups Grid", false);
		
		// FIXME we still have black vertical lines ?!
		// Maybe a ImGui ClipRect bug ?
		// Wait for new ImGui Column Api/Impl ?
		for(int i=0; i<6;++i) {
			ImGui::SetColumnWidth(i, 33);
		}
		
		const Derived & d = static_cast<Derived&>(*this);
		for(const auto & tile: d.tiles) {
			tile.gui(poseTargetCategory, poseTargetTooltip);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::PopStyleVar(3);
	}
};

namespace fs = std::experimental::filesystem;


template <typename Applier>
void DrawAppliedRow(const IconMap & icons,
                    const std::pair<float, float> minMax,
                    const std::string & target_name,
                    const float & target_value,
                    Applier && applier)
{
	bool showTooltip = false;
	
	fs::path targetImageName = target_name;
	targetImageName.replace_extension();
	
	const auto & texIdIt = icons.find(targetImageName);
	bool haveTexture = texIdIt != icons.end();
	if(haveTexture) {
		MhGui::Image(texIdIt->second, ImVec2(16, 16));
		showTooltip |= ImGui::IsItemHovered();
	} else {
		ImGui::Dummy(ImVec2(16, 16));
	}
	ImGui::SameLine();
	
	// FIXME only the button in the first line is working
	if(ImGui::Button("X")) {
		applier(target_name, 0.f, true);
	}
	showTooltip |= ImGui::IsItemHovered();
	ImGui::SameLine();
	
	float val = target_value;
	float min = minMax.first;
	float max = minMax.second;
	if(ImGui::SliderFloat(target_name.c_str(), &val, min, max)) {
		applier(target_name, val, false);
	}
	showTooltip |= ImGui::IsItemHovered();
	
	if(showTooltip && haveTexture) {
		ImGui::BeginTooltip();
		MhGui::Image(texIdIt->second, ImVec2(128, 128));
		ImGui::EndTooltip();
	}
}

#endif // MHUICOMMON_H
