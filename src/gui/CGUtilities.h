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
#pragma once

#include <cstdio>
#include <iostream>
#include <string>

#include <animorph/Color.h>
#include <animorph/Matrix.h>
#include <glm/vec3.hpp>

#include "Camera.h"
#include "Rect.h"

using std::string;
using Animorph::Color;
using mhgui::Rect;

typedef void * FontType;

/** \brief Computer Graphics Utilities
 *
 * This namespace contains wrappers and convenience functions for OpenGL
 * calls.
 */
namespace cgutils
{
void reshape(const glm::ivec2 & inSize, const mhgui::Camera & inCamera);

int initWindow(const Rect & rect, const char * title);

void redisplay();

void enableBlend();

void mhWireCube(const float twoxyz[6]);

} // namespace cgutils
