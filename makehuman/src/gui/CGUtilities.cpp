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

#include "gui/Rect.h"
#include "gui/Point.h"
#include "gui/Size.h"
#include "gui/Texture.h"
#include "gui/Camera.h"
#include "gui/CGUtilities.h"

using namespace std;
using namespace mhgui;

void cgutils::drawLine2D (const Point& start, const Point& end, const Color& c)
{
  glColor4f(c.red (), c.green (), c.blue (), c.alpha ());
  glBegin (GL_LINES);

  glVertex3f (start.getX(), start.getY(), 0.0);
  glVertex3f (end.getX(), end.getY(), 0.0);

  glEnd ();
}

/*! Draw a grid on XY plane.*/
void cgutils::drawGrid (const Size& inSize, const int xMargin, const int yMargin, const Color& c, const Color& b, int squareSize)
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
  glColor4f(c.red (), c.green (), c.blue (), c.alpha ());
  glBegin (GL_LINES);

  for (i = squareSize + xMargin; i < inSize.getWidth() - xMargin; i += squareSize)
  {
    glVertex3f (i, yMargin, 0.0);
    glVertex3f (i, inSize.getHeight() - yMargin, 0.0);
  }

  for (i = squareSize + yMargin; i < inSize.getHeight() - yMargin; i += squareSize)
  {
    glVertex3f (xMargin, i, 0.0);
    glVertex3f (inSize.getWidth() - xMargin, i, 0.0);
  }

  glEnd ();

  drawSquareFill(Rect(xMargin, yMargin, 10, 10), b);
  drawSquareFill(Rect(inSize.getWidth() - xMargin - 10, inSize.getHeight() - yMargin - 10, 10, 10), b);

  drawSquareFill(Rect(xMargin, inSize.getHeight() - yMargin - 40, 3, 40), b);
  drawSquareFill(Rect(xMargin, inSize.getHeight() - yMargin, 40, 3), b);

  drawSquareFill(Rect(inSize.getWidth() - xMargin, yMargin, 3, 40), b);
  drawSquareFill(Rect(inSize.getWidth() - xMargin - 40, yMargin, 40, 3), b);

  disableBlend();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

/*! Set orthographic projection before drawing the widgets.*/
void cgutils::enableOrthographicProjection ()
{
  //Get the actual windows size
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLint &w = viewport[2];
  GLint &h = viewport[3];

  // switch to projection mode

	if(glIsEnabled(GL_LIGHTING)) {
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_DEPTH_TEST)) {
		glDisable(GL_DEPTH_TEST);
	}
	
  //glEnable (GL_DEPTH_TEST);
  glMatrixMode (GL_PROJECTION);
  // save projection matrix
  glPushMatrix ();
  // reset matrix
  glLoadIdentity ();
  // set a 2D orthographic projection
  gluOrtho2D (0, w, h, 0);//, 0, 10); // realize 10 layers [0,-10]
  glClear ( GL_DEPTH_BUFFER_BIT );
  // invert the y axis, down is positive (GLUT has different Y than openGL)
  //glScalef (1, -1, 1);
  // move the origin from the bottom left corner
  // to the upper left corner (GLUT has different Y than openGL)
  //glTranslatef (0, -h, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

/*! Restore the projection after drawed the widget.*/
void cgutils::disableOrthographicProjection ()
{
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  //glDisable (GL_DEPTH_TEST);
  
	if(!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}
	if(!glIsEnabled(GL_DEPTH_TEST)){
		glEnable (GL_DEPTH_TEST);
	}
}

/*!Draw the XYZ axis.*/
void cgutils::drawAxis ()
{
	float wid = 2.8;
	char axis[]= "XYZ";
	
	if(glIsEnabled(GL_LIGHTING)) {
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_DEPTH_TEST)) {
		glDisable(GL_DEPTH_TEST);
	}
  
  glBegin (GL_LINES);
  glColor3f (0.5, 1, 0.5);
  glVertex3f (0, 0, -wid);
  glVertex3f (0, 0, wid);
  glColor3f (0.5, 0.5, 1);
  glVertex3f (-wid, 0, 0);
  glVertex3f (wid, 0, 0);
  glColor3f (1, 0.5, 0.5);
  glVertex3f (0,-wid, 0);
  glVertex3f (0, wid, 0);
  glEnd ();
  glPushMatrix ();
  glTranslatef (0, wid, 0);
  glRasterPos2f (0.05, 0.05);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, axis[1]);
  glRotatef (-90, 1,0,0);
  glutSolidCone (0.03, 0.1, 8, 4);
  glPopMatrix ();
  glPushMatrix ();
  glColor3f (0.5, 0.5, 1);
  glTranslatef (wid,0,0);
  glRasterPos2f (0.05, 0.05);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, axis[0]);
  glRotatef (90, 0,1,0);
  glutSolidCone (0.03,.1,8,4);
  glPopMatrix ();
  glPushMatrix ();
  glColor3f(0.5, 1, .5);
  glTranslatef (0,0,wid);
  glRasterPos2f (0.05, 0.05);
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, axis[2]);
  glRotatef (90, 0,0,1);
  glutSolidCone (0.03, 0.1, 8, 4);
  glPopMatrix ();
  
	if(!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}
	if(!glIsEnabled(GL_DEPTH_TEST)){
		glEnable (GL_DEPTH_TEST);
	}
}

