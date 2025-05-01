#include "animorph/Color.h"

namespace Animorph
{
/* ========================================================================== */
/**
 */
/* ========================================================================== */
Color::Color(const Color & inColor)
        : m_color(inColor.m_color)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Color & Color::operator=(const Color & inRHS)
{
	m_color.r = inRHS.m_color.r;
	m_color.g = inRHS.m_color.g;
	m_color.b = inRHS.m_color.b;
	m_color.a = inRHS.m_color.a;

	return *this;
}

} // namespace Animorph
