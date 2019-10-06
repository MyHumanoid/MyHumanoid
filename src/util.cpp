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
 *  File   : util.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#include "util.h"

#include "ComponentID.h"
#include "Global.h"
#include <animorph/DirectoryList.h>
#include <animorph/util.h>
#include <gui/CGUtilities.h>

#include <gui/Window.h>
#include <iostream>
#include <string>
#include <vector>

#include "CompositeMorph.h"
#include "log/log.h"
#include "Vfs.h"

using std::string;

const string searchDataFile(const string & data_file)
{
	string s(searchFile(getDataAlternatives(data_file)));
	if(s.empty()) {
		fprintf(stderr,
		        "*** WARNING in searchDataFile() : attempt to load file '%s'"
		        "' but this could not found in data/!\n",
		        data_file.c_str());
	}
	return s;
}

const string searchPixmapFile(const string & pixmap_file)
{
	string s(searchFile(getPixmapsAlternatives(pixmap_file)));
	if(s.empty()) {
		fprintf(stderr,
		        "*** WARNING in searchPixmapFile() : attempt to load file '%s'"
		        "' but this could not found in pixmaps/!\n",
		        pixmap_file.c_str());
	}
	return s;
}

const string searchDataDir(const string & data_dir)
{
	return searchDir(getDataAlternatives(data_dir));
}

const string searchPixmapDir(const string & pixmap_dir)
{
	return searchDir(getPixmapsAlternatives(pixmap_dir));
}

const StringVector getPixmapsAlternatives(const string & pixmap)
{
	StringVector name_vector;
	name_vector.push_back("data/pixmaps/" + pixmap);
	return name_vector;
}

const StringVector getDataAlternatives(const string & data)
{
	StringVector name_vector;
	name_vector.push_back("data/data/" + data);
	return name_vector;
}

const string searchFile(const StringVector & name_vector)
{
	for(unsigned int i = 0; i < name_vector.size(); i++) {
		const string & try_name = name_vector[i];
		
		auto res = vfs::getType(try_name);
		if(res == vfs::FileType::Regular) {
			return try_name;
		}
	}

	return string();
}

const string searchDir(const StringVector & name_vector)
{
	for(unsigned int i = 0; i < name_vector.size(); i++) {
		const string & try_name = name_vector[i];
		
		auto res = vfs::getType(try_name);
		if(res == vfs::FileType::Directory) {
			return try_name;
		}
	}
	return string();
}

bool GetSymmVertexConfig(int * symm_vertex)
{
	Animorph::FileReader file_reader;
	file_reader.open("data/base.sym");

	if(!file_reader)
		return false;

	char buffer[MAX_LINE_BUFFER];
	int  sx, dx;

	while(file_reader.getline(buffer, MAX_LINE_BUFFER)) {

		if(sscanf(buffer, "%d,%d", &sx, &dx) == 2) {
			symm_vertex[dx] = sx;
		}
	}
	file_reader.close();
	return true;
}
int getSymmJoint(int joint)
{
	return g_mesh.getSymmetricJoint((Animorph::SKELETON_JOINT)joint);
}

void CreateWeightsFile()
{
	return;

	Animorph::FileReader file_reader;
	Animorph::FileWriter file_write;

	file_write.open("data/vertex_complete.weights");

	char buffer[MAX_LINE_BUFFER * 4];
	int  weight[Animorph::SK_JOINT_END];
	int  actual_weight[Animorph::SK_JOINT_END];

	const VertexVector & vertexvector(g_mesh.getVertexVectorMorphOnlyRef());
	unsigned int         size_vx = vertexvector.size();

	int *                symm_vertex = new int[size_vx];
	std::ostringstream * out_stream  = (std::ostringstream *)new std::ostringstream[size_vx];

	int index;

	file_reader.open("data/vertex_sx.weights");

	if(!file_reader)
		return;

	for(unsigned int v = 0; v < size_vx; v++) {
		symm_vertex[v] = -1;
	}

	if(GetSymmVertexConfig(&symm_vertex[0]) == false)
		return;

	for(unsigned int i = 0; i < size_vx; i++) {
		memset(buffer, 0, MAX_LINE_BUFFER * 4);
		out_stream[i].str("");

		file_reader.getline(buffer, MAX_LINE_BUFFER * 4);

		out_stream[i] << buffer << endl;
	}

	for(unsigned int j = 0; j < size_vx; j++) {

		if(symm_vertex[j] == -1) { // sx vertex
			file_write << out_stream[j].str().c_str();
		} else {

			int symm = symm_vertex[j];

			memset(weight, 0, sizeof(int) * Animorph::SK_JOINT_END);
			memset(actual_weight, 0, sizeof(int) * Animorph::SK_JOINT_END);

			sscanf((char *)out_stream[symm].str().c_str(),
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d",
			       &index, &weight[0], &index, &weight[1], &index, &weight[2], &index,
			       &weight[3], &index, &weight[4], &index, &weight[5], &index,
			       &weight[6], &index, &weight[7], &index, &weight[8], &index,
			       &weight[9], &index, &weight[10], &index, &weight[11], &index,
			       &weight[12], &index, &weight[13], &index, &weight[14], &index,
			       &weight[15], &index, &weight[16], &index, &weight[17], &index,
			       &weight[18], &index, &weight[19], &index, &weight[20], &index,
			       &weight[21], &index, &weight[22], &index, &weight[23], &index,
			       &weight[24], &index, &weight[25], &index, &weight[26], &index,
			       &weight[27], &index, &weight[28], &index, &weight[29], &index,
			       &weight[30], &index, &weight[31], &index, &weight[32], &index,
			       &weight[33], &index, &weight[34], &index, &weight[35], &index,
			       &weight[36], &index, &weight[37], &index, &weight[38], &index,
			       &weight[39], &index, &weight[40], &index, &weight[41], &index,
			       &weight[42], &index, &weight[43], &index, &weight[44], &index,
			       &weight[45], &index, &weight[46], &index, &weight[47], &index,
			       &weight[48], &index, &weight[49], &index, &weight[50], &index,
			       &weight[51], &index, &weight[52], &index, &weight[53], &index,
			       &weight[54], &index, &weight[55], &index, &weight[56], &index,
			       &weight[57], &index, &weight[58], &index, &weight[59], &index,
			       &weight[60], &index, &weight[61], &index, &weight[62], &index,
			       &weight[63], &index, &weight[64], &index, &weight[65], &index,
			       &weight[66], &index, &weight[67]);

			for(int joint = 0; joint < Animorph::SK_JOINT_END; joint++) {
				int symm_joint       = getSymmJoint(joint);
				actual_weight[joint] = weight[symm_joint];
				file_write << joint << " ";
				file_write << actual_weight[joint] << " ";
			}
			file_write << endl;
		}
	}
	file_write.close();
	file_reader.close();
}

void loadDefaultBodySettings()
{
	Animorph::BodySettings bodyset;
	bool         state = bodyset.load(searchDataFile("default.bs"));

	if(state) {
		g_global.clearFuzzy();
		
		//g_global.m_comp = bodyset.m_comp;
		g_grids.fromSavedPositions(bodyset.m_comp);
		
		//mhgui::g_mainWindow->characterSettingPanel->updateUi();
	}

	if(state) {
		g_mesh.doMorph(bodyset);
		g_mesh.calcNormals();
	}
}