/*!Start  part of display function before draw the scene.*/
void cgutils::displayStart (const Vector3f& tra, const Vector3f& rot)
{
  //Classic openGL  display commands
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode   ( GL_MODELVIEW );
  glLoadIdentity ( );

  //Zoom and Pan
  glTranslatef(tra.x, tra.y, tra.z);
  glRotatef(rot.x, 1, 0, 0);
  glRotatef(rot.y, 0, 1, 0);
  glRotatef(rot.z, 0, 0, 1);
}

/*! Classic openGL  display commands.*/
void cgutils::displayStart (const Matrix& m)
{
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode   ( GL_MODELVIEW );

  glLoadIdentity ( );

  glMultMatrixf (m.data);
}

#ifdef USE_VERSATILE_TEXTURES
void cgutils::drawSquareFillTexture (const Rect& inRect, float alpha, const Texture& inTexture)
{
  float zlayer = 0.0;

  glColor4f(1.0, 1.0, 1.0, alpha);
  inTexture.mapToGeometry(inRect, zlayer);
}

#else //!USE_VERSATILE_TEXTURES

void cgutils::drawSquareFillTexture (const Rect& inRect, float alpha, unsigned int textuID)
{
  if (textuID) //To avoid use unitialized ID
  {
    glBindTexture(GL_TEXTURE_2D,textuID);
  }

  float zlayer = 0.0;

  glColor4f(1.0, 1.0, 1.0, alpha);
  glEnable (GL_TEXTURE_2D);
  glBegin (GL_QUADS);
  glTexCoord2f(0, 1);
  glVertex3f (inRect.getX(), inRect.getY(), zlayer);
  glTexCoord2f(1, 1);
  glVertex3f (inRect.getX() + inRect.getWidth(), inRect.getY(), zlayer);
  glTexCoord2f(1, 0);
  glVertex3f (inRect.getX() + inRect.getWidth(), inRect.getY() + inRect.getHeight(), zlayer);
  glTexCoord2f(0, 0);
  glVertex3f (inRect.getX(), inRect.getY() + inRect.getHeight(), zlayer);
  glEnd ();
  glDisable (GL_TEXTURE_2D);
}
#endif // USE_VERSATILE_TEXTURES


#ifdef USE_VERSATILE_TEXTURES
void cgutils::drawBackgroundSquare (const Size& inSize, float alpha, const Texture& inTexture)
{
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, inSize.getWidth(), 0, inSize.getHeight(), -20.0, 20.0);
  glScalef(1, -1, 1);
  glTranslatef(0, -inSize.getHeight(), 0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0, 0, -19.9);
  glColor4f(1.0, 1.0, 1.0, alpha);
  glNormal3f( 0.0f, 0.0f, 1.0f);

  inTexture.mapToGeometry(Rect(0, 0, inSize.getWidth(), inSize.getHeight()), 0.0);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

#else //!USE_VERSATILE_TEXTURES

