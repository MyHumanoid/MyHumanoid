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
 *  File   : Texture.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Texture.h"
#include "gui/ImageData.h"
#include "gui/Rect.h"
#include "gui/Size.h"

#include <assert.h>
#include <stdint.h>

namespace mhgui
{

/* ========================================================================== */
/** This class is used internally only and supposed to hold the caracteristics
 *  of a particular Texture fragment.
 */
/* ========================================================================== */
class TextureCharacteristics
{
public:
	TextureCharacteristics(const Size & inSize, const glm::ivec2 & inOffset,
	                       const buffer_color_rgb_t * inDataBuffer, size_t inBytesPerRow,
	                       GLuint inGLTextureId);

	TextureCharacteristics(const Size & inSize, const glm::ivec2 & inOffset,
	                       const buffer_color_rgba_t * inDataBuffer, size_t inBytesPerRow,
	                       GLuint inGLTextureId);

	~TextureCharacteristics();

private:
	bool initData(const buffer_color_rgba_t * inSrcPixelData, size_t inBytesPerRow);

	bool initData(const buffer_color_rgb_t * inSrcPixelData, size_t inBytesPerRow);

public:
	// the data members
	Size   mSize;
	glm::ivec2  mOffset;
	GLuint mTextureId;
}; // class TextureCharacteristics

/* ========================================================================== */
/**
 */
/* ========================================================================== */
TextureCharacteristics::TextureCharacteristics(const Size & inSize, const glm::ivec2 & inOffset,
                                               const buffer_color_rgba_t * inDataBuffer,
                                               size_t inBytesPerRow, GLuint inGLTextureId)
        : mSize(inSize)
        , mOffset(inOffset)
        , mTextureId(inGLTextureId)
{
	initData(inDataBuffer, inBytesPerRow);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
TextureCharacteristics::TextureCharacteristics(const Size & inSize, const glm::ivec2 & inOffset,
                                               const buffer_color_rgb_t * inDataBuffer,
                                               size_t inBytesPerRow, GLuint inGLTextureId)
        : mSize(inSize)
        , mOffset(inOffset)
        , mTextureId(inGLTextureId)
{
	initData(inDataBuffer, inBytesPerRow);
}

/* ========================================================================== */
/** Initialize a Texture Characteristics Object based on a given RGBA Data
 * Buffer.
 *
 * @param inDataBuffer The pointer to the source Image which is organized as
 *                     RGBA
 * @param inBytesPerRow The number of Bytes per Row inDataBuffer is organized.
 * @return true for success, false otherwise.
 */
/* ========================================================================== */
bool TextureCharacteristics::initData(const buffer_color_rgba_t * inDataBuffer,
                                      size_t                      inBytesPerRow)
{
	size_t                bufferSize = (mSize.getWidth() * mSize.getHeight());
	buffer_color_rgba_t * destBuffer = new(std::nothrow) buffer_color_rgba_t[bufferSize];
	if(destBuffer == NULL)
		return false;

	size_t offset =
	        (mOffset.y * inBytesPerRow) + (mOffset.x * sizeof(buffer_color_rgba_t));
	buffer_color_rgba_t * srcPtr = (buffer_color_rgba_t *)&((const char *)inDataBuffer)[offset];
	buffer_color_rgba_t * dstPtr = destBuffer;

	// now copy the data
	for(int y = 0; y < mSize.getHeight(); ++y) {
		for(int x = 0; x < mSize.getWidth(); ++x) {
			dstPtr->r = srcPtr[x].r;
			dstPtr->g = srcPtr[x].g;
			dstPtr->b = srcPtr[x].b;
			dstPtr->a = srcPtr[x].a;
			++dstPtr;
		}
		srcPtr = (buffer_color_rgba_t *)&((char *)srcPtr)[inBytesPerRow];
	}

	/* create a new texture object
	 * and bind it to texname (unsigned integer > 0)
	 */
	::glBindTexture(GL_TEXTURE_2D, mTextureId);
	::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	::glTexImage2D(GL_TEXTURE_2D, 0,
	               4, // Because four components (R, G, B, A)
	               mSize.getWidth(), mSize.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
	               destBuffer);

	delete[] destBuffer;
	return true; // success
}

/* ========================================================================== */
/** Initialize a Texture Characteristics Object based on a given RGB Data
 * Buffer.
 *
 * @param inDataBuffer The pointer to the source Image which is organized as
 *                     RGB
 * @param inBytesPerRow The number of Bytes per Row inDataBuffer is organized.
 * @return true for success, false otherwise.
 */
/* ========================================================================== */
bool TextureCharacteristics::initData(const buffer_color_rgb_t * inDataBuffer, size_t inBytesPerRow)
{
	size_t               bufferSize = (mSize.getWidth() * mSize.getHeight());
	buffer_color_rgb_t * destBuffer = new(std::nothrow) buffer_color_rgb_t[bufferSize];
	if(destBuffer == NULL)
		return false;

	size_t offset =
	        (mOffset.y * inBytesPerRow) + (mOffset.x * sizeof(buffer_color_rgb_t));
	const buffer_color_rgb_t * srcPtr =
	        (const buffer_color_rgb_t *)&((const char *)inDataBuffer)[offset];
	buffer_color_rgb_t * dstPtr = destBuffer;

	// now copy the data
	for(int y = 0; y < mSize.getHeight(); ++y) {
		for(int x = 0; x < mSize.getWidth(); ++x) {
			dstPtr->r = srcPtr[x].r;
			dstPtr->g = srcPtr[x].g;
			dstPtr->b = srcPtr[x].b;
			++dstPtr;
		}
		srcPtr = (buffer_color_rgb_t *)&((char *)srcPtr)[inBytesPerRow];
	}

	/* create a new texture object
	 * and bind it to texname (unsigned integer > 0)
	 */
	::glBindTexture(GL_TEXTURE_2D, mTextureId);
	::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//    ::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	::glTexImage2D(GL_TEXTURE_2D, 0,
	               3, // Because three components (R, G, B)
	               mSize.getWidth(), mSize.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
	               destBuffer);

	delete[] destBuffer;
	return true; // success
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
TextureCharacteristics::~TextureCharacteristics()
{
	::glDeleteTextures(1, &mTextureId);
}

/** Determine the 2^n fragments of a given integer and return them.
 */
static size_t getLog2NFragments(size_t inValue, uint32_t values[31])
{
	uint32_t mask     = 1L;
	int      inValues = 0;

	uint32_t tmpValues[31];

	for(int idx = 0; idx < 32; ++idx) {
		// If the mask is biger than the value then finish searching further.
		if(mask > inValue)
			break;

		if((mask & inValue) != 0)
			tmpValues[inValues++] = 1 << idx;
		mask <<= 1;
	}

	// Reverse the sorting in descending order
	for(int i = 0; i < inValues; ++i)
		values[i] = tmpValues[inValues - 1 - i];

	return inValues;
}

/*
void Texture::buildTiledImageBuffers(const char* inImageData,
                                     size_t      inBytesPerRow,
                                     size_t      inWidth,
                                     size_t      inHeight,
                                     bool        inAlpha)
*/

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Texture::Texture()
        : mCountTilesX(0)
        , mCountTilesY(0)
        ,

        mSizeXInPercent(NULL)
        , mSizeYInPercent(NULL)
        ,

        mTextureCharacteristics(NULL)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Texture::~Texture()
{
	releaseAll();
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool Texture::load(const std::string & inFilename)
{
	return buildTiledImageBuffers(inFilename);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
GLuint Texture::getTextureIdOfXY(size_t inX, size_t inY) const
{
	size_t offset = inY * mCountTilesX + inX;
	return mTextureCharacteristics[offset]->mTextureId;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Texture::mapToGeometry(const Rect & inRect, float inZLayer) const
{
	// Initialized?
	if(mTextureCharacteristics == NULL)
		return;

	// calculate the parts of the textures...

	float floatWidth  = (float)inRect.getWidth();
	float floatHeight = (float)inRect.getHeight();

	float y = (float)inRect.getY();
	for(size_t idxy = 0; idxy < mCountTilesY; ++idxy) {
		float height = getFragmentTextureHeightInPercent(idxy) * floatHeight;
		float x      = (float)inRect.getX();
		for(size_t idxx = 0; idxx < mCountTilesX; ++idxx) {
			float width = getFragmentTextureWidthInPercent(idxx) * floatWidth;

			::glEnable(GL_TEXTURE_2D);
			::glBindTexture(GL_TEXTURE_2D, getTextureIdOfXY(idxx, idxy));

			::glBegin(GL_QUADS);

			::glTexCoord2f(0.0f, 0.0f);
			::glVertex3f(x, y, inZLayer);

			::glTexCoord2f(1.0f, 0.0f);
			::glVertex3f(x + width, y, inZLayer);

			::glTexCoord2f(1.0f, 1.0f);
			::glVertex3f(x + width, y + height, inZLayer);

			::glTexCoord2f(0.0f, 1.0f);
			::glVertex3f(x, y + height, inZLayer);

			::glEnd();

			::glDisable(GL_TEXTURE_2D);
			x += width;
		}
		y += height;
	}
}

// Private methods

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Texture::releaseTextureCharacteristics()
{
	if(mTextureCharacteristics != NULL) {
		for(size_t i = 0; i < getNumberOfTextures(); ++i) {
			delete mTextureCharacteristics[i];
		}
		delete[] mTextureCharacteristics; // not longer needed
		mTextureCharacteristics = NULL;
	}
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Texture::releaseAll()
{
	releaseTextureCharacteristics();

	delete[] mSizeXInPercent;
	delete[] mSizeYInPercent;

	mSizeXInPercent = mSizeYInPercent = NULL;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool Texture::buildTiledImageBuffers(const std::string & inFilename)
{
	releaseAll();

	// use ImageData to load the texture
	ImageData textureData;
	if(textureData.pngLoad(inFilename) == false)
		return false;

	uint32_t sizesX[32];
	uint32_t sizesY[32];

	mCountTilesX = getLog2NFragments(
	        textureData.getWidth(),
	        sizesX); // calculate how many 2^n tiles are needed in horiz. direction
	mCountTilesY = getLog2NFragments(
	        textureData.getHeight(),
	        sizesY); // calculate how many 2^n tiles are needed in vert. direction

	size_t numberOfTextures = getNumberOfTextures();
	mTextureCharacteristics = new TextureCharacteristics *[numberOfTextures];

	GLuint * textureIds = new GLuint[numberOfTextures];

	mSizeXInPercent = new float[mCountTilesX];
	mSizeYInPercent = new float[mCountTilesY];

	size_t offsetX           = 0;
	float  textureTotalWidth = (float)textureData.getWidth();

	for(size_t idxx = 0; idxx < mCountTilesX; ++idxx) {
		mSizeXInPercent[idxx] = (float)sizesX[idxx] / textureTotalWidth;
		offsetX += sizesX[idxx];
	}

	size_t offsetY            = 0;
	float  textureTotalHeight = (float)textureData.getHeight();

	for(size_t idxy = 0; idxy < mCountTilesY; ++idxy) {
		mSizeYInPercent[idxy] = (float)sizesY[idxy] / textureTotalHeight;
		offsetY += sizesY[idxy];
	}

	::glGenTextures(numberOfTextures, textureIds);

	offsetY    = 0;
	size_t idx = 0;
	for(size_t idxy = 0; idxy < mCountTilesY; ++idxy) {
		size_t offsetX = 0;
		for(size_t idxx = 0; idxx < mCountTilesX; ++idxx) {
			if(textureData.hasAlpha()) {
				mTextureCharacteristics[idx] = new TextureCharacteristics(
				        Size(sizesX[idxx], sizesY[idxy]), glm::ivec2(offsetX, offsetY),
				        (buffer_color_rgba_t *)textureData.getData(),
				        textureData.getBytesPerRow(), textureIds[idx]);
			} else {
				mTextureCharacteristics[idx] = new TextureCharacteristics(
				        Size(sizesX[idxx], sizesY[idxy]), glm::ivec2(offsetX, offsetY),
				        (buffer_color_rgb_t *)textureData.getData(),
				        textureData.getBytesPerRow(), textureIds[idx]);
			}

			++idx;
			offsetX += sizesX[idxx];
		}
		offsetY += sizesY[idxy];
	}

	delete[] textureIds;
	return true;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
float Texture::getFragmentTextureWidthInPercent(size_t inIndexX) const
{
	assert(inIndexX < mCountTilesX);
	return mSizeXInPercent[inIndexX];
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
float Texture::getFragmentTextureHeightInPercent(size_t inIndexY) const
{
	assert(inIndexY < mCountTilesY);
	return mSizeYInPercent[inIndexY];
}

} // namespace mhgui
