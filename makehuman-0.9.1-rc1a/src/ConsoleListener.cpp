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
 *  File   : ConsoleListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <animorph/ObjExporter.h>
#include <animorph/ColladaExporter.h>
#include <animorph/BodySettings.h>
#include <gui/Console.h>
#include <gui/CGUtilities.h>
#include <gui/Window.h>
#include <gui/Component.h>
#include <gui/GLUTWrapper.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "util.h"
#include "ConsoleCommands.h"
#include "ConsoleListener.h"
#include "TargetPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include "util.h"
#include "FileTools.h"
#include "CharacterSettingPanel.h"

#ifdef __APPLE__
  #include "FileTools.h" // Move this out when tested on Linux and WIN32 too!
#endif

using namespace std;
using namespace mhgui;

ConsoleListener::ConsoleListener()
: AbstractListener()
{
}

ConsoleListener::~ConsoleListener()
{
}

bool ConsoleListener::mouseOver (const Point& inMousePos, Component *source)
{
  return true;
}

bool ConsoleListener::mouseOut (const Point& inMousePos, Component *source)
{
  return true;
}

bool ConsoleListener::mouseDragged (const Point& inMousePos, Component *source)
{
  return true;
}

bool ConsoleListener::mousePressed (const Point& inMousePos, int button, Component *source)
{
  return true;
}

bool ConsoleListener::mouseWheel    (const Point& inMousePos, int inButton, Component *source )
{
  return false;
}

bool ConsoleListener::mouseReleased (const Point& inMousePos, int button, Component *source)
{
  return true;
}

bool ConsoleListener::keyType (unsigned char key, Component *source)
{
  Console *console = dynamic_cast<Console *>(source); // req. RTTI!
  assert(console); // Check if this is really a Console object?

  if(console->acceptUserInput())
  {
    switch (toupper(key))
    {
      case '\r':
        parseCommand(*console);
        break;
      case '\b':
        console->removeUserText();
        break;
      case '\t':
        console->addUserText(' ');
        console->addUserText(' ');
        console->addUserText(' ');
        break;
      case 27:
        console->close();
        break;
      default:
        console->addUserText(key);
        //std::cout << console->getSize().getWidth() << std::endl;
    }
  }
  else
  {
    if(toupper(key) == 27)
    {
      if(!Global::instance().isRendering())
      {
        console->close();
      }
    }
    else if(toupper(key) == '\r' && console->isInError())
    {
      console->retryCommand();
    }
  }
  return true;
}

