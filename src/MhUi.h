/* SPDX-License-Identifier: MIT */
#pragma once

#include <utility>
#include <string>
#include <unordered_map>

#include "adapter/imgui_adapter.h"
#include "AbstractGl.h"

using IconMap = std::unordered_map<std::string, agl::Texture>;

namespace MhGui
{

template <typename... T> void Image(agl::Texture texture, T &&... p)
{
	ImGui::Image((intptr_t)texture.handle, std::forward<T>(p)...);
}
template <typename... T> bool ImageButton(const char* str_id, agl::Texture texture, T &&... p)
{
	return ImGui::ImageButton(str_id, (intptr_t)texture.handle, std::forward<T>(p)...);
}

} // namespace MhGui
