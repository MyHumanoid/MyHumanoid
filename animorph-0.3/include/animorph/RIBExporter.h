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
 *  File: RIBExporter.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef RIBEXPORTER_H
#define RIBEXPORTER_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif
#include <iostream>
#include <fstream>
#include <iostream>
#include <list>
#include "Mesh.h"
#include "Matrix.h"
#include "util.h"
#include "FaceGroup.h"

using std::string;
using std::list;
using std::ostringstream;
using std::ifstream;

namespace Animorph {

typedef std::pair <string, string> StringPair;

/*! \brief Export Mesh objects as RenderMan Interface Bytestream
 *
 * See eg. http://www.3dartist.com/WP/formats/index.html#rib
 *
 */
class RIBExporter
{
protected:
  Mesh &mesh;
  Matrix tm;

/*
  void createObjectStream (ostringstream &outStream,
                           const string& basename);
*/
  void createObjectStream (ostringstream &out_stream,
                           const FGroupData &facegroupdata,
                           const VertexData &vertexgroupdata);

  void replaceRIBTags (ifstream                &in_stream,
                       ostringstream           &outStream,
                       const list <StringPair> &replaceList);

public:
  /*!
   * \param _mesh construct RIBExporter from a Mesh object
   */
  RIBExporter (Animorph::Mesh &_mesh) : mesh (_mesh)
  {
    tm.identity ();
  }

  /*!
   * \param tm the Matrix which transforms the Mesh before exporting
   */
  void setTransformationMatrix (const Matrix &tm) {this->tm = tm;}

  ///
  /*!

  */
  bool exportFile (const string &outFile);

  bool exportFile (const string            &templateDirectory,
                   const string            &templateFile,
                   const string            &outFile,
                   const list <StringPair> &replaceList);

};

}

#endif	// OBJEXPORTER_H