void ConsoleListener::parseCommand(Console& console)
{
  int i;
  string cmd;
  string arg;
  const string& line = console.getUserText();

  Global &global = Global::instance ();

  if(console.getCommand() == kConsoleCommand_Exit)
  {
    ExportConfigurationXML();
    exit(0);
  }

  if(!line.empty())
  {
    if(console.getStatus() == Console::PROMPT)
    {
      i = line.find_first_of(' ');
      if(i == -1)
      {
        cmd = line;
        arg = "";
      }
      else
      {
        cmd = line.substr(0, i);
        arg = line.substr(i + 1, line.length() - i);
      }
      console.setCommand(cmd);
    }
    else if(console.getStatus() == Console::INPUT)
    {
      cmd = console.getCommand();
      arg = line;
    }

    if(cmd == kConsoleCommand_Load_Background)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleMessage_Load_Background, getMyBackgroundsPath());
      }
      else
      {
        loadWindowBackground(console, arg);
      }
    }else if(cmd == kConsoleCommand_Load_AqsisPath)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleCommand_Load_AqsisPath, GetDefaultAqsisPath());
      }
      else
      {
        loadAqsisPath(console, arg);
      }
    } else if(cmd == kConsoleCommand_Load_PixiePath)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleCommand_Load_PixiePath, GetDefaultPixiePath());
      }
      else
      {
        loadPixiePath(console, arg);
      }
    } else if(cmd == kConsoleCommand_Load_Bodysettings)
    {
      if(global.getAppMode() == BODY_DETAILS ||
             global.getAppMode() == CHARACTER_SETTING ||
             global.getAppMode() == CLOTHES)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Load_Bodysettings, getMyBodysettingsPath());
        }
        else
        {
          loadBodySettings(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_BodyDetails);
      }
    }
    else if(cmd == kConsoleCommand_Load_Poses)
    {
      if(global.getAppMode() == POSES)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Load_Poses, getMyPosesPath());
        }
        else
        {
          loadPoses(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_Poses);
      }
    }
    else if(cmd == kConsoleCommand_Save_Bodysettings)
    {
      if(global.getAppMode() == BODY_DETAILS ||
             global.getAppMode() == CHARACTER_SETTING ||
             global.getAppMode() == CLOTHES)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Save_Bodysettings, getMyBodysettingsPath());
        }
        else
        {
          saveBodySettings(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_BodyDetails);
      }
    }
    else if(cmd == kConsoleCommand_Save_Poses)
    {
      if(global.getAppMode() == POSES)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Save_Poses, getMyPosesPath());
        }
        else
        {
          savePoses(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_Poses);
      }
    }
    else if(cmd == kConsoleCommand_Save_Autozoom)
    {
	    if(arg.size() == 0)
	    {
		    Window& mainWindow(Window::instance());
		    TargetPanel* targetPanel = dynamic_cast<TargetPanel*>(mainWindow.getPanel (kComponentID_TargetPanel));
		    string target = targetPanel->getCategory();
		    std::string pathAutozoom_data = searchDataDir ("targets") + "/" + target + "/" + target + ".camera";
		    console.inputMode(kConsoleMessage_Save_Autozoom, pathAutozoom_data);
	    }
	    else
	    {
		    saveAutozoom(console, arg);
	    }
    }
    else if(cmd == kConsoleCommand_Export_Object)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleMessage_Export_Object, getMyObjPath());
      }
      else
      {
        exportBodySettings(console, arg);
      }
    }
    else if(cmd == kConsoleCommand_Export_Full_Object)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleMessage_Export_Object, getMyObjPath());
      }
      else
      {
        exportBodySettings(console, arg, true);
      }
    }
    else if(cmd == kConsoleCommand_Export_Collada)
    {
      if(arg.size() == 0)
      {
        console.inputMode(kConsoleMessage_Export_Collada, getMyColladaPath());
      }
      else
      {
        exportCollada(console, arg);
      }
    }
    else if(cmd == kConsoleCommand_Set_Parameter)
    {
      if(arg.size() == 0)
      {

      }
      else
      {
        parseSetParameter(console, arg);
      }
    }else if(cmd == kConsoleCommand_Create_WeightsMatrix){

      if(arg.size() == 0)
      {
         CreateWeightsFile();

      }
      else
      {

      }
    }
#ifdef _ANIMATIONS
    else if(cmd == kConsoleCommand_Load_Animations)
    {
      if(global.getAppMode() == ANIMATIONS)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Load_Animations, getMyPosesPath());
        }
        else
        {
          loadAnimation(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_Animations);
      }
    }
#endif
/*
    else if(cmd == kConsoleCommand_Save_Animations)
    {
      if(global.getAppMode() == ANIMATIONS)
      {
        if(arg.size() == 0)
        {
          console.inputMode(kConsoleMessage_Save_Animations, getMyPosesPath());
        }
        else
        {
          saveAnimation(console, arg);
        }
      }
      else
      {
        console.printMessage(kConsoleMessage_WrongMode_Animations);
      }
    }
*/
    else
    {
      commandNotFound(console);
    }
  }
}

void ConsoleListener::commandNotFound(Console& console)
{
  console.setMessage(kConsoleMessage_Unknown_Command);
  console.setStatus(Console::MESSAGE);
}

void ConsoleListener::loadAnimation(Console& console, const string& path)
{
  Global &global = Global::instance ();
  Animation *animation = global.getAnimation();
  assert (animation);

  bool state = animation->load(path);
  if(state)
  {
    console.printMessage(kConsoleMessage_Load_Animations_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Dir_Load_Error);
    console.setError(true);
  }
}
/*
void ConsoleListener::startStopAnimation(Console& console)
{
  Global &global = Global::instance ();
  Animation *animation = global.getAnimation();
  assert (animation);

  if(animation->isLoaded())
  {
    animation->setStarted(!animation->isStarted());
  }
  else
  {
   console.printMessage(kConsoleMessage_NotLoaded_Animations);
   console.setError(true);
  }
}
*/
/*
void ConsoleListener::saveAnimation(Console& console, const string& path)
{
  Global &global = Global::instance ();
  Animation *animation = global.getAnimation();
  assert (animation);

  bool state = animation->save(path);
  if(state)
  {
    console.printMessage(kConsoleMessage_Save_Animations_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Dir_Save_Error);
    console.setError(true);
  }
}
*/

