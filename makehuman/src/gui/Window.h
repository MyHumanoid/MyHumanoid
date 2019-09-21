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
 *  File   : Window.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef WINDOW_H
#define WINDOW_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <list>
#include <string>
#include <animorph/Vector3.h>
#include <animorph/Color.h>
#include <animorph/Matrix.h>
#include "Component.h"
//#include "Console.h"
//#include "Panel.h"
#include "ImageData.h"
#include "Texture.h"
#include "Camera.h"

using std::string;
using std::list;

using namespace Animorph;

namespace mhgui {

class Console;
class Panel;
class Size;
class Point;

int getKeyModifiers();

/// Object oriented interface to a GLUT window
class Window : public Component
{

private:
  static Window *w_singleton;
  string        title;
  bool          fullscreen;
  Color         c;
  bool          image_loaded;
  float         light0Lum;
  float         light1Lum;
  Vector3f      light0Pos;
  Vector3f      light1Pos;

  Texture       texture;

  list <Panel*> panelList; ///< Panels contain widgets, we contain the panels
  int           panelListChangedCount;

  Camera  *inCamera;
  Console *console;
  int mWindowId; ///< The GLUT Window ID;

  //Constructor is private, because this is a singleton
  Window(const Size& inSize, const char* t, const Color& c);
  virtual ~Window ();

  Window &operator = (Window&);  // Disallowed
  Window (const Window&);        // Disallowed

  void drawConsole ();
  void drawPanels ();
public:
  static Window* createSingelton(size_t        inWidth,
                                 size_t        inHeight,
                                 const char   *inTitle,
                                 const Color&  inBgColor);

  static Window& instance () {return *w_singleton;}

  void drawBackground ();

  void setTitle (const string &t);

  //Some "shortcut functions" that wrap a series of OpenGL lines
  void initWindow ();
  //void toggleFullscreen (); --> this needs to be implemented!

  /// For a GLUT callback
  void reshape (const Size& inSize, const Camera& inCamera);

  void setCamera (Camera *p_camera);

  void setConsole (Console *p_console);
  Console* getConsole ();

  //main functions
  bool   addPanel (Panel *p/*, bool now = true*/);
  void   removePanel (Panel *p);
  Panel* getPanel (uint32_t inPanelId);

  bool isMouseOverPanel    (const Point& inMousePos);
  bool isMouseClickPanel   (const Point& inMousePos, int button, int state);
  bool isMouseDraggedPanel (const Point& inMousePos);
  bool isKeyTypePanel      (unsigned char key);

  void defaultDisplay ();

  // callbacks setter
  void setDisplayCallback (void (*display)(void));
  void setMouseCallback (void (*mouse) (int,int,int,int));
  void setTimerCallback (int inMillis, void (*inTimerCB)(int value), int inId);
  void setCloseCallback (void (*close)(void));


  // texture functions
  bool loadPNG (const string& filename);

  virtual void draw ();
  virtual void show ();
  virtual void hide ();
};

void mouseCallbackWrapper(int inButton, int inState, int inX, int inY);

} // namespace mhgui

#endif //WINDOW_H
