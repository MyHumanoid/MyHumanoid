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
 *  File   : BodyPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "TargetSelectionListener.h"
#include "util.h"
#include <gui/Image.h>
#include <gui/Panel.h>
#include <vector>

using std::vector;

class BodyPanel : public Panel
{
public:
	BodyPanel();
	virtual ~BodyPanel();

	void createWidgets();

private: // intentionally not implemented
	BodyPanel(const BodyPanel &);
	BodyPanel & operator=(const BodyPanel &);

private:
	TargetSelectionListener imgListener;
	vector<Image *>         imageVector;
};
