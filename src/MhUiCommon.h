#ifndef MHUICOMMON_H
#define MHUICOMMON_H

#include <array>
#include <string>
#include <optional>
#include <unordered_map>

#include <glm/glm.hpp>

#include "animorph/PoseTarget.h"
#include "render/RenderUtils.h"
#include "StringUtils.h"

#include "Logger.h"
#include "MhUi.h"
#include "Vfs.h"


using vec2 = glm::vec2;

struct Tile {
	
	struct Interaction {
		int click = -1;
		bool hover = false;
	};

	glm::vec2                  m_size = glm::vec2(32, 32);
	std::string                m_imageBase;
	std::string                m_tooltip;
	std::string                m_category;
	std::optional<mh::Texture> m_tex;
	std::optional<mh::Texture> m_texOver;
	
	Tile(const std::string & img, const std::string & _tip, const std::string & _targ)
	{
		m_tooltip   = _tip;
		m_category  = _targ;
		m_imageBase = "data/pixmaps/ui/" + img;
		m_tex       = LoadTextureFromFile(m_imageBase + ".png");
		m_texOver   = LoadTextureFromFile(m_imageBase + "_over.png");
	}

	Interaction gui() const
	{
		Interaction interaction;
		
		auto p = ImGui::GetCursorPos();
		if(ImGui::InvisibleButton(m_imageBase.c_str(), m_size)) {
			interaction.click = 0;
		}
		if(ImGui::IsItemClicked(1)) {
			interaction.click = 1;
		}
		ImGui::SetCursorPos(p);
		if(ImGui::IsItemHovered()) {
			if(m_texOver) {
				MhGui::Image(m_texOver.value(), m_size);
				interaction.hover = true;
			}
		} else {
			if(m_tex)
				MhGui::Image(m_tex.value(), m_size);
		}
		return interaction;
	}
};

template <typename Derived>
struct TileGroupChildWindow {

	std::string m_category;
	std::string m_categoryRight;
	std::string m_tooltip;

	void DisplayGroupTiles()
	{
		using glm::ivec2;
		
		auto tileSize = ivec2(32, 32);
		
		const Derived & d = static_cast<Derived &>(*this);
		
		int columns = 6;
		int rows = d.tiles.size() / 6;
		
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, vec2(0));
		
		auto winSize = ivec2((tileSize.x * columns) + 5, (tileSize.y * rows) + 12);
		
		ImGui::BeginChild("Pose Groups", vec2(winSize), false);
		
		auto tableFlags = ImGuiTableFlags_NoSavedSettings
		                | ImGuiTableFlags_SizingFixedFit;
		
		if(ImGui::BeginTable("Pose Groups Table", columns, tableFlags)) {

			for(const auto & tile : d.tiles) {
				ImGui::TableNextColumn();
				
				Tile::Interaction interaction = tile.gui();
	
				if(interaction.click == 0) {
					m_category = tile.m_category;
				}
				if(interaction.click == 1) {
					log_info("Down right");
					m_categoryRight = tile.m_category;
				}
				if(interaction.hover) {
					m_tooltip = tile.m_tooltip;
				}
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();
		ImGui::PopStyleVar(1);
	}
};

inline bool pathStartsWith(const std::string & path, const std::string prefix)
{

	string::size_type loc = path.find("/", 0);
	if(loc == string::npos)
		return false;

	string sub = path.substr(0, loc);

	return sub == prefix;
}


static float poseTargetDragStartValue = 0;

using OptText = std::optional<mh::Texture>;
using TexPair = std::pair<OptText, OptText>;

inline TexPair getImage(const IconMap & icons, const std::string & name)
{
	const auto & t = icons.find(name);
	if(t != icons.end()) {
		const auto & over = icons.find(name + "_over");
		if(over != icons.end()) {
			return std::make_pair(t->second, over->second);
		} else {
			return std::make_pair(t->second, std::nullopt);
		}
	}
	return std::make_pair(std::nullopt, std::nullopt);
};


template <typename Applier>
void DrawTargetRow(const IconMap & icons, const std::pair<float, float> minMax,
                   const std::string & target_name, const float & target_value,
                   std::string & tooltip, Applier && applier)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, vec2(0));

	ImGuiIO & io = ImGui::GetIO();
	
	std::string targetImageName = removeExtension(target_name);

	const auto iconSize = vec2(64, 64);

	const auto p = ImGui::GetCursorPos();
	ImGui::InvisibleButton(target_name.c_str(), iconSize);
	if(ImGui::IsMouseDown(0)) {
		// poseTargetDragStartValue = target_value;
	};
	bool active  = ImGui::IsItemActive();
	bool hovered = ImGui::IsItemHovered();
	if(active) {
		vec2 delta = vec2(io.MousePos) - vec2(io.MouseClickedPos[0]);

		float scaled = glm::mix(minMax.first, minMax.second, delta.x / 200);

		// auto scaled = deltaX;// * posToValFactor;
		auto absVal = poseTargetDragStartValue + scaled;

		float foo = glm::clamp(absVal, minMax.first, minMax.second);

		applier(target_name, foo, true);

		ImGui::GetForegroundDrawList()->AddLine(
		        io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button),
		        4.0f); // Draw a line between the button and the mouse cursor
	}
	auto icon = getImage(icons, targetImageName);
	if(hovered || active) {
		tooltip = target_name;
		if(icon.second) {
			ImGui::SetCursorPos(p);
			MhGui::ImageButton((target_name + "a").c_str(), *icon.second, iconSize);
		} else {
			ImGui::SetCursorPos(p);
			MhGui::ImageButton((target_name + "b").c_str(), *icon.first, iconSize);
		}
	} else {
		if(icon.first) {
			ImGui::SetCursorPos(p);
			MhGui::ImageButton((target_name + "c").c_str(), *icon.first, iconSize);
		}
	}
	ImGui::SameLine();
	ImGui::Text("%.2f", target_value);

	ImGui::PopStyleVar();
}

template <typename Applier>
void DrawAppliedRow(const IconMap & icons, const std::pair<float, float> minMax,
                    const std::string & name, const float & value, Applier && applier)
{
	bool showTooltip = false;
	
	std::string targetImageName = removeExtension(name);

	const auto & texIdIt     = icons.find(targetImageName);
	bool         haveTexture = texIdIt != icons.end();
	if(haveTexture) {
		MhGui::Image(texIdIt->second, ImVec2(16, 16));
		showTooltip |= ImGui::IsItemHovered();
	} else {
		ImGui::Dummy(ImVec2(16, 16));
	}
	ImGui::SameLine();

	ImGui::PushID(name.c_str());
	if(ImGui::Button("X")) {
		applier(name, 0.f, true);
	}
	ImGui::PopID();
	
	showTooltip |= ImGui::IsItemHovered();
	ImGui::SameLine();

	float val = value;
	float min = minMax.first;
	float max = minMax.second;
	if(ImGui::SliderFloat(name.c_str(), &val, min, max)) {
		applier(name, val, false);
	}
	showTooltip |= ImGui::IsItemHovered();

	if(showTooltip && haveTexture) {
		ImGui::BeginTooltip();
		MhGui::Image(texIdIt->second, ImVec2(128, 128));
		ImGui::EndTooltip();
	}
}

#endif // MHUICOMMON_H
