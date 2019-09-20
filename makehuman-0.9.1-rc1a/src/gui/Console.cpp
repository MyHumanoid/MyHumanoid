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
 *  File   : Console.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Console.h"
#include "gui/CGUtilities.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace Animorph;
using namespace mhgui;

namespace mhgui {

//constructor
Console::Console (uint32_t consoleID)
  : Widget(consoleID, Rect(0, 0, 1, 1)),
    texture(),
    backgroundColor(0.0, 0.0, 0.0, 0.8), // set background transparency with alpha component of this variable
    foregroundColor(1.0, 1.0, 1.0),
    cmdForegroundColor(0.0, 1.0, 1.0),
    splashForegroundColor(1.0, 1.0, 0.0),
    command(),
    commandLine(),
    inputMessage(),
    message(),
    userText(),
    commandPrompt(">> "),
    commandPromptLength(30),
    image_loaded(),
    userTextMaxLength(255),
    status(0),
    inError(false),
    cursorPos (0, 20),
    textFont(GLUT_BITMAP_HELVETICA_18)
{
}

void Console::setCommandPrompt (const string& inCommandPrompt)
{
  commandPrompt = inCommandPrompt;
  commandPromptLength = glutBitmapLength(textFont, (const unsigned char*)(commandPrompt.c_str()));
}

void Console::setCommand(const string& text)
{
  command = text;
}

void Console::setCommandLine(const string& text)
{
  commandLine = text;
}

void Console::setInputMessage(const string& text)
{
  inputMessage = text;
}

void Console::setMessage(const string& text)
{
  message = text;
}

void Console::addUserText (const char text)
{
  if (userText.size () < userTextMaxLength)
    userText += text;
}

void Console::removeUserText ()
{
  if (userText.size ())
    userText.erase (userText.size () - 1);
}

void Console::setUserText (const string& text)
{
  userText = text;
}

const string Console::getUserText () const
{
  return userText;
}

const string Console::getCommand () const
{
  return command;
}

void Console::setStatus(unsigned int inStatus)
{
  if(status == PROMPT && inStatus != PROMPT)
  {
    commandLine = getUserText();
    setUserText("");
  }
  status = inStatus;
}

const unsigned int Console::getStatus () const
{
  return status;
}

void Console::setUserTextMaxLength (unsigned int len)
{
  userTextMaxLength = len;
}

void Console::open ()
{
  if(!isActive())
  {
    clear();
    setActive(true);
    cgutils::redisplay();
  }
}

void Console::clear()
{
  setUserText("");
  setCommand("");
  setCommandLine("");
  setInputMessage("");
  setMessage("");
  setUserText("");
  setStatus(PROMPT);
  inError = false;
}

bool Console::acceptUserInput()
{
  return (status == INPUT || status == PROMPT) ? true : false;
}

void Console::openWithCommand(const string& inCmd, const string& inMessage, const string& defUserText)
{
  if(!isActive())
  {
    clear();
    setStatus(INPUT);
    setCommand(inCmd);
    setCommandLine(inCmd);
    setInputMessage(inMessage);
    setUserText(defUserText);
    setActive(true);
    cgutils::redisplay();
  }
}

void Console::retryCommand()
{
    setStatus(INPUT);
    cgutils::redisplay();
}

void Console::printMessage(const string& msg)
{
  setMessage(msg);
  setStatus(getStatus() == INPUT ? INPUT_MESSAGE : MESSAGE);
}

void Console::close ()
{
  if(isActive())
  {
    setActive(false);
    cgutils::redisplay();
  }
}

// background color setter
void Console::setBackgroundColor (Animorph::Color c)
{
  backgroundColor = c;
}

// foreground color setter
void Console::setForegroundColor (Animorph::Color c)
{
  foregroundColor = c;
}

// command foreground color setter
void Console::setCmdForegroundColor (Animorph::Color c)
{
  cmdForegroundColor = c;
}

// splash foreground color setter
void Console::setSplashForegroundColor (Animorph::Color c)
{
  splashForegroundColor = c;
}


void Console::loadPNG (const string& filename)
{
  if (filename.empty())
    return;

  image_loaded = true;
  //cout << "load: " << filename << endl;

  // read the PNG file using pngLoad
  // raw data from PNG file is in image buffer

  if (texture.load(filename) == false)
  {
    cerr << "(pngLoad) %s FAILED" << filename << endl;
  }
}

bool Console::isMouseDragged (const Point& inMousePos)
{
  return false;
}

//Check if mouse over, and use the listener mouseover function
bool Console::isMouseOver (const Point& inMousePos)
{
  return false;
}

//Check if mouse click, and use the listener mousepressed or mousereleased function
bool Console::isMouseClick (const Point& inMousePos, int button, int state)
{
  return false;
}

void Console::show ()
{
  setVisible(true);

  if (image_loaded)
  {
#ifndef USE_VERSATILE_TEXTURES
    textureID = cgutils::setupGLTexture (image_data);
#endif
  }
}

void Console::hide ()
{
  setVisible(false);
}

const Texture& Console::getTextures ()
{
  return texture;
};

void Console::drawOverlay ()
{

}

void Console::addSplashLine(const string& line)
{
  if(splashLines.size() >= MAX_SPLASH_LINES)
  {
    cerr << "splahs lines limit (" << MAX_SPLASH_LINES << ") exceeded" << endl;
  }
  else
  {
    splashLines.push_back(line);
  }
}

void Console::inputMode(const string& inputMessage, const string& defaultText)
{
  setStatus(Console::INPUT);
  setInputMessage(inputMessage);
  setUserText(defaultText);
}

void Console::clearSplash()
{
  splashLines.clear();
}

void Console::drawSplashInfo()
{
  for (list<string>::const_iterator sl_it = splashLines.begin ();
         sl_it != splashLines.end ();
         sl_it++)
  {
    cgutils::drawString (cursorPos, textFont, *sl_it, splashForegroundColor);
    cursorPos.moveBy(Point(0, LINE_SPACE));
  }
}

void Console::drawPrompt()
{
  cgutils::drawString(cursorPos, textFont, commandPrompt, cmdForegroundColor);
  cursorPos.moveBy(Point(commandPromptLength, 0));

  cgutils::drawMultiLineString(cursorPos, textFont, userText, foregroundColor, getSize().getWidth() - commandPromptLength, LINE_SPACE);
  cursorPos.moveBy(Point(-commandPromptLength, LINE_SPACE));
}

void Console::drawCommandLine()
{
  cgutils::drawString(cursorPos, textFont, commandPrompt, cmdForegroundColor);
  cursorPos.moveBy(Point(commandPromptLength, 0));
  cgutils::drawString(cursorPos, textFont, commandLine, foregroundColor);
  cursorPos.moveBy(Point(-commandPromptLength, LINE_SPACE));
}

void Console::drawInputMessage()
{
  cgutils::drawString(cursorPos, textFont, inputMessage, cmdForegroundColor);
  cursorPos.moveBy(Point(0, LINE_SPACE));
}

void Console::drawMessage()
{
  cgutils::drawString(cursorPos, textFont, message, cmdForegroundColor);
  cursorPos.moveBy(Point(0, LINE_SPACE));
}

//draw function
void Console::draw ()
{
  if (isVisible())
  {
    cursorPos = Point(0, 20);

    cgutils::enableBlend ();
    if (image_loaded != 0)
    {
      cgutils::drawSquareFillTexture (getRect(), backgroundColor.alpha (), texture);
    }
    else
    {
      cgutils::drawSquareFill (getRect(), backgroundColor);
    }
    cgutils::disableBlend ();

    drawSplashInfo();

    switch(status)
    {
      case PROMPT:
           drawPrompt();
           break;
      case INPUT:
           drawCommandLine();
           drawInputMessage();
           drawPrompt();
           break;
      case MESSAGE:
           drawCommandLine();
           drawMessage();
           break;
      case INPUT_MESSAGE:
           drawCommandLine();
           drawInputMessage();
           drawPrompt();
           drawMessage();
           break;
    }
  }
}

} // namespace mhgui

