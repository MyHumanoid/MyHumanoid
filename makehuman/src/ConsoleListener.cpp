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

#include "ConsoleListener.h"
#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "ConsoleCommands.h"
#include "FileTools.h"
#include "Global.h"
#include "TargetPanel.h"
#include "util.h"
#include <animorph/BodySettings.h>
#include <animorph/ColladaExporter.h>
#include <animorph/ObjExporter.h>
#include <assert.h>
#include <gui/CGUtilities.h>
#include <gui/Component.h>
#include <gui/Console.h>
#include <gui/GLUTWrapper.h>
#include <gui/Window.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace mhgui;

ConsoleListener::ConsoleListener()
    : AbstractListener()
{
}

ConsoleListener::~ConsoleListener() {}

bool ConsoleListener::mouseOver(const Point &inMousePos, Component *source)
{
	return true;
}

bool ConsoleListener::mouseOut(const Point &inMousePos, Component *source)
{
	return true;
}

bool ConsoleListener::mouseDragged(const Point &inMousePos, Component *source)
{
	return true;
}

bool ConsoleListener::mousePressed(const Point &inMousePos, int button,
                                   Component *source)
{
	return true;
}

bool ConsoleListener::mouseWheel(const Point &inMousePos, int inButton,
                                 Component *source)
{
	return false;
}

bool ConsoleListener::mouseReleased(const Point &inMousePos, int button,
                                    Component *source)
{
	return true;
}

bool ConsoleListener::keyType(unsigned char key, Component *source)
{
	Console *console = dynamic_cast<Console *>(source); // req. RTTI!
	assert(console); // Check if this is really a Console object?

	if (console->acceptUserInput()) {
		switch (toupper(key)) {
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
			// std::cout << console->getSize().getWidth() << std::endl;
		}
	} else {
		if (toupper(key) == 27) {
			if (!Global::instance().isRendering()) {
				console->close();
			}
		} else if (toupper(key) == '\r' && console->isInError()) {
			console->retryCommand();
		}
	}
	return true;
}

void ConsoleListener::parseCommand(Console &console)
{
	int i;
	string cmd;
	string arg;
	const string &line = console.getUserText();

	Global &global = Global::instance();

	if (console.getCommand() == kConsoleCommand_Exit) {
		ExportConfigurationXML();
		exit(0);
	}

	if (!line.empty()) {
		if (console.getStatus() == Console::PROMPT) {
			i = line.find_first_of(' ');
			if (i == -1) {
				cmd = line;
				arg = "";
			} else {
				cmd = line.substr(0, i);
				arg = line.substr(i + 1, line.length() - i);
			}
			console.setCommand(cmd);
		} else if (console.getStatus() == Console::INPUT) {
			cmd = console.getCommand();
			arg = line;
		}

		if (cmd == kConsoleCommand_Load_AqsisPath) {
			if (arg.size() == 0) {
				console.inputMode(kConsoleCommand_Load_AqsisPath,
				                  GetDefaultAqsisPath());
			} else {
				loadAqsisPath(console, arg);
			}
		} else if (cmd == kConsoleCommand_Load_PixiePath) {
			if (arg.size() == 0) {
				console.inputMode(kConsoleCommand_Load_PixiePath,
				                  GetDefaultPixiePath());
			} else {
				loadPixiePath(console, arg);
			}
		} else if (cmd == kConsoleCommand_Save_Autozoom) {
			if (arg.size() == 0) {
				Window &mainWindow(*g_mainWindow);
				TargetPanel *targetPanel = dynamic_cast<TargetPanel *>(
				    mainWindow.getPanel(kComponentID_TargetPanel));
				string target = targetPanel->getCategory();
				std::string pathAutozoom_data =
				    searchDataDir("targets") + "/" + target + "/" + target + ".camera";
				console.inputMode(kConsoleMessage_Save_Autozoom, pathAutozoom_data);
			} else {
				saveAutozoom(console, arg);
			}
		} else if (cmd == kConsoleCommand_Set_Parameter) {
			if (arg.size() == 0) {

			} else {
				parseSetParameter(console, arg);
			}
		} else if (cmd == kConsoleCommand_Create_WeightsMatrix) {

			if (arg.size() == 0) {
				CreateWeightsFile();

			} else {
			}
		}
		else {
			commandNotFound(console);
		}
	}
}

void ConsoleListener::commandNotFound(Console &console)
{
	console.setMessage(kConsoleMessage_Unknown_Command);
	console.setStatus(Console::MESSAGE);
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

void ConsoleListener::loadAqsisPath(Console &console, const string &path)
{
	int ret;
	if ((ret = SetAqsisBasePath(path + "/")) >= 0) {
		saveRenderingPaths(path + "/", AQSIS);
		console.printMessage(kConsoleMessage_OK);

		ExportConfigurationXML();
	} else {
		switch (ret) {
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

void ConsoleListener::loadPixiePath(Console &console, const string &path)
{
	int ret;
	if ((ret = SetPixieBasePath(path + "/")) >= 0) {
		saveRenderingPaths(path + "/", PIXIE);
		console.printMessage(kConsoleMessage_OK);

		ExportConfigurationXML();
	} else {
		switch (ret) {
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

void ConsoleListener::parseSetParameter(Console &console, const string &path)
{
	int ret;

	if ((ret = ParseParameter(path)) < 0) {
		switch (ret) {
		case -3:
			console.printMessage(kConsoleMessage_InvalidPath);
			break;
		default:
			console.printMessage(kConsoleMessage_Parameter_NG);
			break;
		}

	} else {
		ExportConfigurationXML();
		console.printMessage(kConsoleMessage_Parameter_OK);
	}
}





void ConsoleListener::saveAutozoom(Console &console, const string &filename)
{
	Global &global = Global::instance();
	Autozoom *autozoom = global.getAutozoom();
	Camera *camera = global.getCamera();
	assert(autozoom);

	bool state = autozoom->save(filename, *camera);

	if (state) {
		console.printMessage(kConsoleMessage_Save_Autozoom_Success);
	} else {
		console.printMessage(kConsoleMessage_Save_Error);
	}
}
