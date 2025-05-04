// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2
#pragma once

#ifdef IMGUI_VERSION
#error "Imgui Already included"
#endif

#define IM_VEC2_CLASS_EXTRA \
constexpr ImVec2(const glm::vec2 & other) : x(other.x), y(other.y) { } \
constexpr operator glm::vec2() const { return glm::vec2(x, y); }

#define IM_VEC4_CLASS_EXTRA \
constexpr ImVec4(const glm::vec4 & other) : x(other.x), y(other.y), z(other.z), w(other.w) { } \
constexpr operator glm::vec4() const { return glm::vec4(x, y, z, w); }

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>

#include <type_traits>

static_assert(sizeof(glm::vec2) == sizeof(ImVec2));
static_assert(sizeof(glm::vec4) == sizeof(ImVec4));

static_assert(std::is_convertible_v<glm::vec2, ImVec2>);
static_assert(std::is_convertible_v<ImVec2, glm::vec2>);
static_assert(std::is_convertible_v<glm::vec4, ImVec4>);
static_assert(std::is_convertible_v<ImVec4, glm::vec4>);