void ConsoleListener::loadWindowBackground(Console& console, const string& filename)
{
  Window& mainWindow(Window::instance());
  if(mainWindow.loadPNG (filename))
  {
    mainWindow.show();
    console.printMessage(kConsoleMessage_Load_Background_Success);
  }
  else
  {
    mainWindow.loadPNG (searchPixmapFile ("ui/background.png"));
    console.printMessage(kConsoleMessage_Load_Error);
    console.setError(true);
  }
}

void ConsoleListener::loadBodySettings(Console& console, const string& filename)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);
  Window &mainWindow = Window::instance ();

  FaceGroup   &clothesgroup  (mesh->getClothesGroupRef ());

  BodySettings bodyset;
  bool state = bodyset.load (filename);
  if(state)
  {
    state = clothesgroup.loadVisibilities (filename);
  }

  if(state)
  {
    global.resetFuzzyValues();
    state = loadSelectorsPositions(filename);

    CharacterSettingPanel *tmpPanel = (CharacterSettingPanel *)mainWindow.getPanel(kComponentID_CharacterSettingPanel);
    if(tmpPanel != NULL)
    {
      tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Age);
      tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Breast);
      tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_MuscleSize);
      tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Shape);
    }
  }

  if (state)
  {
    mesh->doMorph (bodyset);
    mesh->calcNormals();
    if(global.getSubdivision())
    {
      mesh->calcSubsurf();
    }
    console.printMessage(kConsoleMessage_Load_Bodysettings_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Load_Error);
    console.setError(true);
  }
}

void ConsoleListener::loadPoses(Console& console, const string& filename)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);

  BodySettings poses;
  bool state = poses.load (filename);

  if (state)
  {
    mesh->doPose (poses);
    if(global.getSubdivision())
    {
      mesh->calcSubsurf();
    }
    console.printMessage(kConsoleMessage_Load_Poses_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Load_Error);
    console.setError(true);
  }
}

void ConsoleListener:: loadAqsisPath(Console& console, const string& path)
{
  int ret;
  if( (ret = SetAqsisBasePath(path + "/")) >= 0){
    saveRenderingPaths(path + "/",AQSIS);
    console.printMessage(kConsoleMessage_OK);

    ExportConfigurationXML();
  }else{
    switch(ret){
      case -1:
        console.printMessage(kConsoleMessage_InvalidPath);
        break;
      case -2:
        console.printMessage(kConsoleMessage_WrongPath);
        break;
      default:
        break;
    }
  }

}

void ConsoleListener::loadPixiePath(Console& console, const string& path)
{
  int ret;
  if( (ret = SetPixieBasePath(path + "/") ) >= 0){
    saveRenderingPaths(path + "/",PIXIE);
    console.printMessage(kConsoleMessage_OK);

    ExportConfigurationXML();
  }else{
    switch(ret){
      case -1:
        console.printMessage(kConsoleMessage_InvalidPath);
        break;
      case -2:
        console.printMessage(kConsoleMessage_WrongPath);
        break;
      default:
        break;
    }
  }

}

void ConsoleListener::parseSetParameter(Console& console, const string& path)
{
  int ret;

  if( (ret = ParseParameter(path))<0){
    switch(ret){
      case -3:
        console.printMessage(kConsoleMessage_InvalidPath);
        break;
      default:
        console.printMessage(kConsoleMessage_Parameter_NG);
        break;
    }

  }else{
    ExportConfigurationXML();
    console.printMessage(kConsoleMessage_Parameter_OK);
  }

}


void ConsoleListener::saveBodySettings(Console& console, const string& filename)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);

  BodySettings bodyset = mesh->getBodySettings ();
  FaceGroup   &clothesgroup  (mesh->getClothesGroupRef ());

#if defined(__APPLE__) && defined(__MACH__)

/* This piece of code has been tested on Mac OS X only!
 * On OS X the bodysettings (and all user specific settings as well) will be
 * are supposed to be saved to the Users Document folder in a directory named
 * makehuman.
 *
 * Since this directory does not exists the first time so we'll need to create
 * one.
 */

  // First create a directory at the given location if it does not exists

  string path(FileTools::getFilePath(filename));
  if (!FileTools::fileExists(path))
  {
    bool rc = FileTools::makeDirHier(path);
    assert(rc == true);
  }
