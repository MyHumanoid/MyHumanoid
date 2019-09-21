/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File   : ImageData.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/ImageData.h"
#include <iostream>
#include <sys/stat.h>

using namespace std;

using namespace std;

namespace mhgui
{

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ImageData::ImageData()
    : image_data(NULL)
    , end_bufferPtr(NULL)
    , width(0)
    , height(0)
    , bytesPerRow(0)
    , alpha(false)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ImageData::~ImageData()
{
	delete[] image_data;
	image_data = NULL;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
unsigned long ImageData::getWidth() const { return width; }

/* ========================================================================== */
/**
 */
/* ========================================================================== */
unsigned long ImageData::getHeight() const { return height; }

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const void *ImageData::getData() const { return image_data; }

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool ImageData::hasAlpha() const { return alpha; }

/** Load a given png file (OS even loads a couple of additional filetypes jpg,
 *  tiff, gif, bmp, psd ...)
 *
 * @param filename The name of the PNG file to load to.
 * @return true if successful, false otherwise
 */
bool ImageData::pngLoad(const string &filename)
{
	return pngLoadPNGLib(filename);
}

bool ImageData::pngLoadPNGLib(const string &filename)
{
	FILE *infile;        /* PNG file pointer */
	png_structp png_ptr; /* internally used by libpng */
	png_infop info_ptr;  /* user requested transforms */

	char sig[8]; /* PNG signature array */

	int bit_depth;
	int color_type;

	unsigned int i;
	png_bytepp row_pointers = NULL;

	// clear the image Data buffer first
	delete[] image_data;
	image_data = NULL;

	/* Open the file. */
	infile = fopen(filename.c_str(), "rb");
	if (!infile) {
		return false; // indicate an error
	}

	/*
	 * 		13.3 readpng_init()
	 */

	/* Check for the 8-byte signature */
	fread(sig, 1, 8, infile);

	if (!png_check_sig((unsigned char *)sig, 8)) {
		fclose(infile);
		return false; // seems not to be a valid png file
	}

	/*
	 * Set up the PNG structs
	 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(infile);
		return false; // out of memory
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(infile);
		return false; // out of memory
	}

	/*
	 * block to handle libpng errors,
	 * then check whether the PNG file had a bKGD chunk
	 */
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(infile);
		return false; // could not install error handler
	}

	/*
	 * takes our file stream pointer (infile) and
	 * stores it in the png_ptr struct for later use.
	 */
	/* png_ptr->io_ptr = (png_voidp)infile;*/
	png_init_io(png_ptr, infile);

	/*
	 * lets libpng know that we already checked the 8
	 * signature bytes, so it should not expect to find
	 * them at the current file pointer location
	 */
	png_set_sig_bytes(png_ptr, 8);

	/* Read the image info.*/

	/*
	 * reads and processes not only the PNG file's IHDR chunk
	 * but also any other chunks up to the first IDAT
	 * (i.e., everything before the image data).
	 */

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	             NULL, NULL, NULL);

	// Set up some transforms.
	if (color_type & PNG_COLOR_MASK_ALPHA) {
		alpha = true;
		// png_set_strip_alpha(png_ptr);
	}
	if (bit_depth > 8) {
		png_set_strip_16(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_GRAY ||
	    color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}

	//  Update the png info struct
	png_read_update_info(png_ptr, info_ptr);

	// Rowsize in bytes.
	bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data buffer.
	if ((image_data = (char *)new (std::nothrow) char[bytesPerRow * height]) ==
	    NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false; // could not get memory
	}

	if ((row_pointers = (png_bytepp) new (
	         std::nothrow) char[height * sizeof(png_bytep)]) == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete[] image_data;
		image_data = NULL;
		return false; // could not get memory
	}

	// set the individual row_pointers to point at the correct offsets

	// cerr << "height: " << height << endl;

	for (i = 0; i < height; ++i)
		row_pointers[i] = (png_byte *)image_data + i * bytesPerRow;

	// now we can go ahead and just read the whole image
	png_read_image(png_ptr, row_pointers);

	/* and we're done!  (png_read_end() can be omitted if no processing of
	 * post-IDAT text/time/etc. is desired) */

	delete[] row_pointers;

	/* Clean up. */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(infile);

	return true; // okay
}

} // namespace mhgui
