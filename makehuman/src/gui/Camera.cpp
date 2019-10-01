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
 *  File   : Camera.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Camera.h"
#include "gui/CGUtilities.h"
#include <iostream>

#include "log/log.h"

#define MAX_LINE 3500

using namespace Animorph;
using namespace std;

namespace mhgui
{

Camera::Camera()
        : last_mouse_pos(0.0f, 0.0f)
        , last_pos_camera(0.0f, 0.0f, 0.0f)
        , width(0)
        , height(0)
        , angle(0.0f)
        , axis(0.0f, 0.0f, 0.0f)
        , cam_pos()
        , cam_center()
        , mode(false)
        ,

        mCameraPos(0.0f, 0.0f, 0.0f)
        , mAngleX(0)
        , mAngleY(0)
        , mAngleZ(0)
        , startVector(0.0f, 0.0f, -30.0f)
        , endVector(0.0f, -7.0f, -3.0f)
        ,

        startAngleX((0.0 * M_PI) / 180.0f)
        , endAngleX((-90.0 * M_PI) / 180.0f)
        ,

        startAngleY((0.0 * M_PI) / 180.0f)
        , endAngleY((00.0 * M_PI) / 180.0f)
        ,

        timeForMorph(1.0)
        , // in Seconds
        step(-1)
{
}

Camera::Camera(const Camera & inRHS)
        : last_mouse_pos(inRHS.last_mouse_pos)
        , last_pos_camera(inRHS.last_pos_camera)
        , width(inRHS.width)
        , height(inRHS.height)
        , angle(inRHS.angle)
        , axis(inRHS.axis)
        , cam_pos(inRHS.cam_pos)
        , cam_center(inRHS.cam_center)
        , mode(inRHS.mode)
        ,

        // Ugly Test code following - just for test purposes!
        mCameraPos(inRHS.mCameraPos)
        , mAngleX(inRHS.mAngleX)
        , mAngleY(inRHS.mAngleY)
        , mAngleZ(inRHS.mAngleZ)
{
}

Camera & Camera::operator=(const Camera & inRHS)
{
	if(&inRHS != this) {
		last_mouse_pos  = inRHS.last_mouse_pos;
		last_pos_camera = inRHS.last_pos_camera;
		width           = inRHS.width;
		height          = inRHS.height;
		angle           = inRHS.angle;
		axis            = inRHS.axis;
		cam_pos         = inRHS.cam_pos;
		cam_center      = inRHS.cam_center;

		// Ugly Test code following - just for test purposes!
		mCameraPos = inRHS.mCameraPos;
		mAngleX    = inRHS.mAngleX;
		mAngleY    = inRHS.mAngleY;
		mAngleZ    = inRHS.mAngleZ;
	}
	return *this;
}

/*! Set internal width and height values. In Makehuman is called by Glut
 * callbacks when it is resized.*/
void Camera::reshape(int width, int height)
{
	this->width  = width;
	this->height = height;
}

/*! Modify orientation values. In Makehuman is called by Glut callbacks when a
 * key is pressed.*/
void Camera::rotate(float theta, Animorph::RotateAxis axis)
{
	Matrix rotate;

	if(axis == X_AXIS)
		mAngleX += theta;
	else if(axis == Y_AXIS)
		mAngleY += theta;
	else if(axis == Z_AXIS)
		mAngleZ += theta;

	rotate.setRotation(mAngleY, Y_AXIS);
	cam_pos = rotate;
	rotate.setRotation(mAngleZ, Z_AXIS);
	cam_pos = rotate * cam_pos;
	rotate.setRotation(mAngleX, X_AXIS);
	cam_pos = rotate * cam_pos;
}

/*! Restore the default rotation. */
void Camera::resetRotation()
{
	cam_pos.identity();
	mAngleX = mAngleY = mAngleZ = 0.0f;
}

/*! Restore the default rotation. */
void Camera::resetPosition()
{
	cam_center.identity();
	mCameraPos = glm::vec3(0);
}

/*! Modify orientation values. In Makehuman is called via Glut callbacks when
 mouse is dragged with a button down other than the right button..*/
void Camera::rotateMouse(int x, int y)
{
	rotate((M_PI / 180) * (y - last_mouse_pos.y), X_AXIS);
	rotate((M_PI / 180) * (x - last_mouse_pos.x), Y_AXIS);

	last_mouse_pos.x = x;
	last_mouse_pos.y = y;
}

/*! Update last mouse position. These values are used by rotateMouse() to keep
track of the movement. In Makehuman is called via Glut callbacks during a mouse
drag action with a button down other than the right button. */
void Camera::mouseRotateStart(int x, int y)
{
	last_mouse_pos.x = x;
	last_mouse_pos.y = y;
}

/*! Move the camera along the xy plane. In Makehuman is called via Glut
 callbacks when mouse is dragged with the right button down. */
void Camera::moveMouse(int x, int y)
{
	float moveRate = 4500;

	if(mCameraPos.z != 0) {
		moveRate /= abs(mCameraPos.z);
	}

	move((x - last_mouse_pos.x) / moveRate, -(y - last_mouse_pos.y) / moveRate, 0.0f);

	last_mouse_pos.x = x;
	last_mouse_pos.y = y;
}

/*! Move the camera. In Makehuman is called via Glut callbacks when mouse wheel
is used
(zoom in/out is a forward/backward movement of the camera), or a key is pressed.
*/
void Camera::move(float x, float y, float z)
{
	mCameraPos.x += x;
	mCameraPos.y += y;
	mCameraPos.z += z;

	Matrix m_tmp;

	m_tmp.setTranslation(x, y, z);

	cam_center = m_tmp * cam_center;
}

/*! Pass the current camera matrix to cgutils::displayStart(), which generates
OpenGL commands. */
void Camera::applyMatrix()
{
	Matrix m_tmp;

	m_tmp = cam_center * cam_pos;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(m_tmp.data);
}

/*! Set the camera mode. Ortho (false) or perspective (true). */
void Camera::setPerspective(bool m)
{
	mode = m;
}

float kTimePerRaster(0.03f);

int Camera::steps()
{
	return static_cast<int>(timeForMorph / kTimePerRaster);
}

float Camera::getYForX(float x)
{
	float v = 0.5 * (::cosf(x * M_PI) + 1.0f);
	return 1.0f - (v * v);
}

void Camera::calcForStepAnimate(float inX)
{
	cam_center.identity();
	mCameraPos = glm::vec3(0.f);
	cam_pos.identity();
	mAngleX = mAngleY = mAngleZ = 0.0f;

	float s = getYForX(inX);

	static glm::vec3 currentVector;
	static float     currentAngleX;
	static float     currentAngleY;

	currentAngleX = startAngleX + s * (endAngleX - startAngleX);
	currentAngleY = startAngleY + s * (endAngleY - startAngleY);

	currentVector = glm::vec3(startVector.x + s * (endVector.x - startVector.x),
	                          startVector.y + s * (endVector.y - startVector.y),
	                          startVector.z + s * (endVector.z - startVector.z));

	rotate(currentAngleX, X_AXIS);
	rotate(currentAngleY, Y_AXIS);

	move(currentVector.x, currentVector.y, currentVector.z);
}

bool Camera::timerTrigger()
{
	if(step < 0)
		return false;

	float currentTime = step * kTimePerRaster;

	calcForStepAnimate(currentTime / timeForMorph);
	glutPostRedisplay();

	++step;

	if(step > steps())
		step = -1;

	return true;
}

void Camera::moveCameraAnimated(const std::string & filename, AutozoomData data,
                                const VertexVector & vertexvector)
{
	glm::vec3 delta_camera(calcCenteroid(data.vertsIndexes, vertexvector));
	delta_camera -= data.vertsCenteroid;
	endAngleX = data.xyRot_camera.x;

	endAngleY   = data.xyRot_camera.y;
	endVector   = data.pos_camera - delta_camera;
	step        = 0;
	startVector = getPosition();
	startAngleX = getAngleX();
	startAngleY = getAngleY();
	timerTrigger();
}

/*! Load of camera data files. */
void Autozoom::fromStream(std::ifstream & in_stream, const std::string & filename)
{
	AutozoomData data;

	clear();

	float p0, p1, p2, r1, r2, c0, c1, c2;
	char  verts[MAX_LINE];
	char  buffer[MAX_LINE];

	in_stream.getline(buffer, MAX_LINE);

	/// TODO: lettura del file
	if(sscanf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%s", &p0, &p1, &p2, &r1, &r2, &c0, &c1, &c2,
	          verts) == 9) {
		glm::vec3 pos(p0, p1, p2);
		glm::vec2 xyRot(r1, r2);
		glm::vec3 cen(c0, c1, c2);
		data.pos_camera     = pos;
		data.xyRot_camera   = xyRot;
		data.vertsCenteroid = cen;
		stringTokeni(verts, ",", data.vertsIndexes);

		(*this)[filename] = data;
	} else {
		log_error("illegal data format: {}", buffer);
	}
}

