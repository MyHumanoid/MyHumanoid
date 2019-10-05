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
#include "gui/Size.h"
#include "gui/Texture.h"

#include "log/log.h"

#include "render/DebugGl.h"

using Animorph::Color;
using mhgui::Rect;
using mhgui::Camera;
using mhgui::Texture;

void cgutils::drawLine2D(const glm::ivec2 & start, const glm::ivec2 & end, const Color & c)
{
	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glBegin(GL_LINES);

	glVertex3f(start.x, start.y, 0.0);
	glVertex3f(end.x, end.y, 0.0);

	glEnd();
}

/*! Draw a grid on XY plane.*/
void cgutils::drawGrid(const Size & inSize, const int xMargin, const int yMargin, const Color & c,
                       const Color & b, int squareSize)
{
	int i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, inSize.getWidth(), 0, inSize.getHeight(), -20.0, 20.0);
	glScalef(1, -1, 1);
	glTranslatef(0, -inSize.getHeight(), 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	enableBlend();

	glTranslatef(0, 0, -0.1);
	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glBegin(GL_LINES);

	for(i = squareSize + xMargin; i < inSize.getWidth() - xMargin; i += squareSize) {
		glVertex3f(i, yMargin, 0.0);
		glVertex3f(i, inSize.getHeight() - yMargin, 0.0);
	}

	for(i = squareSize + yMargin; i < inSize.getHeight() - yMargin; i += squareSize) {
		glVertex3f(xMargin, i, 0.0);
		glVertex3f(inSize.getWidth() - xMargin, i, 0.0);
	}

	glEnd();

	drawSquareFill(Rect(xMargin, yMargin, 10, 10), b);
	drawSquareFill(
	        Rect(inSize.getWidth() - xMargin - 10, inSize.getHeight() - yMargin - 10, 10, 10),
	        b);

	drawSquareFill(Rect(xMargin, inSize.getHeight() - yMargin - 40, 3, 40), b);
	drawSquareFill(Rect(xMargin, inSize.getHeight() - yMargin, 40, 3), b);

	drawSquareFill(Rect(inSize.getWidth() - xMargin, yMargin, 3, 40), b);
	drawSquareFill(Rect(inSize.getWidth() - xMargin - 40, yMargin, 40, 3), b);

	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/*! Set orthographic projection before drawing the widgets.*/
void cgutils::enableOrthographicProjection()
{
	// Get the actual windows size
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLint & w = viewport[2];
	GLint & h = viewport[3];

	// switch to projection mode

	if(glIsEnabled(GL_LIGHTING)) {
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_DEPTH_TEST)) {
		glDisable(GL_DEPTH_TEST);
	}

	// glEnable (GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	// save projection matrix
	glPushMatrix();
	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	glOrtho(0, w, h, 0, -1, 1);

	glClear(GL_DEPTH_BUFFER_BIT);
	// invert the y axis, down is positive (GLUT has different Y than openGL)
	// glScalef (1, -1, 1);
	// move the origin from the bottom left corner
	// to the upper left corner (GLUT has different Y than openGL)
	// glTranslatef (0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*! Restore the projection after drawed the widget.*/
void cgutils::disableOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// glDisable (GL_DEPTH_TEST);

	if(!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}
	if(!glIsEnabled(GL_DEPTH_TEST)) {
		glEnable(GL_DEPTH_TEST);
	}
}

/*!Draw the XYZ axis.*/
void cgutils::drawAxis()
{
	float wid    = 2.8;
	char  axis[] = "XYZ";

	if(glIsEnabled(GL_LIGHTING)) {
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_DEPTH_TEST)) {
		glDisable(GL_DEPTH_TEST);
	}

	glBegin(GL_LINES);
	glColor3f(0.5, 1, 0.5);
	glVertex3f(0, 0, -wid);
	glVertex3f(0, 0, wid);
	glColor3f(0.5, 0.5, 1);
	glVertex3f(-wid, 0, 0);
	glVertex3f(wid, 0, 0);
	glColor3f(1, 0.5, 0.5);
	glVertex3f(0, -wid, 0);
	glVertex3f(0, wid, 0);
	glEnd();
	glPushMatrix();
	glTranslatef(0, wid, 0);
	glRasterPos2f(0.05, 0.05);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, axis[1]);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.03, 0.1, 8, 4);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.5, 0.5, 1);
	glTranslatef(wid, 0, 0);
	glRasterPos2f(0.05, 0.05);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, axis[0]);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(0.03, .1, 8, 4);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.5, 1, .5);
	glTranslatef(0, 0, wid);
	glRasterPos2f(0.05, 0.05);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, axis[2]);
	glRotatef(90, 0, 0, 1);
	glutSolidCone(0.03, 0.1, 8, 4);
	glPopMatrix();

	if(!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}
	if(!glIsEnabled(GL_DEPTH_TEST)) {
		glEnable(GL_DEPTH_TEST);
	}
}

