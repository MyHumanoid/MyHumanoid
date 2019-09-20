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
 *  File   : BsPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POSESBSCATEGORYPANEL_H
#define POSESBSCATEGORYPANEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <algorithm>
#include <mhgui/Panel.h>
#include "PosesBsSelectionListener.h"
#include "TargetSlider.h"
#include "ImageListener.h"
#include "util.h"
#include "ComponentID.h"

using std::string;
using std::vector;

 static const uint32_t poses_bs_cat_table_id[17] = {
  kComponentID_PosesBsCategoryPanel_Category1,
  kComponentID_PosesBsCategoryPanel_Category2,
  kComponentID_PosesBsCategoryPanel_Category3,
  kComponentID_PosesBsCategoryPanel_Category4,
  kComponentID_PosesBsCategoryPanel_Category5,
  kComponentID_PosesBsCategoryPanel_Category6,
  kComponentID_PosesBsCategoryPanel_Category7,
  kComponentID_PosesBsCategoryPanel_Category8,
  kComponentID_PosesBsCategoryPanel_Category9,
  kComponentID_PosesBsCategoryPanel_Category10,
  kComponentID_PosesBsCategoryPanel_Category11,
  kComponentID_PosesBsCategoryPanel_Category12,
  kComponentID_PosesBsCategoryPanel_Category13,
  kComponentID_PosesBsCategoryPanel_Category14,
  kComponentID_PosesBsCategoryPanel_Category15,
  kComponentID_PosesBsCategoryPanel_Category16,
  kComponentID_PosesBsCategoryPanel_Category17
};

class PosesBsCategoryPanel : public Panel
{
private: // intentionally not implemented
  PosesBsCategoryPanel             (const PosesBsCategoryPanel&);
  PosesBsCategoryPanel& operator = (const PosesBsCategoryPanel&);

  PosesBsSelectionListener          selectionListener;

  // use local memory managment currently
  // TODO: implement managed widgets
  vector <Image*> imageVector;
  StringList *str_list;
public:
  PosesBsCategoryPanel ();
  virtual ~PosesBsCategoryPanel ();

  void createWidgets ();
  void resetTargetValues(const string currentTargetName = "");
  bool checkEnabled(string category);
};

#endif // POSESBSCATEGORYPANEL_H
