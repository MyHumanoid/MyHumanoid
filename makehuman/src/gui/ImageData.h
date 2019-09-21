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
 *  File   : ImageData.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef IMAGEDATA_H
#define IMAGEDATA_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <stdlib.h>
#include <stdint.h>

#include "Texture.h"

#define USE_PNGLIB

#if defined(USE_PNGLIB)
  #include <png.h>
#endif

using std::string;

namespace mhgui {

struct buffer_color_rgba_t {uint8_t r,g,b,a;};
struct buffer_color_rgb_t  {uint8_t r,g,b;};

/// Represents a PNG image loaded into memory
class ImageData
{
  friend class Texture;
private:
  char         *image_data; /* raw png image data */
  char         *end_bufferPtr;
  png_uint_32 width, height;
  unsigned int  bytesPerRow;
  bool          alpha;

// Intionally declared as private because not implemented (yet)
private:
   ImageData(const ImageData&);
   ImageData& operator=(const ImageData&);

private:
   ImageData ();
  ~ImageData ();

  bool pngLoad             (const string& filename);

  unsigned long getWidth  () const;
  unsigned long getHeight () const;
  unsigned int  getBytesPerRow() const {return bytesPerRow;}
  bool          hasAlpha  () const;
  const void*   getData   () const;

private:
    bool pngLoadPNGLib(const string& filename);
};

} // namespace mhgui

#endif //IMAGEDATA_H
