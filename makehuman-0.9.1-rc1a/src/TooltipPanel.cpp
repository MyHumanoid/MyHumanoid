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
 *  File   : TooltipPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "TooltipPanel.h"
#include "ImageListener.h"
#include "ComponentID.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Tooltip.h>

using namespace std;
using namespace Animorph;

TooltipPanel::TooltipPanel (const int winHeight)
: Panel (kComponentID_TooltipPanel, Rect(400,winHeight - 36,64,16)),
  tooltip     (NULL),
  tooltipLabel(NULL)
{
  setBottomAnchor (true);
}

TooltipPanel::~TooltipPanel ()
{
  delete tooltip;
  delete tooltipLabel;
}

void TooltipPanel::createWidgets ()
{
  // -------------------------------------------------------------------------
  tooltipLabel = new Image(kComponentID_Dummy,
                           searchPixmapFile ("ui/tooltips_label_1.png"),
                           Rect(0,0,16,16));
  addWidget (tooltipLabel);

  // -------------------------------------------------------------------------
  tooltipLabel = new Image(kComponentID_Dummy,
                           searchPixmapFile ("ui/tooltips_label_2.png"),
                           Rect(0,0,16,16));
  addWidget (tooltipLabel);

  // -------------------------------------------------------------------------
  tooltipLabel = new Image(kComponentID_Dummy,
                           searchPixmapFile ("ui/tooltips_label_3.png"),
                           Rect(0,0,16,16));
  addWidget (tooltipLabel);

  // -------------------------------------------------------------------------
  tooltip = new Image(kComponentID_Dummy,
                      searchPixmapFile ("ui/tooltips.png"),
                      Rect(0,0,16,16));
  addWidget (tooltip);
}