#endif // MAC OS X specific code

  bool state = bodyset.save (filename);

  if(state)
  {
    state = clothesgroup.saveVisibilities (filename);
  }

  if(state)
  {
    state = saveSelectorsPositions (filename);
  }

  if (state)
  {
    console.printMessage(kConsoleMessage_Save_Bodysettings_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Save_Error);
    console.setError(true);
  }
}

void ConsoleListener::savePoses(Console& console, const string& filename)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);

  BodySettings poses = mesh->getPoses ();

#if defined(__APPLE__) && defined(__MACH__)
/* This piece of code has been tested on Mac OS X only!
 * On OS X the poses (and all user specific settings as well) will be
 * are supposed to be saved to the Users Document folder in a directory named
 * makehuman.
 *
 * Since this directory does not exists the first time so we'll need to create
 * one.
 */

  // First create a directory at the given location if it does not exists

  string path(FileTools::getFilePath(filename));
  if (!FileTools::fileExists(path))
  {
    bool rc = FileTools::makeDirHier(path);
    assert(rc == true);
  }
#endif // MAC OS X specific code

  bool state = poses.save (filename);

  if (state)
  {
    console.printMessage(kConsoleMessage_Save_Poses_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Save_Error);
    console.setError(true);
  }
}

void ConsoleListener::saveAutozoom(Console& console, const string& filename)
{
	Global &global = Global::instance ();
	Autozoom *autozoom = global.getAutozoom ();
	Camera *camera = global.getCamera ();
	assert (autozoom);

  bool state = autozoom->save (filename,*camera);

  if (state)
  {
	  console.printMessage(kConsoleMessage_Save_Autozoom_Success);
  }
  else
  {
	  console.printMessage(kConsoleMessage_Save_Error);
  }
}

void ConsoleListener::exportBodySettings(Console& console, string& filename, bool full)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);

  ObjExporter obj_export (*mesh);

  if( filename.substr( filename.size () - 1, 1) != PATH_SEPARATOR)
  {
    filename.append(PATH_SEPARATOR);
  }

#if defined(_WIN32)
  createDirWhenNotExists(filename);
#else
/* This piece of code has been tested on Mac OS X only!
 * On OS X the body settings (and all user specific settings as well) will be
 * are supposed to be saved to the Users Document folder in a directory named
 * makehuman.
 *
 * Since this directory does not exists the first time so we'll need to create
 * one.
 */

  // First create a directory at the given location if it does not exists

  string path(FileTools::getFilePath(filename));
  if (!FileTools::fileExists(path))
  {
    bool rc = FileTools::makeDirHier(path);
    assert(rc == true);
  }
#endif // MAC OS X specific code

  bool state = obj_export.exportFile (filename, full);

  if (state)
  {
    console.printMessage(kConsoleMessage_Export_Object_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Save_Error);
    console.setError(true);
  }
}

void ConsoleListener::exportCollada(Console& console, string& filename)
{
  Global &global = Global::instance ();
  Mesh *mesh = global.getMesh ();
  assert (mesh);

  ColladaExporter collada_export (*mesh);

  if( filename.substr( filename.size () - 1, 1) != PATH_SEPARATOR)
  {
    filename.append(PATH_SEPARATOR);
  }

#if defined(_WIN32)
  createDirWhenNotExists(filename);
#else
/* This piece of code has been tested on Mac OS X only!
 * On OS X the body settings (and all user specific settings as well) will be
 * are supposed to be saved to the Users Document folder in a directory named
 * makehuman.
 *
 * Since this directory does not exists the first time so we'll need to create
 * one.
 */

  // First create a directory at the given location if it does not exists

  string path(FileTools::getFilePath(filename));
  if (!FileTools::fileExists(path))
  {
    bool rc = FileTools::makeDirHier(path);
    assert(rc == true);
  }
#endif // MAC OS X specific code

  bool expMode = true;

  if(global.getExpMode() == WITHOUT_CONTROLLER)
    expMode = false;

  bool state = collada_export.exportFile (filename,expMode);

  if (state)
  {
    console.printMessage(kConsoleMessage_Export_Collada_Success);
  }
  else
  {
    console.printMessage(kConsoleMessage_Save_Error);
    console.setError(true);
  }

}