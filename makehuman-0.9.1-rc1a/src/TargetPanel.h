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
 *  File   : TargetPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef TARGETPANEL_H
#define TARGETPANEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <algorithm>
#include <mhgui/Panel.h>
#include <mhgui/MultiPanel.h>
#include "TargetSliderListener.h"
#include "TargetSlider.h"
#include "ImageListener.h"
#include "util.h"
#include "PageTargetPanel.h"

using std::string;
using std::vector;

static const string applied_target_list = "#nobody_use_such_a_targetdir&%$§";

class TargetPanel : public MultiPanel
{
private: // intentionally not implemented
  TargetPanel             (const TargetPanel&);
  TargetPanel& operator = (const TargetPanel&);

  TargetSliderListener   imgSliderListener;
  ImageListener          imgListener;
  string                 category;

  // use local memory managment currently
  // TODO: implement managed widgets
  vector <TargetSlider*> targetVector;
  vector <Image*> imageVector;
  PageTargetPanel *newPagePanel();

public:
  TargetPanel (const string& category, const Rect& rect);
  virtual ~TargetPanel ();

  void createWidgets ();
  const string& getCategory() const {return category;}
  void resetTargetValues(const string currentTargetName = "");
};

#endif // TARGETPANEL_H
