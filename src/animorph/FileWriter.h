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
 *  File: FileWriter.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

using std::ofstream;
using std::string;

namespace Animorph
{

/*! \brief Wrapper for ofstream using C locale
 */
class FileWriter : public ofstream
{
private:
	char * locale;

	// Intentionally declared as private because not implemented yet
private:
	FileWriter(const FileWriter &);
	FileWriter & operator=(const FileWriter &);

public:
	FileWriter()
	        : locale(NULL)
	{
	}

	/// destructor closes the file
	virtual ~FileWriter() { close(); }

	/*!
	 * \param filename the file to open for writing
	 * \param mode Used as parameter for ofstream::open()
	 * \return 0 if the file could be opened.
	 */
	virtual int open(const string &          filename,
	                 std::ios_base::openmode mode = std::ios_base::out);

	/// closes the currently opened file
	virtual void close();
};

} // namespace Animorph
