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

#include "GlInclude.h"

#include "gui/CGUtilities.h"
#include "gui/Camera.h"
#include "gui/Rect.h"

#include "Logger.h"

#include "render/DebugGl.h"

using Animorph::Color;
using mhgui::Rect;
using mhgui::Camera;

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble       fW, fH;

	// fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

// Glut call back functions
void cgutils::reshape(const glm::ivec2 & inSize, const Camera & inCamera)
{
	glViewport(0, 0, inSize.x, inSize.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const float kZFar     = 1000.0f;
	const float kZNear    = 1.0f;
	const float kFOVY     = 10.0;
	const float zToCenter = (fabs(inCamera.getPosition().z));

	if(inCamera.isPerspective()) // if perspective
	{
		if(inSize.y == 0)
			perspectiveGL(kFOVY, static_cast<float>(inSize.x), kZNear, kZFar);
		else
			perspectiveGL(kFOVY,
			              static_cast<float>(inSize.x) /
			                      static_cast<float>(inSize.y),
			              kZNear, kZFar);
	} else {
		float ratioW = static_cast<float>(inSize.x) /
		               static_cast<float>(inSize.y);
		float scalex = (zToCenter * 0.08761 *
		                ratioW); // provvisorio: ricavare un fattore dalla distanza
		float scaley = (zToCenter * 0.08761); // della camera dall'origine
		glOrtho(-scalex, scalex, -scaley, scaley, kZNear, kZFar);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Init window with some classic openGL commands
int cgutils::initWindow(const Rect & rect, const char * title, const glm::vec3 & light0Pos,
                        const glm::vec3 & light1Pos, float light0Lum, float light1Lum)
{

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextFlags(/*GLUT_FORWARD_COMPATIBLE | */ GLUT_DEBUG);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(rect.size.x, rect.size.y);
	glutInitWindowPosition(rect.pos.x, rect.pos.y);
	int winID = glutCreateWindow(title);

	glewInit();

	{
		log_info("GLEW Version: {}.{}.{}", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
		       GLEW_VERSION_MICRO);

		glewExperimental = true;
		GLenum err       = glewInit();
		if(err != GLEW_OK) {
			log_error("GLEW error: {}", glewGetErrorString(err));
		}
	}

	initDebugGl();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Light0 features
	float light_diffuse0[]  = {light0Lum, light0Lum, light0Lum, light0Lum};
	float light_specular0[] = {1.0, 1.0, 1.0, 1.0};

	// Light1 features
	float light_diffuse1[]  = {light1Lum, light1Lum, light1Lum, light1Lum};
	float light_specular1[] = {1.0, 1.0, 1.0, 1.0};

	// Lights positions
	float light_position0[] = {light0Pos.x, light0Pos.y, light0Pos.z, 0.0};
	float light_position1[] = {light1Pos.x, light1Pos.y, light1Pos.z, 0.0};

	// Apply lights positions
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	// Apply lights features
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

	// Clear the background
	glClearColor(0, 0, 0, 1);

	return winID;
}

void cgutils::enableBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
