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
 *  File: DirectoryList.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef DIRECTORYLIST_H
#define DIRECTORYLIST_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <list>

using std::string;
using std::list;

typedef list <string> StringList;

namespace Animorph {

/*! \brief Represents a subset of files in a directory hierarchy

To use this class:

-# instantiate it
-# set its parameters with setRootPath(), setRecursive(), setFileFilter() and setFileType()
-# access the list of matching files with getDirectoryList()

 */
class DirectoryList
{
private:
  /// recursive function to read directory list
  void readDirRecursive (const string& dir_str);
  bool hasFileFilterEnding (const string& file) const;

  StringList file_list;
  string file_filter;
  string path;
  int recursive_level;
  int recursive_counter;
  int file_type;

public:
  DirectoryList ();

  enum
  {
    NO_LIMIT = -1,   /*!< read directories recursive without limit */
    NO_RECURSIVE = 0 /*!< read only the base directory without recursion */
  };

  enum FileType
  {
    REGULAR_FILE, /*!< only regular files */
    DIRECTORY     /*!< only directories */
  };

  /// set a pattern of files to pay attention
  /*!
   *
   * \param file_filter Only files with this ending are paid attention. If a
   *                    dot is in the extension, it need to be included.
   *			(e.g. ".target")
   */
  void setFileFilter (const string& file_filter);

  /// set the root path where to start file listing
  /*!
   * \param path the root path
   */
  void setRootPath (const string& path);

  /// set the recursive level
  /*!
   * \param recursive_level The number of maximim recursive directories to
   *			    search for files. Set to NO_LIMIT if the directory
   * 			    should be read without recursive limit. And set to
   *                        NO_RECURSIVE if only the files in the root
   *			    directory should be listed.
   */
  void setRecursive (int recursive_level);

  /*!
   * \return the complete list of all matching files
   */
  const StringList& getDirectoryList ();

  /// set the file types of interest (default: REGULAR_FILE)
  /*!
   * \param file_type the file type
   */
  void setFileType (FileType file_type);
};

}

#endif	// DIRECTORYLIST_H
