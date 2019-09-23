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
 *  File: util.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef UTIL_H
#define UTIL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Vector3.h"
#include "Vertex.h"
#include "VertexVector.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Animorph
{

enum RotateAxis { X_AXIS, Y_AXIS, Z_AXIS };

// delete all following 'characters' in 'str'
void UtilStringDelFollow(std::string &str, const std::string &characters);

// delete all leading 'characters' in 'str'
void UtilStringDelLead(std::string &str, const std::string &characters);

// delete all surrounding 'characters' in 'str'
void UtilStringDelSurround(std::string &str, const std::string &characters);

// some functions that are very usefull for writing files
bool hasFileEnding(const std::string &filename, const std::string &ending);
std::string cutFileEnding(std::string filename, const std::string &ending = "");

// some generic template functions for delete algorithms
template <typename T> void delete_one(T *t)
{
	delete t;
	t = NULL;
}

template <typename T> void delete_array(T *t)
{
	delete[] t;
	t = NULL;
}

// '<<' operator for vector class
/*template <typename T>
std::ostream &operator << (std::ostream &s, std::vector<T> iv)
{
  s << "[";
  for (unsigned i = 0; i < iv.size (); i++)
  {
    s << iv[i];
    if (i < iv.size ()-1)
      s << ",";
  }
  s << "]" << std::endl;

  return s;
}*/

/// print Vector on std::cout
template <typename T> void printVector(std::vector<T> iv)
{
	std::cout << "[";
	for (unsigned i = 0; i < iv.size(); i++) {
		std::cout << iv[i];
		if (i < iv.size() - 1)
			std::cout << ",";
	}
	std::cout << "]" << std::endl;
}

/// create std::string from any number
template <typename T> std::string toString(const T &thing, int w = 0, int p = 0)
{
	std::ostringstream os;
	os << std::setw(w) << std::setprecision(p) << thing;
	return os.str();
}

/// line - to extract tokens from
/// seperator - token separator characters
/// result - sequential string container
template <typename T>
void StringToken(const std::string &line, const std::string &separator,
                 T &result)
{
	std::string::size_type start = line.find_first_not_of(separator);
	if (std::string::npos == start)
		return; // nothing found

	do {
		std::string::size_type end = line.find_first_of(separator, start);
		if (std::string::npos == end) {
			result.push_back(line.substr(start));
			start = std::string::npos; // finish loop
		} else {
			result.push_back(line.substr(start, end - start));
			start = line.find_first_not_of(separator, end);
		}
	} while (std::string::npos != start);
}

template <typename T>
void stringTokeni(const string &line, const std::string &separator, T &result)
{
	std::string::size_type start = line.find_first_not_of(separator);
	if (std::string::npos == start)
		return; // nothing found

	do {
		std::string::size_type end = line.find_first_of(separator, start);
		if (std::string::npos == end) {
			result.push_back(std::atoi(line.substr(start).c_str()));
			start = std::string::npos; // finish loop
		} else {
			result.push_back(std::atoi(line.substr(start, end - start).c_str()));
			start = line.find_first_not_of(separator, end);
		}
	} while (std::string::npos != start);
}

int replaceString(const string &match, const string &replace, string &str,
                  unsigned int maxReplace = 0);

/*! \brief Returns the location of the center of gravity
 * \param vertexNumbers a vector of indices into vertexvector
 * \param vertexvector a vector of vertices, from which only the ones indicated
 * by vertexNumbers are used \return location of the center of gravity
 */
Vector3f calcCenteroid(const vector<int> &vertexNumbers,
                       const VertexVector &vertexvector);

Vector3f calcAverageNormalLength(const vector<int> vertexNumbers,
                                 const VertexVector &vertexvector);

} // namespace Animorph

#endif // UTIL_H
