/* SPDX-License-Identifier: GPL-3.0-or-later */
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

/* To Do: This code needs to be verified on WIN32 first!
 * It works well on Mac OS X (BSD) and Linux */
#ifndef _WIN32

/* ========================================================================== *
 * FileTools - Includes
 * ========================================================================== */

#ifndef _FileTools_H_
    #include "FileTools.h"
#endif // _FileTools_H_

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <inttypes.h>
#include <dirent.h>
#include <limits.h>
#include <iostream>

#include <string.h>

/* ========================================================================== *
 * FileTools - Implementation
 * ========================================================================== */
enum dirmode_t{kModeDirs, kModeFiles, kModeBoth};
const static string kEmptyString;
const static list  <string> kEmptyList;

/* ========================================================================== *
 * Basic File IO which is not related to Mac OS
 * ========================================================================== */

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string FileTools::appendFilenameToPath(const string &inPath, const string& inFilename)
{
    string retPath(inPath);
    if (retPath.length()>0 && retPath[inPath.length()-1] != '/')
        retPath.append(1, '/');
    retPath.append(inFilename);
    return retPath;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::createEmptyFileIfDoesNotExists(const string& inFilename, mode_t inPermissions)
{
    int fileHD = ::open(inFilename.c_str(), O_WRONLY | O_CREAT | O_APPEND, inPermissions);
    bool rc = (fileHD >= 0);

    if (rc) ::close(fileHD);

    return rc;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
off_t FileTools::getFileSize(const string& inFilename)
{
    if (inFilename.empty())
        return -1LL;

    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return -1LL;
    return fileStat.st_size;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::fileExists(const string& inFilename)
{
    if (inFilename.empty())
        return false; // No input file -> not exists ;-)

    struct stat fileStat;
    return (::stat(inFilename.c_str(), &fileStat)==0);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::isDirectory(const string& inFilename)
{
    if (inFilename.empty())
        return false; // No input file -> not a directory ;-)

    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return false;
    return S_ISDIR(fileStat.st_mode);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::isRegularFile(const string& inFilename)
{
    if (inFilename.empty())
        return false; // No input file -> not a file ;-)

    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return false;
    return S_ISREG(fileStat.st_mode);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::rmDir(const string& inDirPath, bool inRecursive)
{
    if (inRecursive)
    {
        // Remove the Files first
        list<string> files(getFiles(inDirPath));
        list<string>::const_iterator fi;
        for (fi = files.begin(); fi != files.end(); ++fi)
        {
            rmFile(*fi);
        }

        // and then all dirs
        list<string> dirs(getDirs(inDirPath));
        list<string>::reverse_iterator di;
        for (di = dirs.rbegin(); di != dirs.rend(); ++di)
        {
            rmDir(*di, false);
        }
    }

    if (inDirPath.empty())
        return false; // no input file -> no action ;-)

    if (!isDirectory(inDirPath))
        return false; // no path -> no Action

    return (::rmdir(inDirPath.c_str()) == 0);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::makeDir(const string& inDirPath, mode_t inMode)
{
    if (inDirPath.empty())
        return false; // No input file -> no action ;-)

    if (isDirectory(inDirPath)) // already exists?
        return false;
    return (::mkdir(inDirPath.c_str(), inMode) == 0);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
static const string getPathUpToLevel(const string& inPath, int inLevel)
{
    int searchPos = 0;
    string::size_type foundPos = string::npos;
    for (int level=0; level<=inLevel;++level)
    {
        foundPos = inPath.find("/", searchPos);
        if (foundPos == string::npos)
            return string();
        searchPos = foundPos+1;
    }
    return (foundPos != string::npos) ? inPath.substr(0, foundPos+1) : string();
}

/* ========================================================================== */
/** // does the same as mkdir -p
 */
/* ========================================================================== */
bool FileTools::makeDirHier(const string& inDirPath, mode_t inMode)
{
    if (inDirPath.empty())
        return false; // No input file -> no action ;-)

    // Check if this Directory already exists
    if (isDirectory(inDirPath))
        return false;

    string givenPath(inDirPath);
    if (givenPath[givenPath.length()-1] != '/')
        givenPath.append(1, '/');

    for (int level=0;;++level)
    {
        // split extend the path beginning from the first path component
        string path(getPathUpToLevel(givenPath, level));

        if (path.empty())
            break;

        if (isDirectory(path)) // already exists?
            continue;

        if ((::mkdir(path.c_str(), inMode)) != 0)
            return false;
    }
    return true;
}

/* ========================================================================== */
/** Copy a file by preserving the Protection flags.
 *
 * @param inSrcFilename the path+name of the source file.
 * @param inDstFilename the path+name of the destination file.
 * @param inCreateIntermediateDirs set this to true whether the function should create
 *        the given path in inDstFilename if it does not exists.
 *        You may omit this parameter - in this case it defaults to false.
 *
 * @return true on success, or false on failure.
 *
 * Failure could be:
 *  o inSrcFilename and inDstFilename are identical names;
 *  o Either the source- or the destination File could not be successful read/written;
 */
/* ========================================================================== */
bool FileTools::copyFile(const string& inSrcFilename,
                            const string& inDstFilename,
                            bool inCreateIntermediateDirs)
{
    // If src and dest are identical, then do nothing!
    if (inSrcFilename.compare(inDstFilename) == 0)
        return false;

    if (inSrcFilename.empty() || !fileExists(inSrcFilename))
        return false;

    const size_t kMaxCopyBufferSize = 512*1024;
    mode_t wrPerm = getModeFlags(inSrcFilename);

    bool success = false; // failure by default

    if (inCreateIntermediateDirs)
    {
        makeDirHier(getFilePath(inDstFilename), 0755);
    }

    int rdFile = ::open(inSrcFilename.c_str(), O_RDONLY);
    if (rdFile >= 0)
    {
        int wrFile = ::open(inDstFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, wrPerm);
        if (wrFile >= 0)
        {
            size_t srcFileSize = getFileSize(inSrcFilename);
            size_t bufferSize = std::min<size_t>(kMaxCopyBufferSize, srcFileSize);
            void *buffer = ::malloc(bufferSize);
            if (buffer != NULL)
            {
                size_t writtenFileSize = 0;
                for(;;)
                {
                    ssize_t rdBytes = ::read(rdFile, (void*)buffer, bufferSize);
                    if (rdBytes == -1)
                        break;

                    if (rdBytes==0)
                    {
                        success = (srcFileSize == writtenFileSize);
                        break;
                    }

                    const uint8_t *wrPtr = (const uint8_t *)buffer;
                    size_t bytesToWrite = rdBytes;
                    for(;bytesToWrite>0;)
                    {
                        int written= ::write(wrFile, wrPtr, bytesToWrite);
                        if (written <= 0)
                            break;

                        wrPtr           += written;
                        bytesToWrite    -= written;
                        writtenFileSize += written;
                    }
                }
                ::free(buffer);
            }
            ::close(wrFile);
        }
        ::close(rdFile);
    }
    if (!success)
        rmFile(inDstFilename);
    return success;
}

/* ========================================================================== */
/** Create a symbolic link of a file.
 *
 * @param inSrcFilename the path+name of the source file.
 * @param inDstFilename the path+name of the destination file.
 * @param inCreateIntermediateDirs set this to true whether the function should create
 *        the given path in inDstFilename if it does not exists.
 *        You may omit this parameter - in this case it defaults to false.
 *
 * @return true on success, or false on failure.
 *
 * Failure could be:
 *  o inSrcFilename and inDstFilename are identical names;
 *  o Either the source- or the destination File could not be successful read/written;
 */
/* ========================================================================== */
bool FileTools::linkFile(const string& inSrcFilename,
                         const string& inDstFilename,
                         bool inCreateIntermediateDirs)
{
    // If src and dest are identical, then do nothing!
    if (inSrcFilename.compare(inDstFilename) == 0)
        return false;

    if (inSrcFilename.empty() || !fileExists(inSrcFilename))
        return false;

    if (inCreateIntermediateDirs)
    {
        makeDirHier(getFilePath(inDstFilename), 0755);
    }

    return (::symlink(inSrcFilename.c_str(), inDstFilename.c_str()) == 0);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool FileTools::rmFile(const string& inFilename)
{
    if (inFilename.empty())
        return false; // no input file -> no action ;-)

    if (!isRegularFile(inFilename))
        return false; // no file -> no Action

    return (::unlink(inFilename.c_str()) == 0);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
mode_t FileTools::getProtectionFlags (const string& inFilename)
{
    return getModeFlags(inFilename) & (S_IRWXU | S_IRWXG | S_IRWXO);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
mode_t FileTools::getModeFlags (const string& inFilename)
{
    if (inFilename.empty())
        return 0; // No input file -> no action ;-)

    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return 0; // File does not exists!
    return fileStat.st_mode;
}

/* ========================================================================== */
/** Touch the given file. This call does the same as the Unix 'touch' command
 *  e.g It creates an empty file if it does not already exists.
 */
/* ========================================================================== */
bool FileTools::touchFile(const string& inFilename, mode_t inMode)
{
    if (!fileExists(inFilename))
    {
        // Create an empty file if it does not exists!
        int fh = ::open(inFilename.c_str(), O_CREAT | O_WRONLY, inMode);
        if (fh < 0)
            return false;
        ::close(fh);
        return true;
    }
    else
    {
        return (0 == ::utimes(inFilename.c_str(), NULL));
    }
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ssize_t FileTools::readFileIntoBuffer(const string&  inFilename,
                                      void          *outBuffer,
                                      size_t         readSize)
{
    if (inFilename.empty())
        return -1; // No input file -> no action ;-)

    int fd;
    if ((fd = ::open(inFilename.c_str(), O_RDONLY)) < 0)
        return -1;

    int rc = -1; // mark "error" per default

    ssize_t remainingBytes = readSize;
    char   *buffer         = (char*)outBuffer;

    // Repeat until *all* bytes has been really read!
    for(;;)
    {
        ssize_t rdBytes = ::read(fd, buffer, remainingBytes);
        if (rdBytes < 0)
            break;

        remainingBytes -= rdBytes;
        assert (remainingBytes >= 0);

        if (remainingBytes == 0)
        {
            rc = readSize;
            break;
        }
        buffer += rdBytes; // advance buffer pointer
    }

    ::close(fd);

    return rc;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ssize_t FileTools::writeFileFromString(const string& inFilename,
                                       const string& inData,
                                       mode_t        inMode)
{
    return writeFileFromBuffer(inFilename, inData.c_str(), inData.length(), inMode);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ssize_t FileTools::writeFileFromBuffer(const string& inFilename,
                                       const void   *inBuffer,
                                       size_t        writeSize,
                                       mode_t        inMode)
{
    if (inFilename.empty())
        return -1; // No input file -> no action ;-)

    int fd;
    if ((fd = ::open(inFilename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, inMode)) < 0)
        return -1;

    int rc = -1; // mark "error" per default

    ssize_t remainingBytes = writeSize;
    const char *buffer     = (const char*)inBuffer;
    for (;;)
    {
        ssize_t wrBytes = ::write(fd, buffer, remainingBytes);
        if (wrBytes < 0) // 1 -if an error occured
            break;

        remainingBytes -= wrBytes;
        assert (remainingBytes >= 0);

        if (remainingBytes == 0)
        {
            rc = writeSize; // mark "all bytes are written"
            break;
        }
        buffer += wrBytes; // advance the pointer
    }
    ::close(fd);

    return rc;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
ssize_t FileTools::appendFileFromBuffer(const string& inFilename,
                                        const void *inBuffer,
                                        size_t      writeSize,
                                        mode_t      inMode)
{
    if (inFilename.empty())
        return -1; // No input file -> no action ;-)

    int fd;
    if ((fd = ::open(inFilename.c_str(), O_CREAT | O_WRONLY | O_APPEND, inMode)) < 0)
        return -1;

    ssize_t wrBytes = ::write(fd, inBuffer, writeSize);
    ::close(fd);

    return wrBytes;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string FileTools::getFilePath(const string& inFilename)
{
    if (inFilename.empty())
        return kEmptyString; // No input file -> no path ;-)

    size_t prefixPos = inFilename.rfind("/");

    /* Does the given File *not* contain a '/'?
     * If yes, then the handed over filename is just a filename *without* any path */
    if (prefixPos == string::npos)
        return kEmptyString; // we have no path here

    return inFilename.substr(0, prefixPos+1);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string FileTools::getFilePrefix(const string& inFilename)
{
    if (inFilename.empty())
        return kEmptyString; // No input file -> no prefix ;-)

    size_t prefixPos = inFilename.rfind(".");

    if (prefixPos == string::npos)
        return kEmptyString;

    return inFilename.substr(prefixPos+1, inFilename.length()-prefixPos-1);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string FileTools::getFileNameWithPrefix(const string& inFilename)
{
    if (inFilename.empty())
        return kEmptyString; // No input file -> no prefix ;-)

    size_t prefixPos = inFilename.find_last_of("/");

    /* Does the given File *not* contain a '/'?
     * If yes, then the handed over filename is just a filename *without* any path */
    if (prefixPos == string::npos)
        return inFilename;

    return inFilename.substr(prefixPos+1, inFilename.length()-prefixPos-1);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string FileTools::getFileNameWithoutPrefix(const string& inFilename)
{
    if (inFilename.empty())
        return kEmptyString; // No input file -> no filename ;-)

    string fileName(getFileNameWithPrefix(inFilename));
    if (fileName.empty())
        return fileName;

    size_t prefixPos = fileName.find_last_of(".");

    if (prefixPos == string::npos)
        return fileName;

    return fileName.substr(0, prefixPos);
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
static const list<string> getFilesAndDirs(const string& inFromDir,
                                          size_t        inMaxDepth,
                                          size_t        inCurrentDepth,
                                          dirmode_t     inMode)
{
    list<string> res;
#ifdef _WIN32
#warning "TO DO HERE"
#elif __CYGWIN__
#warning "TO DO HERE"
    std::cerr << "FileTools.cpp: getFilesAndDirs() on Cygwin not supported" << std::endl;
#else
    DIR *dir = opendir(inFromDir.c_str());
    if (dir != NULL)
    {
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {

//            printf("%s is type 0x%x\n", dp->d_name, dp->d_type);

            string newPath(inFromDir);
            newPath.append(dp->d_name);

            if (dp->d_type == DT_DIR)
            {
                if(strcmp(dp->d_name, "..") != 0 &&
                   strcmp(dp->d_name, ".") != 0)
                {
                    newPath.append("/");

                    if (inMode == kModeBoth || inMode == kModeDirs)
                    {
                        res.push_back(newPath);
                    }

                    if (inCurrentDepth<inMaxDepth)
                    {
                        // Step 1 : recursive call
                        list<string> tmp_res(getFilesAndDirs(newPath.c_str(), inMaxDepth, inCurrentDepth+1, inMode));
                        // Step 2 : append array to this one!
                        list<string>::const_iterator i;
                        for (i = tmp_res.begin(); i != tmp_res.end(); ++i)
                        {
                            string thisPath(*i);
                            res.push_back(thisPath);
                        }
                    }
                }
            }
            else if (dp->d_type == DT_REG && (inMode == kModeBoth || inMode == kModeFiles))
                    res.push_back(newPath);
        }
        closedir(dir);
    }
#endif
    return res;
}

/* ========================================================================== */
/** Get all directories (and just dirs but *no* files) recursive from a
 *  particular path.
 */
/* ========================================================================== */
const list<string> FileTools::getDirs(const string& inFromDir, size_t inMaxDepth)
{
    if (inFromDir.empty())
        return kEmptyList;

    string s(inFromDir);
    if (inFromDir[s.length()-1]!='/') s.append("/");

    return getFilesAndDirs(s.c_str(), inMaxDepth, 0, kModeDirs);
}

/* ========================================================================== */
/** Get all Files (and just files but *no* directories) recursive from a
 *  particular path.
 */
/* ========================================================================== */
const list<string> FileTools::getFiles(const string& inFromDir, size_t inMaxDepth)
{
    if (inFromDir.empty())
        return kEmptyList;

    string s(inFromDir);
    if (inFromDir[s.length()-1]!='/') s.append("/");

    return getFilesAndDirs(s.c_str(), inMaxDepth, 0, kModeFiles);
}


/* ========================================================================== */
/** Get all Files *and* files recursive from a particular path.
 */
/* ========================================================================== */
const list<string> FileTools::getFilesAndDirs(const string& inFromDir, size_t inMaxDepth)
{
    if (inFromDir.empty())
        return kEmptyList;

    string s(inFromDir);
    if (inFromDir[s.length()-1]!='/') s.append("/");

    return getFilesAndDirs(s.c_str(), inMaxDepth, 0, kModeBoth);
}

#if 0 // -x-
static const char *kVolumeBase="/Volumes/";
/* ========================================================================== */
/** Collect all Volumes which are available on this Machine.
 *
 *  @return a vector of string objects which collects all Volumes currently
 *          mounted.
 */
/* ========================================================================== */
const vector<string> FileTools::getDrives()
{
    vector <string> res;
#ifdef _WIN32
#warning "TO DO HERE"
#else
    DIR *dir = opendir(kVolumeBase);
    if (dir != NULL)
    {
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL)
        {
    //        printf("%s is type 0x%x\n", dp->d_name, dp->d_type);
            if (dp->d_type == DT_DIR &&
                strcmp(dp->d_name, "..") != 0 &&
                strcmp(dp->d_name, ".") != 0)
                res.push_back(dp->d_name);
        }
        (void)closedir(dir);
    }
#endif
    return res;
}
#endif

/* ========================================================================== */
/** Return the last access time of a given file in milliseconds since
 *  January the 1st 1970.
 *
 *  If the given time does not exists, then return 0;
 *
 * @param inFilename the File to examine.
 *
 * @return the last access time of the given file in ms since Jan-1st-1970 or
 * 0L if the file does not exists.
 *
 * @see getLastModificationTime(const char*)
 * @see getLastChangedTime(const char*)
 */
/* ========================================================================== */
int64_t FileTools::getLastAccessTime(const string& inFilename)
{
    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return 0LL; // File does not exists!
#if defined(linux) || defined(__CYGWIN__)
    return (fileStat.st_atime*1000);
#else
    return ((uint64_t)fileStat.st_atimespec.tv_sec * 1000LL) +
           ((uint64_t)fileStat.st_atimespec.tv_nsec / (1000LL*1000LL));
#endif
}

/* ========================================================================== */
/** Return the last modification time of a given file in milliseconds since
 *  January the 1st 1970.
 *
 *  If the given time does not exists, then return 0;
 *
 * @param inFilename the File to examine.
 *
 * @return the last modification time of the given file in ms since Jan-1st-1970 or
 * 0L if the file does not exists.
 *
 * @see getLastAccessTime(const char*)
 * @see getLastChangedTime(const char*)
 */
/* ========================================================================== */
int64_t FileTools::getLastModificationTime(const string& inFilename)
{
    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return 0LL; // File does not exists!
#if defined(linux) || defined(__CYGWIN__)
    return (fileStat.st_mtime*1000);
#else
    return ((uint64_t)fileStat.st_mtimespec.tv_sec * 1000LL) +
           ((uint64_t)fileStat.st_mtimespec.tv_nsec / (1000LL*1000LL));
#endif
}

/* ========================================================================== */
/** Return the last status change time of a given file in milliseconds since
 *  January the 1st 1970.
 *
 *  If the given time does not exists, then return 0;
 *
 * @param inFilename the File to examine.
 *
 * @return the last status change time of the given file in ms since Jan-1st-1970 or
 * 0L if the file does not exists.
 *
 * @see getLastAccessTime(const char*)
 * @see getLastModificationTime(const char*)
 */
/* ========================================================================== */
int64_t FileTools::getLastChangedTime(const string& inFilename)
{
    struct stat fileStat;
    if (::stat(inFilename.c_str(), &fileStat)!=0)
        return 0LL; // File does not exists!
#if defined(linux) || defined(__CYGWIN__)
    return (fileStat.st_ctime*1000);
#else
    return ((uint64_t)fileStat.st_ctimespec.tv_sec * 1000LL) +
           ((uint64_t)fileStat.st_ctimespec.tv_nsec / (1000LL*1000LL));
#endif
}

#endif // _WIN32 - To Do: Verify this code On Wind32 first!


