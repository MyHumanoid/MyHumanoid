/* SPDX-License-Identifier: MIT */
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#define IM_VEC2_CLASS_EXTRA \
	ImVec2(const glm::vec2 & other) \
	{ \
		x = other.x; \
		y = other.y; \
	} \
	operator glm::vec2() const { \
		return glm::vec2(x, y); \
	} \

#define IM_VEC4_CLASS_EXTRA \
	ImVec4(const glm::vec4 & other) \
	{ \
		x = other.x; \
		y = other.y; \
		z = other.z; \
		w = other.w; \
	} \
	operator glm::vec4() const { \
		return glm::vec4(x, y, z, w); \
	} \


#include <imgui.h>
#include <examples/imgui_impl_glut.h>
#include <examples/imgui_impl_opengl3.h>

#include <utility>

#include "render/GlTypes.h"

namespace MhGui
{

template <typename... T> void Image(mh::Texture texture, T &&... p)
{
	ImGui::Image((void *)(intptr_t)texture.handle, std::forward<T>(p)...);
}
template <typename... T> bool ImageButton(mh::Texture texture, T &&... p)
{
	return ImGui::ImageButton((void *)(intptr_t)texture.handle, std::forward<T>(p)...);
}

} // namespace MhGui
