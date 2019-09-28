/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <optional>

#include "render/GlTypes.h"

std::optional<mh::Texture> LoadTextureFromFile(const char* filename);
