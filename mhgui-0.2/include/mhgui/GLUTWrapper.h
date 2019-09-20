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
 *  File   : GLUTWrapper.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef GLUTWRAPPER_H
#define GLUTWRAPPER_H 1

#if defined(__APPLE__) && defined(__MACH__)
  #ifdef USE_FREEGLUT
    #include <GLUT/freeglut.h>
  #else
    #include <GLUT/glut.h>
  #endif
#else
  #ifdef USE_FREEGLUT
    #include <GL/freeglut.h>
  #else
    #include <GL/glut.h>
  #endif
#endif

#if !defined(GLUT_WHEEL_UP)
  #define GLUT_WHEEL_UP    3
  #define GLUT_WHEEL_DOWN  4
#endif

#if !defined(GLUT_WHEEL_RIGHT)
  #define GLUT_WHEEL_RIGHT 5
  #define GLUT_WHEEL_LEFT  6
#endif

#endif //GLUTWRAPPER_H
