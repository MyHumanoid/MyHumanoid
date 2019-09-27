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
 *  File: Hotspot.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef HOTSPOT_H
#define HOTSPOT_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "FileReader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace Animorph
{

typedef std::vector<int> HotspotData;

/*! \brief Loadable map from hotspot names to vectors of ints

The format of Hotspot file:
\verbatim
<string>:
<int>
<int>
...
{<newline>
<hotspot>}
\endverbatim
*/
class Hotspot : public std::map<std::string, HotspotData>
{
private:
	void fromStream(std::ifstream &in_stream);

public:
	/// load a Hotspot
	/*!
	 * \param filename the file with Hotspot data to load
	 * \return true if file was found
	 * \return false if file wasn't found
	 */
	bool load(const std::string &filename);
};

} // namespace Animorph

#endif // HOTSPOT_H
