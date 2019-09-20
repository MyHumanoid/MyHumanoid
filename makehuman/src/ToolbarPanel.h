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
 *  File   : ToolbarPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *   
 */

#ifndef TOOLBARPANEL_H
#define TOOLBARPANEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <gui/Panel.h>
#include <gui/Image.h>
#include "ImageListener.h"
#include "util.h"

using std::vector;

class ToolbarPanel : public Panel
{
public:
  ToolbarPanel ();
  virtual ~ToolbarPanel ();

  void createWidgets ();

private: // intentionally not implemented
  ToolbarPanel             (const ToolbarPanel&);
  ToolbarPanel& operator = (const ToolbarPanel&);

private:
  ImageListener imgListener1;
  vector <Image*> imageVector;
};

#endif // TOOLBARPANEL_H
