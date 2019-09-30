/* SPDX-License-Identifier: MIT */
#pragma once

namespace mh
{

template <typename TAG, typename T> struct GlType {
	T handle;
	explicit GlType(T handle)
	        : handle(handle)
	{
	}
};

using Texture = GlType<struct Texture_TAG, unsigned int>;
using Shader  = GlType<struct Shader_TAG, unsigned int>;

} // namespace mh
