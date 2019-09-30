#ifndef MHUICOMMON_H
#define MHUICOMMON_H

#include <array>
#include <string>
#include <optional>

#include "MhGui.h"

#include "render/RenderUtils.h"

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


#endif // MHUICOMMON_H
