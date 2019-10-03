#pragma once

#include <optional>

#include "render/GlTypes.h"

std::optional<mh::Shader> LoadShader(const char * vertex_path, const char * fragment_path);
