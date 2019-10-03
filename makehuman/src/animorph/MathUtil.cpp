#include "animorph/MathUtil.h"

using namespace Animorph;

float Animorph::wrapPi(float theta)
{
	theta += glm::pi<float>();
	theta -= floor(theta / (2.0 * glm::pi<float>())) * 2.0 * glm::pi<float>();
	theta -= glm::pi<float>();

	return theta;
}
