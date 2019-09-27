#include "animorph/Matrix.h"

using namespace Animorph;
using namespace std;

void Matrix::fromEuler(Euler &e)
{
	float srx = sinf(e.x);
	float sry = sinf(e.y);
	float srz = sinf(e.z);
	float crx = cosf(e.x);
	float cry = cosf(e.y);
	float crz = cosf(e.z);

	switch (e.getNotation()) {
	case Euler::XYZ:
		data[0] = cry * crz;
		data[4] = cry * (-srz);
		data[8] = sry;
		data[1] = crx * srz + srx * sry * crz;
		data[5] = crx * crz - srx * sry * srz;
		data[9] = -srx * cry;
		data[2] = srx * srz - crx * sry * crz;
		data[6] = srx * crz + crx * sry * srz;
		data[10] = crx * cry;
		break;
	}
}

void Matrix::fromQuaternion(const Quaternion &q)
{
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float wz = q.w * q.z;
	float xz = q.x * q.z;
	float wx = q.w * q.x;
	float yz = q.y * q.z;
	float wy = q.w * q.y;

	data[0] = 1 - 2 * yy - 2 * zz;
	data[1] = 2 * xy - 2 * wz;
	data[2] = 2 * xz + 2 * wy;

	data[4] = 2 * xy + 2 * wz;
	data[5] = 1 - 2 * xx - 2 * zz;
	data[6] = 2 * yz - 2 * wx;

	data[8] = 2 * xz - 2 * wy;
	data[9] = 2 * yz + 2 * wx;
	data[10] = 1 - 2 * xx - 2 * yy;
}

void Matrix::setRotation(float theta, RotateAxis axis)
{
	float s = sinf(theta);
	float c = cosf(theta);

	switch (axis) {
	case X_AXIS: // rotate about x axis
		data[0] = 1.0;
		data[4] = 0.0;
		data[8] = 0.0;
		data[12] = 0.0;
		data[1] = 0.0;
		data[5] = c;
		data[9] = -s;
		data[13] = 0.0;
		data[2] = 0.0;
		data[6] = s;
		data[10] = c;
		data[14] = 0.0;
		data[3] = 0.0;
		data[7] = 0.0;
		data[11] = 0.0;
		data[15] = 1.0;
		break;

	case Y_AXIS: // rotate about y axis
		data[0] = c;
		data[4] = 0.0;
		data[8] = s;
		data[12] = 0.0;
		data[1] = 0.0;
		data[5] = 1.0;
		data[9] = 0.0;
		data[13] = 0.0;
		data[2] = -s;
		data[6] = 0.0;
		data[10] = c;
		data[14] = 0.0;
		data[3] = 0.0;
		data[7] = 0.0;
		data[11] = 0.0;
		data[15] = 1.0;
		break;

	case Z_AXIS: // rotate about z axis
		data[0] = c;
		data[4] = -s;
		data[8] = 0.0;
		data[12] = 0.0;
		data[1] = s;
		data[5] = c;
		data[9] = 0.0;
		data[13] = 0.0;
		data[2] = 0.0;
		data[6] = 0.0;
		data[10] = 1.0;
		data[14] = 0.0;
		data[3] = 0.0;
		data[7] = 0.0;
		data[11] = 0.0;
		data[15] = 1.0;
		break;
	}
}

void Matrix::setRotation(float theta, const glm::vec3 &axis)
{
	// implement a unit vector check?

	float c = cosf(theta);
	float s = sinf(theta);

	float a = 1.0 - c;
	float ax = a * axis.x;
	float ay = a * axis.y;
	float az = a * axis.z;

	data[0] = ax * axis.x + c;
	data[1] = ax * axis.y + axis.z * s;
	data[2] = ax * axis.z - axis.y * s;

	data[4] = ay * axis.x - axis.z * s;
	data[5] = ay * axis.y + c;
	data[6] = ay * axis.z + axis.x * s;

	data[8] = az * axis.x + axis.y * s;
	data[9] = az * axis.y - axis.x * s;
	data[10] = az * axis.z + c;

	// reset the translation portion
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
}

void Matrix::setScale(const glm::vec3 &s) { setScale(s.x, s.y, s.z); }

void Matrix::setScale(float x, float y, float z)
{
	data[0] = x;
	data[4] = 0.0;
	data[8] = 0.0;
	data[12] = 0.0;
	data[1] = 0.0;
	data[5] = y;
	data[9] = 0.0;
	data[13] = 0.0;
	data[2] = 0.0;
	data[6] = 0.0;
	data[10] = z;
	data[14] = 0.0;
	data[3] = 0.0;
	data[7] = 0.0;
	data[11] = 0.0;
	data[15] = 1.0;
}

void Matrix::setTranslation(const glm::vec3 &t)
{
	setTranslation(t.x, t.y, t.z);
}

