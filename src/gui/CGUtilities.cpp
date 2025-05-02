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
 *  File   : CGUtilities.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#include "gui/CGUtilities.h"

#include <glm/ext.hpp>

#include "gui/Camera.h"
#include "gui/Rect.h"
#include "AbstractGl.h"

#include "Logger.h"


using Animorph::Color;
using mhgui::Rect;
using mhgui::Camera;

glm::mat4 g_projectionMatrix;

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble       fW, fH;

	// fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	//glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

// Glut call back functions
void cgutils::reshape(const glm::ivec2 & inSize, const Camera & inCamera)
{
	glViewport(0, 0, inSize.x, inSize.y);

	const float kZFar     = 1000.0f;
	const float kZNear    = 1.0f;
	const float kFOVY     = 10.0;
	const float zToCenter = (fabs(inCamera.getPosition().z));

	if(inCamera.isPerspective()) // if perspective
	{
		glm::vec2 fSize = inSize;
		
		g_projectionMatrix = glm::perspectiveFov(kFOVY, fSize.x, fSize.y, kZNear, kZFar);
	} else {
		float ratioW = static_cast<float>(inSize.x) /
		               static_cast<float>(inSize.y);
		float scalex = (zToCenter * 0.08761 *
		                ratioW); // provvisorio: ricavare un fattore dalla distanza
		float scaley = (zToCenter * 0.08761); // della camera dall'origine
		
		g_projectionMatrix = glm::ortho(-scalex, scalex, -scaley, scaley, kZNear, kZFar);
	}
}

void cgutils::enableBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
