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
 *  File   : util.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef MHUTIL_H
#define MHUTIL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <dirent.h>
#include <gui/Window.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#ifndef _WIN32
#include <pwd.h>
#endif

#ifdef _WIN32
//#define BUFFER 512
#include <io.h>
#include <shlobj.h>
#include <windows.h>

typedef HRESULT (*MYPROC)(HWND, int, HANDLE, DWORD, LPTSTR);
#else
#define stricmp strcasecmp
#endif

using std::string;
using std::vector;

using namespace mhgui;

typedef vector<string> StringVector;

const StringVector getPixmapsAlternatives(const string &pixmap);
const StringVector getDataAlternatives(const string &data);
const StringVector getBackgroundsAlternatives(const string &data);
const StringVector getTexturesAlternatives(const string &texture);
const StringVector getShadersAlternatives(const string &shader);

const string searchDataFile(const string &data_file);
const string searchPixmapFile(const string &pixmap_file);
const string searchBackgroundFile(const string &data_file);
const string searchTextureFile(const string &texture_file);
const string searchShaderFile(const string &shader_file);

const string searchDataDir(const string &data_dir);
const string searchPixmapDir(const string &pixmap_dir);

const string searchFile(const StringVector &name_vector);
const string searchDir(const StringVector &name_vector);

const string getHomeDir();

const string getUserWorkDir();
void createDirWhenNotExists(const string &inPath);

const string getMyObjPath();
const string getMyColladaPath();
const string getMyPosesPath();
const string getMyPosesBasePath();
const string getMyBodysettingsPath();
const string getMyBodysettingsBasePath();
const string getRenderingPath();
const string getTexturesPath();
const string getShadersPath();
const string getMyBackgroundsPath();

void createWorkingDirs();
void loadRenderingPaths();
void saveRenderingPaths(string path, int RENDER_TYPE);

void CreateWeightsFile();

int ParseParameter(string cmd);

void loadDefaultBodySettings();
bool loadSelectorsPositions(const std::string &filename);
bool loadSelectorsPositions(const std::vector<string> &strings,
                            const float value = 1.0);
bool saveSelectorsPositions(const std::string &filename,
                            std::ios_base::openmode mode = std::ios::app);

template <class T> struct deleteFunctor {
	void operator()(T &t) { delete t; }
};
#endif // MHUTIL_H