void Matrix::setTranslation(float x, float y, float z)
{
	data[0] = 1.0;
	data[4] = 0.0;
	data[8] = 0.0;
	data[12] = x;
	data[1] = 0.0;
	data[5] = 1.0;
	data[9] = 0.0;
	data[13] = y;
	data[2] = 0.0;
	data[6] = 0.0;
	data[10] = 1.0;
	data[14] = z;
	data[3] = 0.0;
	data[7] = 0.0;
	data[11] = 0.0;
	data[15] = 1.0;
}

glm::vec3 Matrix::getTranslation()
{
	return glm::vec3(data[12], data[13], data[14]);
}

/**************************************/
/* Non-Member operators and functions */
/**************************************/

glm::vec3 Animorph::operator*(const glm::vec3 &v, const Matrix &m) // REDO
{
	// data[3], data[7], data[11], and data[15] are everytime [0 0 0 1] in OpenGL.
	// if speed is critical, here's some place for optimization...

	return glm::vec3(
	    v.x * m.data[0] + v.y * m.data[4] + v.z * m.data[8] + m.data[12],
	    v.x * m.data[1] + v.y * m.data[5] + v.z * m.data[9] + m.data[13],
	    v.x * m.data[2] + v.y * m.data[6] + v.z * m.data[10] + m.data[14]);
}

Matrix Animorph::operator*(const Matrix &a, const Matrix &b)
{
	Matrix r;

	// data[3], data[7], data[11], and data[15] are everytime [0 0 0 1] in OpenGL.
	// if speed is critical, here's some place for optimization...

	r.data[0] = a.data[0] * b.data[0] + a.data[4] * b.data[1] +
	            a.data[8] * b.data[2] + a.data[12] * b.data[3];
	r.data[4] = a.data[0] * b.data[4] + a.data[4] * b.data[5] +
	            a.data[8] * b.data[6] + a.data[12] * b.data[7];
	r.data[8] = a.data[0] * b.data[8] + a.data[4] * b.data[9] +
	            a.data[8] * b.data[10] + a.data[12] * b.data[11];
	r.data[12] = a.data[0] * b.data[12] + a.data[4] * b.data[13] +
	             a.data[8] * b.data[14] + a.data[12] * b.data[15];
	r.data[1] = a.data[1] * b.data[0] + a.data[5] * b.data[1] +
	            a.data[9] * b.data[2] + a.data[13] * b.data[3];
	r.data[5] = a.data[1] * b.data[4] + a.data[5] * b.data[5] +
	            a.data[9] * b.data[6] + a.data[13] * b.data[7];
	r.data[9] = a.data[1] * b.data[8] + a.data[5] * b.data[9] +
	            a.data[9] * b.data[10] + a.data[13] * b.data[11];
	r.data[13] = a.data[1] * b.data[12] + a.data[5] * b.data[13] +
	             a.data[9] * b.data[14] + a.data[13] * b.data[15];
	r.data[2] = a.data[2] * b.data[0] + a.data[6] * b.data[1] +
	            a.data[10] * b.data[2] + a.data[14] * b.data[3];
	r.data[6] = a.data[2] * b.data[4] + a.data[6] * b.data[5] +
	            a.data[10] * b.data[6] + a.data[14] * b.data[7];
	r.data[10] = a.data[2] * b.data[8] + a.data[6] * b.data[9] +
	             a.data[10] * b.data[10] + a.data[14] * b.data[11];
	r.data[14] = a.data[2] * b.data[12] + a.data[6] * b.data[13] +
	             a.data[10] * b.data[14] + a.data[14] * b.data[15];

	// for OpenGL matrices only we could skip the part below, because it's
	// everytime [0 0 0 1] but for completness and general purpose Matrix class we
	// calulate it if speed is critical, here's some place for optimization...
	r.data[3] = a.data[3] * b.data[0] + a.data[7] * b.data[1] +
	            a.data[11] * b.data[2] + a.data[15] * b.data[3];
	r.data[7] = a.data[3] * b.data[4] + a.data[7] * b.data[5] +
	            a.data[11] * b.data[6] + a.data[15] * b.data[7];
	r.data[11] = a.data[3] * b.data[8] + a.data[7] * b.data[9] +
	             a.data[11] * b.data[10] + a.data[15] * b.data[11];
	r.data[15] = a.data[3] * b.data[12] + a.data[7] * b.data[13] +
	             a.data[11] * b.data[14] + a.data[15] * b.data[15];

	return r;
}

/// << operator for matrix output
std::ostream &Animorph::operator<<(std::ostream &s, const Matrix &m)
{
	for (int i = 0; i < 4; i++) {
		s << "[";
		for (int n = 0; n < 4; n++) {
			s << m.data[(n * 4) + i] << " ";
		}
		s << "]" << std::endl;
	}

	return s;
}
