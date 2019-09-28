/* SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <GL/glew.h>

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
