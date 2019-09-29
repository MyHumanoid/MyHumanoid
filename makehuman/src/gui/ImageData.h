/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
 */
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>

#include <png.h>

#include "Texture.h"

using std::string;

namespace mhgui
{

struct buffer_color_rgba_t {
	uint8_t r, g, b, a;
};
struct buffer_color_rgb_t {
	uint8_t r, g, b;
};

/// Represents a PNG image loaded into memory
class ImageData
{
	friend class Texture;

private:
	char *       image_data; /* raw png image data */
	char *       end_bufferPtr;
	png_uint_32  width, height;
	unsigned int bytesPerRow;
	bool         alpha;

	// Intionally declared as private because not implemented (yet)
private:
	ImageData(const ImageData &);
	ImageData & operator=(const ImageData &);

private:
	ImageData();
	~ImageData();

	bool pngLoad(const string & filename);

	unsigned long getWidth() const;
	unsigned long getHeight() const;
	unsigned int  getBytesPerRow() const { return bytesPerRow; }
	bool          hasAlpha() const;
	const void *  getData() const;

private:
	bool pngLoadPNGLib(const string & filename);
};

} // namespace mhgui
