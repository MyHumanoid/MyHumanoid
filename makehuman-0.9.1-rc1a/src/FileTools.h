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
 *  File   : FileTools.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *  Author : Hans-Peter Dusel <hdusel@tangerine-soft.de>
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef _FileTools_H_
#define _FileTools_H_

/* To Do: This code needs to be verified on WIN32 first!
 * It works well on Mac OS X (BSD) and Linux */
#ifndef _WIN32

#include <string>
#include <list>
#include <cassert>

using std::string;
using std::list;

/* ========================================================================== *
 * FileTools - Includes
 * ========================================================================== */

namespace FileTools
{

    // Some basic file routines which actually does not habe to do something with the MacFS
    bool createEmptyFileIfDoesNotExists(const string& inFilename,
                                        mode_t inPermissions = 0600);

    const string appendFilenameToPath(const string &inPath, const string& inFilename);

    off_t   getFileSize     (const string& inFilename);
    bool    fileExists      (const string& inFilename);
    bool    isDirectory     (const string& inFilename);
    bool    isRegularFile   (const string& inFilename);
    bool    rmDir           (const string& inDirPath, bool inRecursive=false);
    bool    makeDir         (const string& inDirPath, mode_t inMode=0744);
    bool    makeDirHier     (const string& inDirPath, mode_t inMode=0744);

    bool    rmFile          (const string& inFilename);

    mode_t  getProtectionFlags (const string& inFilename);
    mode_t  getModeFlags    (const string& inFilename);

    bool    copyFile(const string& inSrcFilename, const string& inDstFilename, bool inCreateIntermediateDirs=false);
    bool    linkFile(const string& inSrcFilename, const string& inDstFilename, bool inCreateIntermediateDirs=false);

    ssize_t readFileIntoBuffer  (const string& inFilename,       void* outBuffer, size_t readSize);

    ssize_t writeFileFromString (const string& inFilename, const string& inData,                    mode_t inMode=0600);
    ssize_t writeFileFromBuffer (const string& inFilename, const void* inBuffer,  size_t writeSize, mode_t inMode=0600);
    ssize_t appendFileFromBuffer(const string& inFilename, const void* inBuffer,  size_t writeSize, mode_t inMode=0600);

    // Deal with file- and pathnames
    const string getFilePath                (const string& inFilename);
    const string getFilePrefix              (const string& inFilename);
    const string getFileNameWithPrefix      (const string& inFilename);
    const string getFileNameWithoutPrefix   (const string& inFilename);

    bool    touchFile(const string& inFilename, mode_t inMode=0600);

    const list<string> getDirs        (const string& inFromDir, size_t inMaxDepth=(size_t)-1);
    const list<string> getFiles       (const string& inFromDir, size_t inMaxDepth=(size_t)-1);
    const list<string> getFilesAndDirs(const string& inFromDir, size_t inMaxDepth=(size_t)-1);

    int64_t getLastAccessTime         (const string& inFilename);
    int64_t getLastModificationTime   (const string& inFilename);
    int64_t getLastChangedTime        (const string& inFilename);
} // namespace FileTools

#endif // _WIN32 - To Do: Verify this code On Wind32 first!

#endif // _FileTools_H_
