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

#ifndef FACEGROUP_H
#define FACEGROUP_H 1

#include <map>
#include <ios>
#include "Face.h"
#include "FaceVector.h"
#include "FileWriter.h"
#include "util.h"

namespace Animorph {

typedef std::vector <int> FGroupData;
typedef std::map <int, int> VertexData;

struct FGroup
{
  bool visible;
  /// A vector of ints
  FGroupData facesIndexes;
  FGroupData facesIndexes_subd; ///subdivision
};

/*! \brief A map of FaceGroup identifiers (part names) via associated vertex group numbers to vertex indices

Can be loaded from and saved to a file.

The format of FaceGroup file:
\verbatim
<string>
<int> <int> ...
\endverbatim
*/
class FaceGroup : public std::map <std::string, FGroup>
{
private:
  void fromStream (std::ifstream &in_stream);
  void createStreamVisibilities (std::ostringstream &out_stream);
  void fromStreamVisibilities (std::ifstream &in_stream);

  bool loaded;
  /// Maps FaceGroup identifiers via vertex group numbers to vertex indices
  mutable std::map <std::string, VertexData> vertexes;

public:
  FaceGroup() : loaded(false), vertexes() {};
  bool load (const std::string& filename);
  bool saveVisibilities (const std::string& filename, std::ios_base::openmode mode = std::ios::app);
  bool loadVisibilities (const std::string& filename);
  void toggleVisible(const std::string& name);
  /// Extracts our vertex indices from a given FaceVector
  void calcVertexes (const FaceVector &facevector);
  const VertexData &getPartVertexesRef(const std::string &part) const {return vertexes[part];}
};

}

#endif //FACEGROUP_H
