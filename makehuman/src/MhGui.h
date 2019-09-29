/* SPDX-License-Identifier: MIT */
#pragma once

#include <glm/vec2.hpp>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#define IM_VEC2_CLASS_EXTRA                                                              \
	ImVec2(const glm::ivec2 & other)                                                     \
	{                                                                                    \
		x = other.x;                                                                     \
		y = other.y;                                                                     \
	}                                                                                    \
	operator glm::ivec2() const {                                                        \
		return glm::ivec2(x, y);                                                         \
	}                                                                                    \

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
