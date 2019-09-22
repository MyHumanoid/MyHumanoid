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
			console->close();
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

		{
			commandNotFound(console);
		}
	}
}

void ConsoleListener::commandNotFound(Console &console)
{
	console.setMessage(kConsoleMessage_Unknown_Command);
	console.setStatus(Console::MESSAGE);
}
