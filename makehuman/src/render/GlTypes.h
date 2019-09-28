/* SPDX-License-Identifier: MIT */
#pragma once

#include <GL/glew.h>

namespace mh {

template <typename TAG, typename T>
struct GlType {
	T handle;
	explicit GlType(T handle)
	    : handle(handle)
	{}
};

using Texture = GlType<struct Texture_TAG, GLuint>;

}
