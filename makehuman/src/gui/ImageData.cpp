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

namespace mhgui {

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ImageData::ImageData ()
    : image_data(NULL), end_bufferPtr(NULL),
    width(0), height(0),
    bytesPerRow(0),
    alpha(false)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ImageData::~ImageData ()
{
  delete [] image_data;
  image_data = NULL;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
unsigned long ImageData::getWidth () const
{
  return width;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
unsigned long ImageData::getHeight () const
{
  return height;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const void* ImageData::getData () const
{
  return image_data;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool ImageData::hasAlpha () const
{
  return alpha;
}

/** Load a given png file (OS even loads a couple of additional filetypes jpg,
 *  tiff, gif, bmp, psd ...)
 *
 * @param filename The name of the PNG file to load to.
 * @return true if successful, false otherwise
 */
bool ImageData::pngLoad (const string& filename)
{

#if defined(USE_PNGLIB)
  return pngLoadPNGLib(filename);
#elif defined(__APPLE__) && defined(__MACH__)
  return pngLoadOSX(filename);
#endif
}

#ifdef USE_OSX_COREGRAPHICS

/* ========================================================================== */
/** Determine the size of a given file.
 *
 * @param inFilename The name of the file the size needs to be determined.
 * @return the size of the file in bytes or -1 if the given file does not exists.
 */
/* ========================================================================== */
static off_t getFileSize(const string& inFilename)
{
  struct stat fileStat;
  return (::stat(inFilename.c_str(), &fileStat) == 0) ? fileStat.st_size : -1LL;
}

/* ========================================================================== */
/** Load a given graphics file and prepare it to be useable as a OpenGL Texture.
 *  This particular implementation uses the Quartz Graphic Engine of OS X to load
 *  the image rather than the pnglib.
 *
 * @filename The name of the Image file to load (full qualified)
 * @return 0 on success 1 on error;
 */
/* ========================================================================== */
bool ImageData::pngLoadOSX(const string& filename)
{
  // clear the image Data buffer first
  delete [] image_data;
  image_data = NULL;

  ssize_t fileSize = getFileSize(filename);
  if (fileSize < 0)
    return false; // file does not exists

  int fileHd = ::open(filename.c_str(), O_RDONLY);
  if (fileHd < 0)
    return false; // file is not readable

  uint8_t *rdDataBuffer = new(std::nothrow) uint8_t[fileSize];
  if (rdDataBuffer != NULL)
  {

    uint8_t *rdPtr = rdDataBuffer;
    size_t remainingSize = fileSize;
    for(;;)
    {
      ssize_t rdSize = ::read(fileHd, rdPtr, remainingSize);
      remainingSize -= rdSize;
      if (remainingSize <= 0)
        break;
      rdPtr += rdSize;
    }
    ::close(fileHd);

    CGDataProviderRef source = CGDataProviderCreateWithData(NULL, rdDataBuffer, fileSize, NULL);
    assert(source);
    if (source)
    {
      CGImageRef imageRef = ::CGImageCreateWithPNGDataProvider(source, NULL, false, kCGRenderingIntentAbsoluteColorimetric);
      assert(imageRef);
      if (imageRef)
      {

        width  = ::CGImageGetWidth (imageRef);
        height = ::CGImageGetHeight(imageRef);
        CGImageAlphaInfo alphaInfo = ::CGImageGetAlphaInfo(imageRef);

        alpha =  (alphaInfo == kCGImageAlphaFirst ||
                  alphaInfo == kCGImageAlphaLast  ||
                  alphaInfo == kCGImageAlphaOnly  ||
                  alphaInfo == kCGImageAlphaPremultipliedFirst  ||
                  alphaInfo == kCGImageAlphaPremultipliedLast);

        // Now create a Bitmap Context prepare the params

        picture_params_t picBufferParams(width,
                                         height,
                                         true); // true because Quartz ALWAYS req. alpha!

        bytesPerRow = picBufferParams.bytesPerRow;
        image_data = new(std::nothrow) char[picBufferParams.calcMemSize()];
        assert(image_data);

        static CGColorSpaceRef sColorSpace = NULL;

        if (sColorSpace==NULL)
        {
          sColorSpace = ::CGColorSpaceCreateDeviceRGB();
          assert(sColorSpace);
        }

        CGContextRef cgContext = ::CGBitmapContextCreate(image_data,
                                                         picBufferParams.width,
                                                         picBufferParams.height,
                                                         picBufferParams.bitsPerComponent,
                                                         picBufferParams.bytesPerRow,
                                                         sColorSpace,
                                                         kCGImageAlphaPremultipliedLast);
        assert(cgContext);
        if (cgContext)
        {
          CGRect destRect={{0,0},{width, height}};

          ::CGContextDrawImage(cgContext, destRect, imageRef);

          if (alpha == false)
          {
            picture_params_t newParams(picture_params_t(picBufferParams.width,
                                                        picBufferParams.height,
                                                        false // false because NO alpha
                                                        ));
            char *newBuffer = new(std::nothrow) char[newParams.calcMemSize()];
            assert(newBuffer);
            buffer_color_rgba_t *srcBuff = (buffer_color_rgba_t*)image_data;
            buffer_color_rgb_t  *dstBuff = (buffer_color_rgb_t*) newBuffer;
            for (uint32_t y=0; y<(height); ++y)
            {
              for (uint32_t x=0; x<(width); ++x)
              {
                dstBuff->r = srcBuff->r;
                dstBuff->g = srcBuff->g;
                dstBuff->b = srcBuff->b;
                dstBuff++; srcBuff++;
              }
            }
            // Assign the new bufer
            delete [] image_data;
            bytesPerRow = newParams.bytesPerRow;
            image_data = newBuffer;
          }
          else // We have an alpha channel!
          {

            /* CGBitmapContextCreate does not accept Colorspaces with non premultiplied colors!
             * Hence we allocate a graphicscontext supposed to hold the alphachannel only.
             * Afterwards just copy this channel into the appropriate alpha component.
             */

            size_t bitsPerComponent   = 8;
            size_t bitsPerPixel       = bitsPerComponent * 1; // Just Alpha
            size_t bytesPerRow        = ((bitsPerPixel * width) + 7) / 8;
            size_t alphaBufferSize    = bytesPerRow*height;

            uint8_t * alphaMap = new uint8_t[alphaBufferSize];
            bzero(alphaMap, alphaBufferSize);
            CGContextRef cgAlphaContext = ::CGBitmapContextCreate(alphaMap,
                                                                  width, height,
                                                                  bitsPerComponent,
                                                                  bytesPerRow,
                                                                  NULL,
                                                                  kCGImageAlphaOnly);
            assert(cgAlphaContext);
            if (cgAlphaContext)
            {
              ::CGContextDrawImage(cgAlphaContext, destRect,imageRef);

              buffer_color_rgba_t *alphaPtr1 = (buffer_color_rgba_t*)image_data;
              uint8_t* alphaPtr              = alphaMap;
              for (uint32_t y=0; y<height; ++y)
              {
                // copy the alpha information into the composit array
                for (uint32_t x=0; x<width; ++x)
                  alphaPtr1[x].a = alphaPtr[x];

                alphaPtr  += width;
                alphaPtr1 += width;
              }
            }
            delete[] alphaMap;
            ::CGContextRelease(cgAlphaContext);
          } // if alpha
          ::CGContextRelease(cgContext);
        }
        ::CGImageRelease(imageRef);
      }
      ::CGDataProviderRelease(source);
    }
    delete [] rdDataBuffer;
  }

  end_bufferPtr = &image_data[(bytesPerRow * height)-1];
  return true; // okay
}

/* ========================================================================== */
/** Calculate the picture parameters supposed to hold in a memory buffer for
 *  a particular size and Color Model (alpha or not).
 *
 * Actually this stucture is used internally only!
 *
 * @param inWidth    The width of the picture the characteristics should
 *                   calculated for.
 * @param inHeight   The height of the picture the characteristics should
 *                   calculated for.
 * @param inHasAlpha Set this to true if the picture expected to load has
 *                   an alpha channel.
 *
 */
/* ========================================================================== */
picture_params_t::picture_params_t(size_t inWidth, size_t inHeight, bool inHasAlpha)
: width(inWidth), height(inHeight), hasAlpha(inHasAlpha),
  bitsPerComponent(8), // every component has 8 bits (GLuint)
  bitsPerPixel(bitsPerComponent * (inHasAlpha ? 4:3)),
  bytesPerRow(((bitsPerPixel * inWidth) + 7) >> 3) // align to multiple of 8!
{
}

#else // ! USE_OSX_COREGRAPHICS

bool ImageData::pngLoadPNGLib (const string& filename)
{
  FILE         *infile;         /* PNG file pointer */
  png_structp   png_ptr;        /* internally used by libpng */
  png_infop     info_ptr;       /* user requested transforms */

  char         sig[8];           /* PNG signature array */

  int           bit_depth;
  int           color_type;

  unsigned int i;
  png_bytepp row_pointers = NULL;

  // clear the image Data buffer first
  delete [] image_data;
  image_data = NULL;

  /* Open the file. */
  infile = fopen(filename.c_str (), "rb");
  if (!infile)
  {
    return false; // indicate an error
  }

  /*
  * 		13.3 readpng_init()
  */

  /* Check for the 8-byte signature */
  fread(sig, 1, 8, infile);

  if (!png_check_sig((unsigned char *) sig, 8))
  {
    fclose(infile);
    return false; // seems not to be a valid png file
  }

  /*
  * Set up the PNG structs
  */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
  {
    fclose(infile);
    return false; // out of memory
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
    fclose(infile);
    return false; // out of memory
  }

  /*
  * block to handle libpng errors,
  * then check whether the PNG file had a bKGD chunk
  */
  if (setjmp(png_jmpbuf(png_ptr)))
  {
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

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
               &color_type, NULL, NULL, NULL);

  // Set up some transforms.
  if (color_type & PNG_COLOR_MASK_ALPHA)
  {
    alpha = true;
    //png_set_strip_alpha(png_ptr);
  }
  if (bit_depth > 8)
  {
    png_set_strip_16(png_ptr);
  }
  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  {
    png_set_gray_to_rgb(png_ptr);
  }
  if (color_type == PNG_COLOR_TYPE_PALETTE)
  {
    png_set_palette_to_rgb(png_ptr);
  }

  //  Update the png info struct
  png_read_update_info(png_ptr, info_ptr);

  // Rowsize in bytes.
  bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);

  // Allocate the image_data buffer.
  if ((image_data = (char *) new (std::nothrow) char [bytesPerRow * height]) == NULL)
  {
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
    return false; // could not get memory
  }

  if ((row_pointers = (png_bytepp) new (std::nothrow) char [height*sizeof(png_bytep)]) == NULL)
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    delete [] image_data;
    image_data = NULL;
    return false; // could not get memory
  }

  // set the individual row_pointers to point at the correct offsets

  //cerr << "height: " << height << endl;

  for (i = 0;  i < height; ++i)
    row_pointers[i] = (png_byte*) image_data + i*bytesPerRow;

  // now we can go ahead and just read the whole image
  png_read_image (png_ptr, row_pointers);

  /* and we're done!  (png_read_end() can be omitted if no processing of
  * post-IDAT text/time/etc. is desired) */

  delete [] row_pointers;

  /* Clean up. */
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose (infile);

  return true; // okay
}
#endif // USE_OSX_COREGRAPHICS

} // namespace mhgui

