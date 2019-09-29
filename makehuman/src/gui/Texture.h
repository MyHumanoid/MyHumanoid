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
 *  File   : Texture.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#include <GL/gl.h>  // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library

#include <string>

using std::string;

namespace mhgui
{

class TextureCharacteristics;
class Rect;

class Texture
{
public:
	Texture();
	~Texture();

	bool load(const string & inFilename);

	size_t getNumberOfTextures() const { return mCountTilesX * mCountTilesY; }

	float getFragmentTextureWidthInPercent(size_t inIndexX) const;
	float getFragmentTextureHeightInPercent(size_t inIndexY) const;

	float getFragmentTextureOffsetXInPercent(size_t inIndexX) const;
	float getFragmentTextureOffsetYInPercent(size_t inIndexY) const;

	void   mapToGeometry(const Rect & inRect, float inZLayer = 0.0) const;
	GLuint getTextureIdOfXY(size_t inX, size_t inY) const;

private:
	void releaseAll();
	void releaseTextureCharacteristics();
	bool buildTiledImageBuffers(const string & inFilename);

private:
	size_t mCountTilesX;
	size_t mCountTilesY;

	float * mSizeXInPercent;
	float * mSizeYInPercent;

	TextureCharacteristics ** mTextureCharacteristics;
}; // class Texture

} // namespace mhgui

#endif // TEXTURE_H
