#include "animorph/Color.h"

using namespace Animorph;

namespace Animorph
{
/* ========================================================================== */
/**
 */
/* ========================================================================== */
Color::Color(const Color &inColor)
    : r(inColor.r)
    , g(inColor.g)
    , b(inColor.b)
    , a(inColor.a)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Color &Color::operator=(const Color &inRHS)
{
	r = inRHS.r;
	g = inRHS.g;
	b = inRHS.b;
	a = inRHS.a;

	return *this;
}

} // namespace Animorph
