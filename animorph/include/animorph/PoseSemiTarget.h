/* SPDX-License-Identifier: GPL-2.1-or-later */
/*
 *  Copyright (C) 2006  MakeHuman Project
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  File   : PoseSemiTarget.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Lib    : animorph
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef POSESEMITARGET_H
#define POSESEMITARGET_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string>
#include <vector>
#include <set>

#include <sstream>
#include <iostream>
#include <fstream>

#include "util.h"
#include "FileReader.h"
#include "Matrix.h"

using std::string;
using std::set;
using std::vector;

namespace Animorph {

struct PoseTargetData
{
  int   vertex_number;
  float rotation;
};

typedef set<int> UsedVertex;

/*!
The format of PoseSemiTarget file:
\verbatim
<int>,<float>
...
\endverbatim
The format of PoseSemiTarget info file:
\verbatim
<int>,<int>,<int>,...
[X,Y,Z]
\endverbatim
*/
class PoseSemiTarget : public vector <PoseTargetData>
{
private:
  vector<int> centerVertexNumbers;
  RotateAxis  axis;
  UsedVertex  modVertex;

  bool hasCenter;
  Vector3f center;
  void stringTokenize(const string& str, vector<int>& tokens);

public:
  PoseSemiTarget();
  /*!
   * \param filename the file with PoseTarget data to load
   * \return true if file is found
   * \return false if file isn't found
   */
  bool load (const string& filename);

  const vector<int> &getCenterVertexNumbers () {return centerVertexNumbers;}
  UsedVertex &getModVertex () {return modVertex;}
  const RotateAxis& getAxis () const {return axis;}

  bool getHasCenter() const {return hasCenter;}
  void setHasCenter(bool c) {hasCenter = c;}

  const Vector3f &getCenter() const {return center;}
  void setCenter(const Vector3f& c) {center = c;}
};

}

#endif	// POSESEMITARGET_H

