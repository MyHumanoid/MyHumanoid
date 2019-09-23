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

#include <cstdio>
#include <iostream>
#include <string>

#include <animorph/Color.h>
#include <animorph/Matrix.h>
#include <animorph/Vector3.h>

#include "Camera.h"
#include "GLUTWrapper.h"
#include "ImageData.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"

using std::string;
using namespace Animorph;
using namespace mhgui;

typedef void *FontType;

/** \brief Computer Graphics Utilities
 *
 * This namespace contains wrappers and convenience functions for OpenGL
 * calls.
 */
namespace cgutils
{
void drawGrid(const Size &inSize, const int xMargin, const int yMargin,
              const Color &c, const Color &b, int squareSize);

void drawAxis();

void reshape(const Size &inSize, const Camera &inCamera);

void enableOrthographicProjection();

void disableOrthographicProjection();

void drawSquareFillTexture(const Rect &inRect, float alpha,
                           const Texture &inTexture);
void drawBackgroundSquare(const Size &inSize, float alpha,
                          const Texture &inTexture);

void drawSquareFill(const Rect &inRect, const Color &c);

void drawSquare(const Rect &inRect, const Color &c);

void displayStart(const Vector3f &tra, const Vector3f &rot);

void displayStart(const Matrix &m);

int initWindow(const Rect &rect, const char *title, const Vector3f &light0Pos,
               const Vector3f &light1Pos, float light0Lum, float light1Lum,
               const Color &c);

void destroyWindow(int inWindowId);

void drawString(const Point &inPoint, FontType font, const string &str,
                const Color &c);

void drawMultiLineString(Point &inPoint, FontType font, const string &str,
                         const Color &c, const int32_t lineWidth,
                         const int lineOffset);

void drawString3D(const Vector3f &pos, FontType font, const string &str,
                  const Color &c);

void redisplay();

void enableBlend();

void enableScissor(const Rect &box);

void disableScissor();

int getFontWidth(FontType font);

int getFontLength(FontType font, const string &str);

void mhWireCube(const float twoxyz[6]);

void drawLine2D(const Point &start, const Point &end, const Color &c);

} // namespace cgutils

#endif // CGUTILITIES_H
