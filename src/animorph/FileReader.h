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
 *  File: FileReader.h
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

/// define the maximum length of an input line
#define MAX_LINE_BUFFER 1024

namespace Animorph
{

/*! \brief Wrapper for ifstream using C locale
 */
class FileReader
{
	std::ifstream m_stream;
	char * locale;
	
public:
	FileReader(const FileReader &) = delete;
	FileReader & operator=(const FileReader &) = delete;
	
	FileReader();
	~FileReader();
	
	bool get(std::ifstream::char_type & c);
	
	bool getline(std::string & buffer);
	
	/*!
	 * \param filename the file to open for reading
	 * \return false if the file could be opened.
	 */
	bool open(const std::string & filename);

	/// closes the currently opened file
	void close();
};

} // namespace Animorph
