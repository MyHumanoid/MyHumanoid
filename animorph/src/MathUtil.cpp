#include "../include/animorph/MathUtil.h"

using namespace Animorph;

float Animorph::wrapPi(float theta)
{
	theta += M_PI;
	theta -= floor(theta / (2.0 * M_PI)) * 2.0 * M_PI;
	theta -= M_PI;

	return theta;
}
