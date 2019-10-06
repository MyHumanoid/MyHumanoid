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
 *  File   : Camera.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "GLUTWrapper.h"
#include <animorph/FileWriter.h>
#include <animorph/Matrix.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <animorph/VertexVector.h>
#include <animorph/util.h>
#include <cmath>
#include <map>

using Animorph::Matrix;
using Animorph::VertexVector;

namespace mhgui
{

typedef std::vector<int> Reference_Verts;

struct AutozoomData {
	glm::vec3       pos_camera;
	glm::vec2       xyRot_camera;
	Reference_Verts vertsIndexes;
	glm::vec3       vertsCenteroid;
};

// TODO: moving world vs. moving camera problem
/*! \brief Stores camera data.
 */
class Camera
{
private:
	glm::vec2 last_mouse_pos;
	glm::vec3 last_pos_camera;
	int       width;
	int       height;
	float     angle;
	glm::vec3 axis;

	Matrix cam_pos;
	Matrix cam_center;

	bool mode;

	// Ugly Test code following - just for test purposes!
	glm::vec3 mCameraPos;
	float     mAngleX;
	float     mAngleY;
	float     mAngleZ;
	glm::vec3 startVector;
	glm::vec3 endVector;
	float     startAngleX;
	float     endAngleX;
	float     startAngleY;
	float     endAngleY;
	float     timeForMorph;
	int       step;

public:
	/// construct a Camera that manages the world Matrix
	Camera();
	Camera(const Camera & inRHS);
	Camera & operator=(const Camera & inRHS);

	void reshape(int width, int height);
	void rotate(float theta, Animorph::RotateAxis axis);
	void mouseRotateStart(int x, int y);
	void rotateMouse(int x, int y);
	void moveMouse(int x, int y);
	void move(float x, float y, float z);
	void resetRotation();
	void resetPosition();

	void applyMatrix();

	const glm::vec3 & getPosition() const { return mCameraPos; }
	float             getAngleX() const { return mAngleX; }
	float             getAngleY() const { return mAngleY; }
	float             getAngleZ() const { return mAngleZ; }
	bool              isPerspective() const { return mode; }
	void              setPerspective(bool m);

	int   steps();
	float getYForX(float x);
	void  calcForStepAnimate(float inX);
	bool  timerTrigger();

	void moveCameraAnimated(const std::string & filename, AutozoomData data,
	                        const VertexVector & vertexvector);
};

class Autozoom : public std::map<std::string, AutozoomData>
{
private:
	void fromStream(Animorph::FileReader & in_stream, const std::string & filename);
	void createStream(std::ostringstream & out_stream, const std::string & filename,
	                  const Camera & camera);

public:
	bool         lazyLoadData(const std::string & filename);
	bool         save(const std::string & filename, const Camera & camera);
	AutozoomData getAutozoomData(const std::string & filename);
};

} // namespace mhgui
