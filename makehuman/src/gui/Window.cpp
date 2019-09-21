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
 *  File   : Window.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <GL/freeglut.h>

#include <imgui.h>
#include <examples/imgui_impl_glut.h>
#include <examples/imgui_impl_opengl2.h>

#include "gui/Size.h"
#include "gui/Window.h"
#include "gui/Camera.h"
#include "gui/CGUtilities.h"

#include <iostream>
#include "gui/Panel.h"
#include "gui/Console.h"


using namespace std;
using namespace Animorph;

namespace mhgui {

// Remember the key modifier within the glutMouseFunc callback
static int sKeyModifier = 0; // No modifier pressed at init

// Remember the users glutMouseFunc callback
static void (*sMouseFuncCB)(int,int,int,int);

/* ========================================================================== */
/** Trap the glutMouseFunc in order to remember which Key modifier has been
 *  pressed when a mousbutton has been pressed / released.
 *  This method wraps the users glutMouseFunc callback hook either to mime the
 *  expected semantics.
 *
 *  This wrapper is neccessary because glut allows to call getKeyModifiers()
 *  only within three particular callback functions. These are keyboard,
 *  special, or the mouse callback!
 *
 * Note that the neither the keyboard nor the special callback is triggered when
 * a keyboard modifier is altered! So we are using the mouse callback instead.
 */
/* ========================================================================== */
void mouseCallbackWrapper(int inButton, int inState, int inX, int inY)
{
  // remember the key modifier actually pressed
  sKeyModifier = glutGetModifiers();

  // If there is any custom MouseFunc callback registered, then call it.
  if (sMouseFuncCB != NULL)
  {
    sMouseFuncCB(inButton, inState, inX, inY);
  }
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
int getKeyModifiers()
{
  // Return the last remembered keyboard modifier (see mouseCallbackWrapper)
  return sKeyModifier;
}


Window *Window::w_singleton = NULL;

Window* Window::createSingelton(size_t        inWidth,
                                size_t        inHeight,
                                const char   *inTitle,
                                const Color&  inBgColor)
{
    if (w_singleton == NULL)
    {
        w_singleton = new mhgui::Window(Size(inWidth, inHeight) , inTitle, inBgColor);
    }
    return w_singleton;
}

//Constructor
Window::Window (const Size& inSize, const char* t, const Color& inColor)
    : Component(FOUR_CHAR_CONST('M','W','i','n'), Rect(0,0,inSize.getWidth(), inSize.getHeight())),
    title(t), // The window title bar
    fullscreen(false), //full screen no
    c(inColor),
    image_loaded(false),
    light0Lum(+1.0),                // light 0 luminosity
    light1Lum(+1.0),                // light 1 luminosity
    light0Pos(+1.0, +1.0, +1.0),    // light 0 position
    light1Pos(-1.0, +1.0, +1.0),    // light 1 position

    texture(),

    panelList(),
    panelListChangedCount(0),

    inCamera(NULL),
    console(NULL),  // console
    mWindowId(0)

{
}

Window::~Window ()
{
  cgutils::destroyWindow(mWindowId);
}

Panel* Window::getPanel (uint32_t inPanelId)
{
  for (list<Panel*>::const_iterator pl_it = panelList.begin ();
       pl_it != panelList.end ();
       pl_it++)
  {
    Panel* p_cmp = (*pl_it);

    if (inPanelId == p_cmp->getID())
    {
      return p_cmp;
    }
  }

  return NULL;
}

bool Window::loadPNG (const string& filename)
{
  if (filename.empty())
    return false;

  image_loaded = true;

  // read the PNG file using pngLoad
  // raw data from PNG file is in image buffer
  if (texture.load (filename) == false)
  {
    cerr << "(pngLoad) " << filename << " FAILED" << endl;
    return false;
  }
  return true;
}

void Window::setCamera (Camera *p_camera)
{
	inCamera = p_camera;
}

void Window::setConsole (Console *p_console)
{
  console = p_console;
}

Console* Window::getConsole ()
{
  return console;
}

//Callback setters
void Window::setDisplayCallback (void (*inDisplayCB)(void))
{
  glutDisplayFunc (inDisplayCB);
}

void Window::setCloseCallback (void (*inCloseCB)(void))
{
  glutCloseFunc (inCloseCB);
}

void Window::setMouseCallback (void (*inMouseCB) (int,int,int,int))
{
  sMouseFuncCB = inMouseCB;
}

void Window::setTimerCallback (int inMillis, void (*inTimerCB)(int value), int inId)
{
  glutTimerFunc (inMillis, inTimerCB, inId);
}

void Window::draw()
{
  drawBackground ();
  drawPanels ();
  drawConsole ();
}

void Window::show ()
{
  setVisible(true);
}


void Window::hide()
{
}

void Window::drawConsole ()
{
  if (isVisible())
  {
    if(console!=NULL && console->isActive ())
    {
      cgutils::enableOrthographicProjection ();
      console->draw ();
      cgutils::disableOrthographicProjection ();
    }
  }
}

//Add panel into window
bool Window::addPanel (Panel* p)
{
  if (p==NULL)
    return false;

  Panel *panel = dynamic_cast<Panel*>(p);
  // Just Panels are allowed!
  assert(panel);

  for (list<Panel*>::iterator pl_it = panelList.begin ();
       pl_it != panelList.end ();
       pl_it++)
  {
    Panel* p_cmp = (*pl_it);

    if (*p_cmp == *p)
    {
      cerr << "Error: Panel needs a unique name! "
      << "A Panel with name '" << p->getIDAsString() << "' does yet exist!"
      << endl;
      return false;
    }
  }

  panelList.push_back(panel);
  ++panelListChangedCount; // mark a change of the Panel List

  panel->setParentWindow(this);

  // do a reshape event for update
  reshape (getRect ().getSize(), *inCamera);

  return true;
}

void Window::removePanel (Panel *p)
{
  if (p==NULL)
    return;

  for (list<Panel*>::iterator pl_it = panelList.begin ();
       pl_it != panelList.end ();
       pl_it++)
  {
    Panel* p_cmp = (*pl_it);

    if (*p_cmp == *p)
    {
      panelList.erase(pl_it);
      ++panelListChangedCount; // mark a change of the Panel List
      return;
    }
  }
}

void Window::drawBackground ()
{
  if (isVisible())
  {
    glColor3f (c.red (), c.green (), c.blue ()); // TODO: wrap this!

    if (image_loaded)
      cgutils::drawBackgroundSquare (getSize(), 1.0f, texture);
  }
}

//For each panel, draw all widgets
void Window::drawPanels ()
{
  if (isVisible())
  {
    cgutils::enableOrthographicProjection ();

    for (list<Panel*>::const_iterator pl_it = panelList.begin ();
         pl_it != panelList.end ();
         pl_it++)
    {
      Panel *panel = dynamic_cast<Panel*>(*pl_it);
      assert(panel); // assert if this is not a Panel!

      // printf("Drawing Panel '%s'\n", panel->getIDAsString().c_str());
      panel->draw ();
    }
    cgutils::disableOrthographicProjection();
  }
};

void Window::setTitle (const std::string &t)
{
  // hm, this should change the GLUT window title, not?
  title = t;
}

//For each panel, check if mouse is over his widgets
bool Window::isMouseOverPanel (const Point& inMousePos)
{
  bool isOver = false;
  if(console!=NULL && console->isActive())
  {
    return console->isMouseOver(inMousePos);
  }

  int rememberedPanelListChangedCount = panelListChangedCount;

  for (list<Panel*>::reverse_iterator pl_it = panelList.rbegin ();
       pl_it != panelList.rend ();
       pl_it++)
  {
    Panel *panel = (*pl_it);

    isOver = panel->isMouseOverWidgets (inMousePos);
    if (isOver == true)
    {
      break;
    }

    isOver = panel->isMouseOver (inMousePos);
    if (isOver == true)
    {
      break;
    }

    /* Check if the panel List has been changed in between
     * (triggered by a Panel- or a Widget Listener for example)
     * If it has then "reinitialize" the iterator because it is
     * possible that it became invalide because of this change
     * of the list! */
    if (panelListChangedCount != rememberedPanelListChangedCount)
    {
        rememberedPanelListChangedCount = panelListChangedCount;
        pl_it = panelList.rbegin (); // reinitailize the iterator
    }
  }
  return isOver;
}

//For each panel, check if click is over his widgets
bool Window::isMouseClickPanel (const Point& inMousePos, int button, int state)
{
  bool isClick = false;
  if(console!=NULL && console->isActive ())
  {
    return console->isMouseClick (inMousePos, button, state);
  }

  int rememberedPanelListChangedCount = panelListChangedCount;

  for (list<Panel*>::reverse_iterator pl_it = panelList.rbegin ();
       pl_it != panelList.rend ();
       pl_it++)
  {
    Panel *panel = (*pl_it);

    isClick = panel->isMouseClickWidgets (inMousePos, button, state);
    if (isClick == true)
    {
      break;
    }

    isClick = panel->isMouseClick (inMousePos, button, state);
    if (isClick == true)
    {
      break;
    }

    /* Check if the panel List has been changed in between
     * (triggered by a Panel- or a Widget Listener for example)
     * If it has then "reinitialize" the iterator because it is
     * possible that it became invalide because of this change
     * of the list! */
    if (panelListChangedCount != rememberedPanelListChangedCount)
    {
        rememberedPanelListChangedCount = panelListChangedCount;
        pl_it = panelList.rbegin (); // reinitailize the iterator
    }
  }

  return isClick;
}

//For each panel, check if a widget is active
bool Window::isKeyTypePanel (unsigned char key)
{
  bool keyType = false;
  if(console!=NULL && console->isActive ())
  {
    return console->isKeyType (key);
  }

  int rememberedPanelListChangedCount = panelListChangedCount;

  for (list<Panel*>::reverse_iterator pl_it = panelList.rbegin ();
       pl_it != panelList.rend ();
       pl_it++)
  {
    Panel *panel = (*pl_it);

    keyType = panel->isKeyTypeWidgets (key);
    if (keyType == true)
    {
      break;
    }

    keyType = panel->isKeyType (key);
    if (keyType == true)
    {
      break;
    }

    /* Check if the panel List has been changed in between
     * (triggered by a Panel- or a Widget Listener for example)
     * If it has then "reinitialize" the iterator because it is
     * possible that it became invalide because of this change
     * of the list! */
    if (panelListChangedCount != rememberedPanelListChangedCount)
    {
        rememberedPanelListChangedCount = panelListChangedCount;
        pl_it = panelList.rbegin (); // reinitailize the iterator
    }
  }
  return keyType;
}

bool Window::isMouseDraggedPanel (const Point& inMousePos)
{
  bool dragged = false;
  if(console!=NULL && console->isActive ())
  {
    return console->isMouseDragged (inMousePos);
  }

  int rememberedPanelListChangedCount = panelListChangedCount;

  for (list<Panel*>::reverse_iterator pl_it = panelList.rbegin ();
       pl_it != panelList.rend ();
       pl_it++)
  {
    Panel *panel = (*pl_it);

    dragged = panel->isMouseDraggedWidgets (inMousePos);
    if (dragged == true)
    {
      break;
    }

    dragged = panel->isMouseDragged (inMousePos);
    if (dragged == true)
    {
      break;
    }

    /* Check if the panel List has been changed in between
     * (triggered by a Panel- or a Widget Listener for example)
     * If it has then "reinitialize" the iterator because it is
     * possible that it became invalide because of this change
     * of the list! */
    if (panelListChangedCount != rememberedPanelListChangedCount)
    {
        rememberedPanelListChangedCount = panelListChangedCount;
        pl_it = panelList.rbegin (); // reinitailize the iterator
    }
  }
  return dragged;
}

//Init window with some classic openGL commands
void Window::initWindow ()
{
  mWindowId = cgutils::initWindow ( getSize(), title.c_str () ,
                                    light0Pos, light1Pos,
                                    light0Lum, light1Lum, c);
}

//Glut call back functions
void Window::reshape (const Size& inSize, const Camera& inCamera)
{
  cgutils::reshape (inSize, inCamera);
  if (console!=NULL)
  {
    console->setSize(Size(inSize.getWidth(),inSize.getHeight()/2));
  }

  for (list<Panel*>::const_iterator pl_it = panelList.begin ();
       pl_it != panelList.end ();
       pl_it++)
  {
    Panel *panel = (*pl_it);

    if (panel->getMaximize ())
    {
      Rect r (0,0,0,0);
      r.resizeTo (inSize);
      panel->setRect (r);
    }
    else
    {
      if (panel->getBottomAnchor ())
      {
        int bottom_old = getSize().getHeight() - panel->getPosition().getY();
        int bottom = inSize.getHeight() - bottom_old;

        panel->setPosition(Point (panel->getPosition().getX(), bottom));
      }
      if (panel->getRightAnchor ())
      {
        int right_old = getSize().getWidth() - panel->getPosition().getX();
        int right = inSize.getWidth() - right_old;
        panel->setPosition(Point (right, panel->getPosition().getY()));
      }
    }

    panel->calcWidgetPosition ();
  }

  setSize(inSize);
}

} // namespace mhgui


