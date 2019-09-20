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
 *  File: Target.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef TARGET_H
#define TARGET_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <set>
#include "Vector3.h"
#include "FileReader.h"
#include "FileWriter.h"

namespace Animorph {

typedef std::vector<int> UnsortedUsedVertex;

struct TargetData
{
  int vertex_number;
  Vector3f morph_vector;
};

/*! \brief A set of vertices and associated displacements.

A target is a list of vertex indices and associated displacements.
It can be loaded from and saved to a file.

Mesh::doMorph() uses targets to manipulate a base mesh.

The format of Target files:
\verbatim
<int>,<float>,<float>,<float>
...
\endverbatim
*/
class Target : public std::vector <TargetData>
{
private:
  //void fromStream (std::ifstream &in_stream);
  void createStream (std::ostringstream &out_stream);
  /// The indices of the vertices from the Target file
  UnsortedUsedVertex modVertex;

public:

  /*!
   * \param filename the file with Target data to load
   * \return true if file was found
   * \return false if file wasn't found
   */
  bool load (const std::string& filename);

  /*!
   * \param filename the file with Target data to save
   * \return true if file was found
   * \return false if file wasn't found
   */
  bool save (const std::string& filename);

  UnsortedUsedVertex &getModVertex () {return modVertex;}
};

}

#endif	// TARGET_H
