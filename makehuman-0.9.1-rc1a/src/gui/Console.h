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
 *  File   : Console.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef CONSOLE_H
#define CONSOLE_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <animorph/Color.h>
#include "Widget.h"
#include "ImageData.h"
#include "Point.h"
#include "Texture.h"
#include "GLUTWrapper.h"

#define LINE_SPACE             20
#define MAX_SPLASH_LINES       5

using std::string;
using Animorph::Color;

typedef void* FontType;

namespace mhgui {

class Point;

/// Command prompt widget
class Console : public Widget
{
protected:
  //Texture
  Texture     texture;

  Color backgroundColor; // set background transparency with alpha component of this variable
  Color foregroundColor;
  Color cmdForegroundColor;
  Color splashForegroundColor;

  string command;        // command code
  string commandLine;    // full command line
  string inputMessage;   // input text request
  string message;        // op result message
  string userText;       // user input

  string commandPrompt;
  unsigned int commandPromptLength;

  list <string> splashLines;

  bool image_loaded;
  unsigned int userTextMaxLength;
  unsigned int status;
  bool inError;

  Point cursorPos;

  void drawSplashInfo();
  void drawPrompt();
  void drawCommandLine();
  void drawInputMessage();
  void drawMessage();
  void clear();
public:
           Console (uint32_t);
  virtual ~Console () {}

  enum Status
  {
    PROMPT,
    INPUT,
    MESSAGE,
    INPUT_MESSAGE
  };

  void setBackgroundColor (Animorph::Color c);
  void setForegroundColor (Animorph::Color c);
  void setCmdForegroundColor (Animorph::Color c);
  void setSplashForegroundColor (Animorph::Color c);

  void setUserTextMaxLength (unsigned int len);

  void close ();
  void open ();
  void openWithCommand(const string& inCmd, const string& inMessage, const string& defUserText);
  void printMessage(const string& msg);

  void addUserText (const char text);
  void removeUserText();
  void setUserText (const string& text);
  const string getUserText() const;
  void setCommand (const string& text);
  const string getCommand() const;
  void setCommandLine (const string& text);
  void setInputMessage (const string& text);
  void setMessage (const string& text);
  void setStatus (unsigned int inStatus);
  const unsigned int getStatus() const;
  void setError(const bool inErr) {inError = inErr;}
  const bool isInError() const {return inError;}

  void setCommandPrompt (const string& inCommandPrompt);
  void inputMode(const string& inputMessage, const string& defaultText);

  void addSplashLine(const string& line);
  void clearSplash();

  void retryCommand();

  virtual bool isMouseOver    (const Point& inMousePos);
  virtual bool isMouseClick   (const Point& inMousePos, int button, int state);
  virtual bool isMouseDragged (const Point& inMousePos);

  const Texture& getTextures(); //return the texture object
  void loadPNG (const string& filename);

  bool acceptUserInput();

  virtual void show ();
  virtual void hide ();
  virtual void draw ();
  virtual void drawOverlay ();

private:
  Console             (const Console&);
  Console& operator = (const Console&);

  FontType textFont;
};

} // namespace mhgui

#endif //CONSOLE_H
