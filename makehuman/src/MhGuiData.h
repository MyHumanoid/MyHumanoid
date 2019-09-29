#pragma once

#include <experimental/filesystem>
#include <optional>
#include <string>
#include <unordered_map>

#include "render/GlTypes.h"

namespace fs = std::experimental::filesystem;

using IconMap = std::unordered_map<std::string, mh::Texture>;

extern IconMap g_targetImageTextures;
extern IconMap g_poseImageTextures;
extern IconMap g_charactersIconTextures;


