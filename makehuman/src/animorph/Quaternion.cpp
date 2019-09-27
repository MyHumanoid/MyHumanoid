#include "animorph/Quaternion.h"

using namespace Animorph;

void Quaternion::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

void Quaternion::setRotation(float theta, RotateAxis axis)
{
	// theta = wrapPi (theta);

	switch (axis) {
	case X_AXIS: // rotate about x axis
		w = cos(theta * 0.5);
		x = sin(theta * 0.5);
		y = 0.0;
		z = 0.0;
		break;

	case Y_AXIS: // rotate about y axis
		w = cos(theta * 0.5);
		x = 0.0;
		y = sin(theta * 0.5);
		z = 0.0;
		break;

	case Z_AXIS: // rotate about z axis
		w = cos(theta * 0.5);
		x = 0.0;
		y = 0.0;
		z = sin(theta * 0.5);
		break;
	}
}

void Quaternion::fromEuler(Euler &e)
{
	float crx = cos(e.x * 0.5);
	float cry = cos(e.y * 0.5);
	float crz = cos(e.z * 0.5);
	float srx = sin(e.x * 0.5);
	float sry = sin(e.y * 0.5);
	float srz = sin(e.z * 0.5);

	switch (e.getNotation()) {
	case Euler::XYZ:
		w = crx * cry * crz + srx * sry * srz;
		x = srx * cry * crz - crx * sry * srz;
		y = crx * sry * crz + srx * cry * srz;
		z = crx * cry * srz - srx * sry * crz;
		break;
	}
}

Quaternion Animorph::operator*(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion q;

	q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	q.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
	q.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;

	return q;
}

/// Non-Member functions

std::ostream &Animorph::operator<<(std::ostream &s, const Quaternion &q)
{
	s << "[";
	s << "w: " << q.w << " x: " << q.x << " y: " << q.y << " z: " << q.z;
	s << "]";

	return s;
}
