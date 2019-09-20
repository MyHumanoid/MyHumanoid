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
 *  File: OgreXMLExporter.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef OGREXMLEXPORTER_H
#define OGREXMLEXPORTER_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif
#include "xmlParser.h"
#include <iostream>
#include <fstream>
#include "Mesh.h"
#include "Matrix.h"
#include "util.h"

using std::ostringstream;
using std::string;


namespace Animorph {

class OgreXMLExporter
{
protected:
  Mesh &mesh;
  Matrix tm;

  void InsertASubmeshNode(XMLNode *xNodeSubmeshes,XMLNode *xNodeSubmesh, int index_material);
  void CreateLibraryMaterialsNode(struct XMLNode *xNode_library_materials,XMLNode *xNode_library_effects);
  void CreatePolygons(XMLNode *xNode_mesh ,string name);

public:
   /*!
   * \param _mesh construct ColladaExporter from a Mesh object
   */
  OgreXMLExporter (Animorph::Mesh &_mesh) : mesh (_mesh)
  {
    tm.identity ();
  }

  /*!
   * \param tm the Matrix which transformates the Mesh before exporting
   */
  void setTransformationMatrix (Matrix &tm) {this->tm = tm;}

  /// export the Mesh and MTL file in the same file .mesh.xml
  /*!
   * \param filename the file to save
   * \return true if file is saved
   * \return false if file couldn't be saved
  */
  bool exportFile (const string& filename);


};
}
#endif