void cgutils::drawBackgroundSquare (const Size& inSize, float alpha, unsigned int textuID)
{
  if(textuID)
  {
    glBindTexture(GL_TEXTURE_2D,textuID);
  }
  glPushMatrix();
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
          glLoadIdentity();
          glOrtho(0, inSize.getWidth(), 0, inSize.getHeight(), -20.0, 20.0);
          glScalef(1, -1, 1);
          glTranslatef(0, -inSize.getHeight(), 0.0);
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();

          glTranslatef(0, 0, -19.9);
          glColor4f(1.0, 1.0, 1.0, alpha);
          glNormal3f( 0.0f, 0.0f, 1.0f);

          glEnable (GL_TEXTURE_2D);
              glBegin (GL_QUADS);
                  glTexCoord2f(0, 1);
                  glVertex3f (0, 0, 0.0);
                  glTexCoord2f(1, 1);
                  glVertex3f (inSize.getWidth(), 0, 0.0);
                  glTexCoord2f(1, 0);
                  glVertex3f (inSize.getWidth(), inSize.getHeight(), 0.0);
                  glTexCoord2f(0, 0);
                  glVertex3f (0, inSize.getHeight(), 0.0);
              glEnd ();
          glDisable (GL_TEXTURE_2D);

          glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
#endif // USE_VERSATILE_TEXTURES

void cgutils::drawSquareFill (const mhgui::Rect& inRect, const Color& c)
{
  float zlayer = 0.0;

  glColor4f(c.red (), c.green (), c.blue (), c.alpha ());
  glBegin (GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f (inRect.getX(), inRect.getY(), zlayer);
  glTexCoord2f(1, 0);
  glVertex3f (inRect.getX() + inRect.getWidth(), inRect.getY(), zlayer);
  glTexCoord2f(1, 1);
  glVertex3f (inRect.getX() + inRect.getWidth(), inRect.getY() + inRect.getHeight(), zlayer);
  glTexCoord2f(0, 1);
  glVertex3f (inRect.getX(), inRect.getY() + inRect.getHeight(), zlayer);
  glEnd ();
}

void cgutils::drawSquare (const Rect& inRect, const Color& c)
{
  float zlayer = 0.0;

  glColor4f(c.red (), c.green (), c.blue (), c.alpha ());
  glBegin (GL_LINE_LOOP);
      glVertex3f (inRect.getX(), inRect.getY()+1, zlayer);
      glVertex3f (inRect.getX()-1 + inRect.getWidth(), inRect.getY(), zlayer);
      glVertex3f (inRect.getX()-1 + inRect.getWidth(), inRect.getY() + inRect.getHeight(), zlayer);
      glVertex3f (inRect.getX(), inRect.getY() + inRect.getHeight(), zlayer);
  glEnd ();
}

//Glut call back functions
void cgutils::reshape (const Size& inSize, const Camera& inCamera)
{
  glViewport (0, 0, inSize.getWidth(), inSize.getHeight());
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  const float kZFar  = 1000.0f;
  const float kZNear =    1.0f;
  const float kFOVY = 10.0;
  const float zToCenter = (fabs(inCamera.getPosition().z));

  if (inCamera.isPerspective()) //if perspective
  {
    if (inSize.getHeight() == 0)
      gluPerspective ( kFOVY, static_cast <float> (inSize.getWidth()), kZNear, kZFar);
    else
      gluPerspective ( kFOVY, static_cast <float> (inSize.getWidth()) / static_cast <float> (inSize.getHeight()), kZNear, kZFar);
  }
  else
  {
	  float ratioW =  static_cast <float> (inSize.getWidth()) / static_cast <float> (inSize.getHeight());
	  float scalex =  (zToCenter * 0.08761 * ratioW); //provvisorio: ricavare un fattore dalla distanza
	  float scaley = (zToCenter * 0.08761);// della camera dall'origine
    glOrtho(-scalex, scalex, -scaley, scaley, kZNear, kZFar);
  }

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}



//Init window with some classic openGL commands
int cgutils::initWindow (const Size& inSize, const char* title,
                         const Vector3f& light0Pos, const Vector3f& light1Pos,
                         float light0Lum, float light1Lum, const Color& c)
{

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize  (inSize.getWidth(), inSize.getHeight());
  glutInitWindowPosition (0, 0);
  int winID = glutCreateWindow (title);

  //Enables
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  //Standard material
  float mat_ambient[]    = { 0.7, 0.7, 0.7, 1.0 };
  float mat_diffuse[]    = { 0.8, 0.8, 0.8, 1.0 };
  float mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
  float high_shininess[] = { 100.0 };

  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

  //Light0 features
  float light_diffuse0[]  = { light0Lum, light0Lum, light0Lum, light0Lum };
  float light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };

  //Light1 features
  float light_diffuse1[]  = { light1Lum, light1Lum, light1Lum, light1Lum };
  float light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };

  //Lights positions
  float light_position0[] = { light0Pos.x, light0Pos.y, light0Pos.z, 0.0 };
  float light_position1[] = { light1Pos.x, light1Pos.y, light1Pos.z, 0.0 };

  //Apply lights positions
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

  //Apply lights features
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR,light_specular0);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light_specular1);

  //Clear the background
  glClearColor (c.red (), c.green (), c.blue (), c.alpha ());

  return winID;
}

