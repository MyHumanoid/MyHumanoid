#pragma once

#include <optional>

#include "GL/glew.h"

std::optional<GLuint> LoadShader(const char * vertex_path, const char * fragment_path);
