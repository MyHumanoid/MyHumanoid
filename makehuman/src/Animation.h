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
 *  File   : Animation.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef ANIMATION_H
#define ANIMATION_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <animorph/Mesh.h>
#include <animorph/RawMesh.h>
#include <vector>

#define kTimerCallback 100

using namespace Animorph;

using std::vector;

class Animation
{
public:
	Animation();

private:
	unsigned int interpNumber;
	unsigned int frameRate;

	vector<RawMesh> frames;
	vector<RawMesh>::iterator frames_it;

	bool loaded;
	bool started;

	void addFrame(BodySettings &bs, const string &frame_name, Mesh *mesh);
	void interpolate(BodySettings &prev, BodySettings &next);

public:
	void setInterpNumber(const unsigned int inNumber) { interpNumber = inNumber; }
	unsigned int getInterpNumber() { return interpNumber; }

	unsigned int getFrameRate() const { return frameRate; }
	void setFrameRate(const unsigned int inRate) { frameRate = inRate; }

	bool load(const string &path);

	void nextFrame();

	bool isLoaded() const { return loaded; }
	void setLoaded(bool inLoaded) { loaded = inLoaded; }

	bool isStarted() const { return started; }
	void setStarted(bool inStarted) { started = inStarted; }

	RawMesh &getMesh() { return *frames_it; }

	void reset();
};

#endif // ANIMATION_H