void cgutils::destroyWindow(int inWindowId)
{
  if (inWindowId>0)
    glutDestroyWindow(inWindowId);
}

void cgutils::drawString (const Point& inPoint, FontType font, const string& str, const Color& c)
{
  glColor4f (c.red (), c.green (), c.blue (), c.alpha());
  glRasterPos2f (inPoint.getX(), inPoint.getY());

  // TODO: anchor text in the upper left corner instead of lower left!

  for (string::const_iterator str_it = str.begin ();
       str_it != str.end ();
       str_it++)
  {
    char ch = *str_it;

    glutBitmapCharacter(font, ch);
  }
}

void cgutils::drawMultiLineString (Point& inPoint, FontType font, const string& str, const Color& c, const int32_t lineWidth, const int lineOffset)
{
  int32_t currentLength = 0;
  int i = 0, charLen;

  glColor4f (c.red (), c.green (), c.blue (), c.alpha());
  glRasterPos2f (inPoint.getX(), inPoint.getY());

  // TODO: anchor text in the upper left corner instead of lower left!

  for (string::const_iterator str_it = str.begin ();
       str_it != str.end ();
       str_it++)
  {
    char ch = *str_it;

    charLen = glutBitmapLength(font, (const unsigned char*)(str.substr(i++, 1).c_str()));
    currentLength += charLen;

    if(currentLength > lineWidth)
    {
      currentLength = charLen;
      inPoint.moveBy(Point(0, lineOffset));
      glRasterPos2f (inPoint.getX(), inPoint.getY());
    }

    glutBitmapCharacter(font, ch);
  }
}

void cgutils::drawString3D (const Vector3f& pos, FontType font, const string& str, const Color& c)
{
  glColor4f (c.red (), c.green (), c.blue (), c.alpha());
  glRasterPos3f (pos.x, pos.y, pos.z);

  for (string::const_iterator str_it = str.begin ();
       str_it != str.end ();
       str_it++)
  {
    char ch = *str_it;

    glutBitmapCharacter(font, ch);
  }
}

void cgutils::redisplay ()
{
  glutPostRedisplay();
}

void cgutils::swapBuffers ()
{
  glutSwapBuffers ();
}

void cgutils::mainLoop ()
{
  glutMainLoop();
}

void cgutils::enableBlend ()
{
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cgutils::enableLineSmoothing()
{
  glEnable(GL_LINE_SMOOTH);
}

void cgutils::disableLineSmoothing()
{
  glDisable(GL_LINE_SMOOTH);
}

void cgutils::disableBlend ()
{
  glDisable(GL_BLEND);
}

void cgutils::enableScissor (const Rect& box)
{
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLint &h = viewport[3];

  glEnable (GL_SCISSOR_TEST);
  glScissor (box.getX (), h - box.getY () - box.getHeight (),
             box.getWidth (), box.getHeight ());
}

void cgutils::disableScissor ()
{
  glDisable (GL_SCISSOR_TEST);
}

#ifndef USE_VERSATILE_TEXTURES
/* int setupGLTexture char* int int int
 * Function loads image from buffer into
 * OpenGL texture.
 */
unsigned int cgutils::setupGLTexture (ImageData &image_data)
{
  GLuint textureID;

  if (image_data.getData () == NULL)
  {
    return 0;
  }

  glGenTextures(1, &textureID);

  /* create a new texture object
   * and bind it to texname (unsigned integer > 0)
   */
  glBindTexture(GL_TEXTURE_2D, textureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  if (image_data.hasAlpha ())
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image_data.getWidth (), image_data.getHeight (), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image_data.getData ());
  }
  else
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image_data.getWidth (), image_data.getHeight (), 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image_data.getData ());
  }

  return textureID;
}

void cgutils::deleteGLTexture (GLuint textureID)
{
  glDeleteTextures (1, &textureID);
}
#endif // !USE_VERSATILE_TEXTURES

// TODO: temporary broken; fix it!
int cgutils::getFontWidth (FontType font)
{
  if (font == GLUT_BITMAP_8_BY_13)
    return 13;

  if (font == GLUT_BITMAP_9_BY_15)
    return 15;

  if (font == GLUT_BITMAP_TIMES_ROMAN_10)
    return 10;

  if (font == GLUT_BITMAP_TIMES_ROMAN_24)
    return 24;

  if (font == GLUT_BITMAP_HELVETICA_10)
    return 10;

  if (font == GLUT_BITMAP_HELVETICA_12)
    return 12;

  if (font == GLUT_BITMAP_HELVETICA_18)
    return 18;

  return 0;
}

