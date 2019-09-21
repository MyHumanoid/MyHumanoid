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
 *  File   : ConsoleCommands.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef CONSOLECOMMANDS_H
#define CONSOLECOMMANDS_H 1

static const string kConsoleCommand_Load_Background = "lbg";
static const string kConsoleCommand_Save_Autozoom = "saz";
static const string kConsoleCommand_Export_Collada = "eco";
static const string kConsoleCommand_Exit = "quit";
static const string kConsoleCommand_Load_AqsisPath = "aqs";
static const string kConsoleCommand_Load_PixiePath = "pix";
static const string kConsoleCommand_Set_Parameter = "set";
static const string kConsoleCommand_Create_WeightsMatrix = "wgt";

// console messages
static const string kConsoleMessage_Load_Background =
    "Load background image (*.png)";
static const string kConsoleMessage_Load_Background_Success =
    "Background image loaded";

static const string kConsoleMessage_Save_Autozoom =
    "Save Autozoom file (*.camera)";
static const string kConsoleMessage_Save_Autozoom_Success = "Autozoom saved";

static const string kConsoleMessage_Export_Collada =
    "Export Collada DAE file (insert a valid export path)";
static const string kConsoleMessage_Export_Collada_Success =
    "Collada DAE file exported. Press ESC to exit.";

static const string kConsoleMessage_Dir_Load_Error =
    "Error: couldn't read directory. Press ESC to exit or ENTER to retry!";
// static const string kConsoleMessage_Dir_Save_Error             = "Error:
// couldn't save files. Press ESC to exit or ENTER to retry!";
static const string kConsoleMessage_Load_Error =
    "Error: couldn't load file. Press ESC to exit or ENTER to retry!";
static const string kConsoleMessage_Save_Error =
    "Error: couldn't save file. Press ESC to exit or ENTER to retry!";
static const string kConsoleMessage_Unknown_Command = "Unknown command";
static const string kConsoleMessage_InvalidPath =
    "Error: the path doesn't exist.";
static const string kConsoleMessage_WrongPath =
    "Error: the path is wrong. Impossible locate rendering";

static const string kConsoleMessage_RenderEngineNotFound =
    "Unable to find a render engine. Press ESC to exit!";
static const string kConsoleMessage_TextureCompileError =
    "Error during texture compilation. Press ESC to exit!";
static const string kConsoleMessage_ShaderCompileError =
    "Error during shader compilation. Press ESC to exit!";
static const string kConsoleMessage_RenderError =
    "Rendering error. Press ESC to exit!";

static const string kConsoleMessage_WrongMode_Animations =
    "This command can be executed only in Utilities panel";
static const string kConsoleMessage_WrongMode_Poses =
    "This command can be executed only in Poses panel";
static const string kConsoleMessage_WrongMode_BodyDetails =
    "This command can be executed only in Body details, Character settings or "
    "Clothes panel";

static const string kConsoleMessage_NotLoaded_Animations =
    "Animations not loaded!";
static const string kConsoleMessage_OK = "O.K.! path stored, press Esc to Exit";
static const string kConsoleMessage_Parameter_OK =
    "O.K.! parameter stored, press Esc to Exit";
static const string kConsoleMessage_Parameter_NG =
    "Error! parameter not stored, press Esc to Exit";

#endif // CONSOLECOMMANDS_H
