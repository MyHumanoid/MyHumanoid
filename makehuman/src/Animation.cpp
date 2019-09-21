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
 *  File   : Animation.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "Animation.h"
#include "Global.h"
#include <animorph/BodySettings.h>
#include <animorph/DirectoryList.h>

using namespace std;
using namespace Animorph;

Animation::Animation()
    : interpNumber(0)
    , frameRate(50)
    , frames_it(NULL)
    , loaded(false)
    , started(false)
{
}

bool Animation::load(const string &path)
{
	int i = 0;
	DirectoryList dir_list;
	BodySettings poses;
	BodySettings prevPoses;

	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);

	frames.clear();

	dir_list.setRootPath(path);
	dir_list.setRecursive(0);
	dir_list.setFileFilter(".bs");

	const StringList &str_list(dir_list.getDirectoryList());

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);

		bool state = poses.load(file);

		if (state) {
			if (i > 0 && interpNumber > 0) {
				interpolate(prevPoses, poses);
			}

			addFrame(poses, file, mesh);
			prevPoses = poses;
			i++;
		}

		frames_it = frames.begin();
		if (frames_it != frames.end()) {
			loaded = true;
		}
	}

	return true;
}

void Animation::interpolate(BodySettings &prev, BodySettings &next)
{
	BodySettings interPoses[interpNumber];
	unsigned int i;
	float next_value, step;
	BodySettings::const_iterator next_it;
	BodySettings::const_iterator prev_it;

	for (i = 0; i < interpNumber; i++) {
		interPoses[i] = prev;
	}

	for (prev_it = prev.begin(); prev_it != prev.end(); prev_it++) {
		string target_name = (*prev_it).first;

		next_it = next.find(target_name);
		if (next_it == next.end()) {
			next_value = 0.0f;
		} else {
			next_value = (*next_it).second;
		}

		float morph_value = (*prev_it).second;

		if (next_value != morph_value) {
			step = (next_value - morph_value) / (interpNumber + 1);
			for (i = 0; i < interpNumber; i++) {
				interPoses[i][target_name] = morph_value + (step * (i + 1));
			}
		}
	}

	for (next_it = next.begin(); next_it != next.end(); next_it++) {
		string target_name = (*next_it).first;

		prev_it = prev.find(target_name);
		if (prev_it == prev.end()) {
			float morph_value = (*next_it).second;
			if (morph_value != 0) {
				next_value = morph_value;
				morph_value = 0.0f;

				step = (next_value - morph_value) / (interpNumber + 1);
				for (i = 0; i < interpNumber; i++) {
					interPoses[i][target_name] = morph_value + (step * (i + 1));
				}
			}
		}
	}

	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);

	for (i = 0; i < interpNumber; i++) {
		addFrame(interPoses[i], "interpolation", mesh);
	}
}

void Animation::addFrame(BodySettings &bs, const string &frame_name, Mesh *mesh)
{
	mesh->doPose(bs, true);

	if (Global::instance().getSubdivision()) {
		mesh->calcSubsurf();
	}

	const FaceVector &facevector(mesh->getFaceVectorRef());
	const VertexVector &vertexvector(mesh->getVertexVectorRef());

	RawMesh rawmesh;
	rawmesh.setName(frame_name);
	rawmesh.setFaceVector(facevector);

	rawmesh.setVertexVector(vertexvector);

	FaceGroup &clothesgroup(mesh->getClothesGroupRef());
	// clothesgroup.calcVertexes(facevector);
	rawmesh.setClothesGroup(clothesgroup);
	frames.push_back(rawmesh);
}

void Animation::nextFrame()
{
	if (loaded) {
		if (++frames_it == frames.end()) {
			frames_it = frames.begin();
		}
	}
}

void Animation::reset() { frames_it = frames.begin(); }
