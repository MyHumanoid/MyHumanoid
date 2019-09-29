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
 *  File: FaceVector.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef FACEVECTOR_H
#define FACEVECTOR_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Face.h"
#include "FileReader.h"
#include <fstream>
#include <iostream>

namespace Animorph
{

/*! \brief Represents a vector of faces.

Can be loaded from a file.

The format of Faces file:
\verbatim
<int>,<int>,<int>{,<int>}
...
\endverbatim
The format of Face colors file:
\verbatim
<int>
...
\endverbatim
*/
class FaceVector : public std::vector<Face>
{
private:
	void fromGeometryStream(std::ifstream & in_stream);
	void fromColorsStream(std::ifstream & in_stream);

public:
	/// load the Face data from a file
	/*!
	 * \param filename the file to load
	 * \return true if file is found
	 * \return false if file isn't found
	 */
	bool loadGeometry(const std::string & filename);

	/// load the Face Color data from a file
	/*!
	 * \param filename the file to load
	 * \return true if file was found
	 * \return false if file wasn't found
	 */
	bool loadColors(const std::string & filename);
};

} // namespace Animorph

#endif // FACEVECTOR_H
