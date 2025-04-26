/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <unordered_map>
#include <optional>
#include <string>

#include <SDL3/SDL.h>

#include "render/GlTypes.h"

using OptTex = std::optional<mh::Texture>;
using IconMap = std::unordered_map<std::string, mh::Texture>;

std::optional<mh::Texture> LoadTextureFromFile(const std::string & file);

void loadTexturesFromDir(IconMap & target, const std::string & baseDir);

class Surface final {
public:
	Surface(SDL_Surface * ptr, unsigned char * m_img);
	unsigned char * m_img;
	SDL_Surface *m_ptr;
	void free();
};
std::optional<Surface> loadSurfaceFromFile(const std::string & fileName);


