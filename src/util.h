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
#pragma once

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef vector<string> StringVector;

const StringVector getPixmapsAlternatives(const string & pixmap);
const StringVector getDataAlternatives(const string & data);

const std::string searchDataFile(const string & data_file);
const std::string searchPixmapFile(const string & pixmap_file);

const string searchDataDir(const string & data_dir);
const string searchPixmapDir(const string & pixmap_dir);

const string searchFile(const StringVector & name_vector);
const string searchDir(const StringVector & name_vector);

const string getHomeDir();

void loadRenderingPaths();
void saveRenderingPaths(string path, int RENDER_TYPE);

void CreateWeightsFile();

int ParseParameter(string cmd);

void loadDefaultBodySettings();
bool loadSelectorsPositions(const std::string & filename);
bool loadSelectorsPositions(const std::vector<string> & strings, const float value = 1.0);
bool saveSelectorsPositions(const std::string &     filename);

template <class T> struct deleteFunctor {
	void operator()(T & t) { delete t; }
};