bool Autozoom::lazyLoadData(const std::string & filename)
{
	std::map<std::string, AutozoomData>::const_iterator isInited;
	isInited = find(filename);

	if(isInited != end()) // already loaded?
		return true;

	// read the file

	FileReader file_reader;

	file_reader.open(filename);

	if(!file_reader)
		return false;

	fromStream(file_reader, filename);

	return true;
}

/*! Return the autozoom data for the body part. */
AutozoomData Autozoom::getAutozoomData(const std::string & filename)
{
	return (*this)[filename];
}

/*! Save the current camera position to the .camera file of the active target
 * panel. */
bool Autozoom::save(const std::string & filename, const Camera & camera)
{
	FileWriter file_writer;
	file_writer.open(filename);

	if(!file_writer)
		return false;

	std::ostringstream out_stream;
	createStream(out_stream, filename, camera);

	file_writer << out_stream.str();

	return true;
}

/*! Format the data coming from the current view and the vert list from the file
 * itself. A file must already exist */
void Autozoom::createStream(std::ostringstream & out_stream, const std::string & filename,
                            const Camera & camera)
{
	glm::vec3 camVector = camera.getPosition();
	float     camAngleX = camera.getAngleX();
	float     camAngleY = camera.getAngleY();
	out_stream << camVector.x << "," << camVector.y << "," << camVector.z << "," << camAngleX
	           << "," << camAngleY << "," << (*this)[filename].vertsCenteroid.x << ","
	           << (*this)[filename].vertsCenteroid.y << ","
	           << (*this)[filename].vertsCenteroid.z;
	std::vector<int>::iterator itVectorData;
	for(itVectorData = (*this)[filename].vertsIndexes.begin();
	    itVectorData != (*this)[filename].vertsIndexes.end(); itVectorData++) {
		out_stream << "," << *(itVectorData);
	}
}

} // namespace mhgui
