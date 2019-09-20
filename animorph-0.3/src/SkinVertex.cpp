/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  Copyright (C) 2006  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  2  of
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
 *  File   : SkinVertex.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/animorph/SkinVertex.h"

using namespace Animorph;

SkinVertex::SkinVertex (int _skinVertex, float _originalDist, const std::vector<int> &tokens)
  : skinVertex(_skinVertex),
    linkedMuscles(tokens),
    linkedMusclesSet(),
    originalDist(_originalDist)
    //oldCenteroid()
  {
    linkedMusclesSet.insert(tokens.begin(), tokens.end());
  }



