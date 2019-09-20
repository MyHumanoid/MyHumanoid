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

#ifndef BSPANEL_H
#define BSPANEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <algorithm>
#include <gui/Panel.h>
#include <gui/MultiPanel.h>
#include "TargetSlider.h"
#include "ImageListener.h"
#include "util.h"
#include "PageBsPanel.h"
#include "ImageButton.h"
#include "BsPanelSelectionListener.h"

using std::string;
using std::vector;

class BsPanel : public MultiPanel
{
private: // intentionally not implemented
  BsPanel             (const BsPanel&);
  BsPanel& operator = (const BsPanel&);

  BsPanelSelectionListener imgButtonListener;
  ImageListener            imgListener;
  string                   category;
  Image                    *close_image;
  // use local memory managment currently
  // TODO: implement managed widgets
  vector <ImageButton*> imageButtonVector;
  vector <Image*> imageVector;
  PageBsPanel *newPagePanel();
public:
  BsPanel (const string& category, const Rect& rect);
  virtual ~BsPanel ();

  void createWidgets ();
  const string& getCategory() const {return category;}
};

#endif // BSPANEL_H
