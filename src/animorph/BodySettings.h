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
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace Animorph
{

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
private:
	void createStream(std::ostringstream & out_stream);
	void fromStream(std::ifstream & in_stream);

public:
	std::map<std::string, float> m_targets;
	
	/// load a BodySetting
	/*!
	 * \param filename the file with BodySettings data to load
	 * \return true if file is found
	 * \return false if file isn't found
	 */
	bool load(const std::string & filename);

	/// save a BodySetting
	/*!
	 * \param filename the file to save
	 * \return true if file is saved
	 * \return false if file couldn't be saved
	 */
	bool save(const std::string & filename);

	std::vector<string> cursorPositions;
};

} // namespace Animorph