void cgutils::drawSquareFillTexture(const Rect & inRect, float alpha, const Texture & inTexture)
{
	float zlayer = 0.0;

	glColor4f(1.0, 1.0, 1.0, alpha);
	inTexture.mapToGeometry(inRect, zlayer);
}

void cgutils::drawSquareFill(const mhgui::Rect & inRect, const Color & c)
{
	float zlayer = 0.0;

	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(inRect.getX(), inRect.getY(), zlayer);
	glTexCoord2f(1, 0);
	glVertex3f(inRect.getX() + inRect.getWidth(), inRect.getY(), zlayer);
	glTexCoord2f(1, 1);
	glVertex3f(inRect.getX() + inRect.getWidth(), inRect.getY() + inRect.getHeight(), zlayer);
	glTexCoord2f(0, 1);
	glVertex3f(inRect.getX(), inRect.getY() + inRect.getHeight(), zlayer);
	glEnd();
}

void cgutils::drawSquare(const Rect & inRect, const Color & c)
{
	float zlayer = 0.0;

	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glBegin(GL_LINE_LOOP);
	glVertex3f(inRect.getX(), inRect.getY() + 1, zlayer);
	glVertex3f(inRect.getX() - 1 + inRect.getWidth(), inRect.getY(), zlayer);
	glVertex3f(inRect.getX() - 1 + inRect.getWidth(), inRect.getY() + inRect.getHeight(),
	           zlayer);
	glVertex3f(inRect.getX(), inRect.getY() + inRect.getHeight(), zlayer);
	glEnd();
}

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
void cgutils::reshape(const Size & inSize, const Camera & inCamera)
{
	glViewport(0, 0, inSize.getWidth(), inSize.getHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const float kZFar     = 1000.0f;
	const float kZNear    = 1.0f;
	const float kFOVY     = 10.0;
	const float zToCenter = (fabs(inCamera.getPosition().z));

	if(inCamera.isPerspective()) // if perspective
	{
		if(inSize.getHeight() == 0)
			perspectiveGL(kFOVY, static_cast<float>(inSize.getWidth()), kZNear, kZFar);
		else
			perspectiveGL(kFOVY,
			              static_cast<float>(inSize.getWidth()) /
			                      static_cast<float>(inSize.getHeight()),
			              kZNear, kZFar);
	} else {
		float ratioW = static_cast<float>(inSize.getWidth()) /
		               static_cast<float>(inSize.getHeight());
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
	glutInitWindowSize(rect.getWidth(), rect.getHeight());
	glutInitWindowPosition(rect.getX(), rect.getY());
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

void cgutils::drawString(const glm::ivec2 & inPoint, FontType font, const string & str, const Color & c)
{
	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glRasterPos2f(inPoint.x, inPoint.y);

	// TODO: anchor text in the upper left corner instead of lower left!

	for(string::const_iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		char ch = *str_it;

		glutBitmapCharacter(font, ch);
	}
}

void cgutils::drawString3D(const glm::vec3 & pos, FontType font, const string & str,
                           const Color & c)
{
	glColor4f(c.red(), c.green(), c.blue(), c.alpha());
	glRasterPos3f(pos.x, pos.y, pos.z);

	for(string::const_iterator str_it = str.begin(); str_it != str.end(); str_it++) {
		char ch = *str_it;

		glutBitmapCharacter(font, ch);
	}
}

void cgutils::enableBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// TODO: temporary broken; fix it!
int cgutils::getFontWidth(FontType font)
{
	if(font == GLUT_BITMAP_8_BY_13)
		return 13;

	if(font == GLUT_BITMAP_9_BY_15)
		return 15;

	if(font == GLUT_BITMAP_TIMES_ROMAN_10)
		return 10;

	if(font == GLUT_BITMAP_TIMES_ROMAN_24)
		return 24;

	if(font == GLUT_BITMAP_HELVETICA_10)
		return 10;

	if(font == GLUT_BITMAP_HELVETICA_12)
		return 12;

	if(font == GLUT_BITMAP_HELVETICA_18)
		return 18;

	return 0;
}

int cgutils::getFontLength(FontType font, const string & str)
{
	return glutBitmapLength(font, (unsigned char *)str.c_str());
}