int cgutils::getFontLength (FontType font, const string& str)
{
  return glutBitmapLength(font, (unsigned char *) str.c_str());
}

/*
void DrawRoundedSquareFill(float x, float y, float radius, float width, float height, float r, float g, float b)
{
  float cX = x+radius, cY = y+radius;
  double i=0;
  double gap = .05;

  glColor4f(r, g, b, .6);
  glBegin(GL_POLYGON);
  glVertex2f(x, y + height - radius);//Left Line
  glVertex2f(x, y + radius);

  for(i = PI; i <= (1.5*PI); i += gap)
  {
    glVertex2d(radius* cos(i) + cX, radius * sin(i) + cY); //Top Left
  }

  glVertex2f(x + radius, y);
  glVertex2f(x + width - radius, y);//Top Line

  cX = x+width-radius;
  for(i = (1.5*PI); i <= (2 * PI); i += gap)
  {
    glVertex2d(radius* cos(i) + cX, radius * sin(i) + cY); //Top Right
  }

  glVertex2f(x + width, y + radius);
  glVertex2f(x + width, y + height - radius);//Right Line

  cY = y+height-radius;
  for(i = 0; i <= (0.5*PI); i+=gap)
  {
    glVertex2d(radius* cos(i) + cX, radius * sin(i) + cY); //Bottom Right
  }
  glVertex2f(x + width - radius, y + height);//Bottom Line
  glVertex2f(x + radius, y + height);


  cX = x+radius;
  for(i = (0.5*PI); i <= PI; i += gap)
  {
    glVertex2d(radius* cos(i) + cX, radius * sin(i) + cY);//Bottom Left
  }
  glEnd();
}
*/
/*
void DrawCircle (){
  float angle;
  int i;
  GLint circle_points = 100;
  glColor4f(1, 0, 0, .8);
  glBegin(GL_LINE_LOOP);

  for (i = 0; i < circle_points; i++) {
    angle = 2*PI*i/circle_points;
    glVertex2f(cos(angle), sin(angle));
  }

  glEnd();
}
*/


//function which draws an axis aligned wireframe bounding box, given two points.
void cgutils::mhWireCube(const float twoxyz[6])
{
  float sx, sy, sz, tx, ty, tz; //scale and translate the glutWireCube(1)
  sx = twoxyz[3] - twoxyz[0];
  sy = twoxyz[4] - twoxyz[1];
  sz = twoxyz[5] - twoxyz[2];

  tx = twoxyz[0] + sx/2;
  ty = twoxyz[1] + sy/2;
  tz = twoxyz[2] + sz/2;

  const Color box_color (0.8, 0.8, 0.8, 0.75);

  char strz[100], strx[100] , stry[100];
  // conversions int to char[]. units?(these are not meters! we keep this value anyway.)
  snprintf(strz, sizeof(strz), "D = %5.2f", sz * 10);
  snprintf(strx, sizeof(strx), "W = %5.2f", sx * 10);
  snprintf(stry, sizeof(stry), "H = %5.2f", sy * 10);

  cgutils::enableBlend ();
  cgutils::enableLineSmoothing ();
  glDisable (GL_LIGHTING);

  // Set the color for the bounding box
  ::glColor4fv  (box_color.getAsOpenGLVector());

  glPushMatrix();
  glTranslatef(tx, ty, tz);
  glScalef(sx, sy, sz);
  glutWireCube(1);
  glPopMatrix();

  glPushMatrix();

  //draws the D quote
  const Vector3f posz(twoxyz[3] + 0.5, twoxyz[4] + 0.5, tz);
  drawString3D(posz, GLUT_BITMAP_9_BY_15, strz, box_color);

  //draws the W quote
  const Vector3f posx(tx - 0.5, twoxyz[4] + 0.75, twoxyz[5] + 0.5);
  drawString3D(posx, GLUT_BITMAP_9_BY_15, strx, box_color);

  //draws the H
  const Vector3f posy(twoxyz[3] + 1.5, ty, twoxyz[5] + 0.5);
  drawString3D(posy, GLUT_BITMAP_9_BY_15, stry, box_color);

  glPopMatrix();
  glFlush ();

  glEnable (GL_LIGHTING);
  cgutils::disableLineSmoothing ();
  cgutils::disableBlend();
}
