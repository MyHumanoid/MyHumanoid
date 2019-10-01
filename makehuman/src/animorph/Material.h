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
 *  File: Material.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef MATERIAL_H
#define MATERIAL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Color.h"
#include "FileReader.h"
#include <iostream>
#include <string>
#include <vector>

using std::string;
namespace Animorph
{

/*! \brief Represents a material by its name, surface colour and edge colour
 */
class Material
{
	Color  rgbCol;
	Color  edgeCol;
	string name;

public:
	/*!
	 * \return rgbCol the RGB Color values from this Material
	 */
	const Color & getRGBCol() const { return rgbCol; }

	/*!
	 * \return edgeCol the RGB edge color values from this Material
	 */
	const Color & getEdgeCol() const { return edgeCol; }

	/*!
	 * \param rgbCol set the RGB Color values for this Material
	 */
	void setRGBCol(const Color & rgbCol) { this->rgbCol = rgbCol; }

	/*!
	 * \param edgeCol set the RGB edge color values for this Material
	 */
	void setEdgeCol(const Color & edgeCol) { this->edgeCol = edgeCol; }

	/*!
	 * \return name the name of this Material
	 */
	const string & getName() const { return name; }

	/*!
	 * \param name set the name the name for this Material
	 */
	void setName(const std::string & name) { this->name = name; }
};

/*! \brief A loadable vector of materials

The format of Material file:
\verbatim
<string>,<float>,<float>,<float>
...
\endverbatim
*/
class MaterialVector : public std::vector<Material>
{
private:
	void fromStream(std::ifstream & in_stream);

public:
	/// load a Material file
	/*!
	 * \param filename the file with Material data to load
	 * \return true if file is found
	 * \return false if file isn't found
	 */
	bool loadMaterials(const std::string & filename);
};

} // namespace Animorph

#endif // MATERIAL_H
