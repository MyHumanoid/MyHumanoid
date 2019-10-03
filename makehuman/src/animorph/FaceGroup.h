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
 *  File: FaceGroup.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include <optional>

#include "Face.h"
#include "FaceVector.h"
#include "FileWriter.h"
#include "util.h"
#include <ios>
#include <map>

#include "render/GlTypes.h"

namespace Animorph
{

typedef std::vector<int>   FGroupData;
typedef std::map<int, int> VertexData;

struct FGroup {
	bool                       visible;
	std::optional<mh::Texture> texture;
	std::optional<mh::Texture> specular;
	std::optional<mh::Texture> bump;

	/// A vector of ints
	FGroupData facesIndexes;
};

/*! \brief A map of FaceGroup identifiers (part names) via associated vertex
group numbers to vertex indices

Can be loaded from and saved to a file.

The format of FaceGroup file:
\verbatim
<string>
<int> <int> ...
\endverbatim
*/
class FaceGroup
{
private:
	void fromStream(std::ifstream & in_stream);
	void createStreamVisibilities(std::ostringstream & out_stream);
	void fromStreamVisibilities(std::ifstream & in_stream);

public:
	std::map<std::string, FGroup> m_groups;

	bool load(const std::string & filename);
	bool saveVisibilities(const std::string &     filename,
	                      std::ios_base::openmode mode = std::ios::app);
	bool loadVisibilities(const std::string & filename);
	void toggleVisible(const std::string & name);
};

} // namespace Animorph
