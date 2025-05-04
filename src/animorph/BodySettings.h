/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005  Andreas Volz
 *  Copyright (C) 2006-2007  MakeHuman Project
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
 *  File: BodySettings.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "FileReader.h"
#include "FileWriter.h"
#include <iostream>
#include <map>
#include <vector>

#include "ComponentID.h"

namespace Animorph
{

struct SavedCompSetting
{
	glm::ivec2 m_kAge = InvalidPoint;
	glm::ivec2 m_kMuscleSize = InvalidPoint;
	glm::ivec2 m_kBreast = InvalidPoint;
	glm::ivec2 m_kShape = InvalidPoint;
};

/*! \brief A map of names and associated values

The map can be saved to and loaded from a file.

The format of BodySettings file:
\verbatim
<string>,<float>
...
\endverbatim
*/
class BodySettings
{
public:
	std::map<std::string, float> m_targets;
	SavedCompSetting m_comp;
};

bool loadBodySettings(BodySettings &bs, const std::string & filename);
bool saveBodySettings(BodySettings &bs, const std::string & filename);

} // namespace Animorph
