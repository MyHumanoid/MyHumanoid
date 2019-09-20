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
 *  File   : CGUtilities.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef CGUTILITIES_H
#define CGUTILITIES_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <iostream>
#include <cstdio>

#include <animorph/Vector3.h>
#include <animorph/Matrix.h>
#include <animorph/Color.h>

#include "GLUTWrapper.h"
#include "Point.h"
#include "Size.h"
#include "Rect.h"
#include "ImageData.h"
#include "Camera.h"

using std::string;
using namespace Animorph;
using namespace mhgui;

typedef void* FontType;

#define USE_VERSATILE_TEXTURES 1

/** \brief Computer Graphics Utilities
 *
 * This namespace contains wrappers and convenience functions for OpenGL
 * calls.
 */
namespace cgutils
{
void drawGrid (const Size& inSize, const int xMargin, const int yMargin, const Color& c, const Color& b, int squareSize);

void drawAxis ();

void reshape (const Size& inSize, const Camera& inCamera);

void enableLightingDeepTest ();

void disableLightingDeepTest ();

void enableOrthographicProjection ();

void disableOrthographicProjection ();

#ifdef USE_VERSATILE_TEXTURES
    void drawSquareFillTexture (const Rect& inRect, float alpha, const Texture& inTexture);
    void drawBackgroundSquare  (const Size& inSize, float alpha, const Texture& inTexture);
#else
    void drawSquareFillTexture (const Rect& inRect, float alpha, unsigned int textuID);
    void drawBackgroundSquare  (const Size& inSize, float alpha, unsigned int textuID);
#endif


void drawSquareFill (const Rect& inRect, const Color& c);

void drawSquare (const Rect& inRect, const Color& c);

void displayStart (const Vector3f& tra, const Vector3f& rot);

void displayStart (const Matrix& m);

int  initWindow (const Size& inSize, const char* title, const Vector3f& light0Pos,
                 const Vector3f& light1Pos, float light0Lum, float light1Lum,
                 const Color& c);

void destroyWindow(int inWindowId);

void drawString (const Point& inPoint, FontType font, const string& str, const Color& c);

void drawMultiLineString (Point& inPoint, FontType font, const string& str, const Color& c, const int32_t lineWidth, const int lineOffset);

void drawString3D (const Vector3f& pos, FontType font, const string& str, const Color& c);

void redisplay ();

void swapBuffers ();

void mainLoop ();

void enableBlend ();

void disableBlend ();

void enableLineSmoothing();

void disableLineSmoothing();

void enableScissor (const Rect& box);

void disableScissor ();

#ifndef USE_VERSATILE_TEXTURES
// take the raw image and put it into an
// OpenGL 2D texture.
unsigned int setupGLTexture (ImageData &image_data);

void deleteGLTexture (GLuint textureID);
#endif // !USE_VERSATILE_TEXTURES

int getFontWidth (FontType font);

int getFontLength (FontType font, const string& str);

int getKeyModifiers();

void mhWireCube (const float twoxyz[6]);

void drawLine2D (const Point& start, const Point& end, const Color& c);

int getScreenHeight ();

int getScreenWidth ();
}

#endif //CGUTILITIES_H
