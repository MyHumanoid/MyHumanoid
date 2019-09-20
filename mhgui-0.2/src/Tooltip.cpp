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
 *  File   : Tooltip.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/mhgui/Tooltip.h"
#include "../include/mhgui/Panel.h"
#include "../include/mhgui/CGUtilities.h"
#include "../include/mhgui/GLUTWrapper.h"

namespace mhgui {

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Tooltip::Tooltip(const std::string&  inTooltipString,
                 const Point&   inPos,
                 const Color&   inColor,
                 const Panel   *inAnchor)
: label       (inTooltipString),
  pos         (inPos),
  color       (inColor),
  anchorPanel (inAnchor)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Tooltip::Tooltip(const std::string&  inTooltipString,
                 const Point&   inPos,
                 const Panel   *inAnchor)
: label       (inTooltipString),
  pos         (inPos),
  color       (Color(1.0, 0.0, 0.0)),
  anchorPanel (inAnchor)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Tooltip::Tooltip(const Tooltip&  inTooltip)
: label       (inTooltip.label),
  pos         (inTooltip.pos),
  color       (inTooltip.color),
  anchorPanel (inTooltip.anchorPanel)
{
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
Tooltip& Tooltip::operator=(const Tooltip&  inTooltip)
{
    if (&inTooltip != this)
    {
        label       = inTooltip.label;
        pos         = inTooltip.pos;
        color       = inTooltip.color;
        anchorPanel = inTooltip.anchorPanel;
    }
    return *this;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Tooltip::draw() const
{
    cgutils::drawString(Point(0,0), GLUT_BITMAP_HELVETICA_12, " ", color);

    if(anchorPanel == NULL)
    {
        cgutils::drawSquareFill(Rect(-3, -12, cgutils::getFontLength(GLUT_BITMAP_HELVETICA_12, label) + 3, 16),
                                  Color(0,0,0));
        cgutils::drawString(pos, GLUT_BITMAP_HELVETICA_12, label.c_str (), color);
    }
    else
    {
        Point tmpPos(anchorPanel->getPosition());
        tmpPos.moveBy(pos);

        cgutils::drawSquareFill(Rect(tmpPos.getX() - 3, tmpPos.getY() - 12, cgutils::getFontLength(GLUT_BITMAP_HELVETICA_12, label) + 3, 16),
                                 Color(0,0,0));
        cgutils::drawString(tmpPos, GLUT_BITMAP_HELVETICA_12, label.c_str (), color);
    }
}

} // namespace mhgui

