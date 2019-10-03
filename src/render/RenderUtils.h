/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <unordered_map>
#include <optional>

#include "render/GlTypes.h"

using IconMap = std::unordered_map<std::string, mh::Texture>;

std::optional<mh::Texture> LoadTextureFromFile(const std::string & file);

void loadTexturesFromDir(IconMap & target, const std::string & baseDir);
