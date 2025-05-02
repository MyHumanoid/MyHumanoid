// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2
#pragma once

// TODO is this needed ?
#ifdef _WIN32
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <optional>

#ifdef AGL_USE_GLES
	#include <glad/gles2.h>
#else
	#include <glad/gl.h>
#endif

namespace agl {

template <typename TAG, typename T>
struct GlType {
	T handle;
	explicit GlType(T handle) : handle(handle) {}
};

using Texture = GlType<struct Texture_TAG, unsigned int>;
using Shader  = GlType<struct Shader_TAG, unsigned int>;

bool init();

std::optional<agl::Shader> LoadShader(const char * vertex_path, const char * fragment_path);


}
