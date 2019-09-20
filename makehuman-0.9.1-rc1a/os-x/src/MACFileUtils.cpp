/*
 *  Copyright (C) 2006  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  2  of
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
 *  File   : MACFileUtils.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *  Author : Hans-Peter Dusel <hdusel@tangerine-soft.de>
 *
 *  For individual developers look into the AUTHORS file.
 *   
 */

#include "MACFileUtils.h"
#include <CoreServices/CoreServices.h>

static const string kEmptyString;

/* ========================================================================== */
/**
 */
/* ========================================================================== */
static const string getNameOfFile(const FSRef* inFsRef)
{
    UInt8 path[256];

    if (::FSRefMakePath(inFsRef, path, sizeof(path)) != noErr)
        return kEmptyString;

    return string((const char*)path);
}

/* ========================================================================== */
/** Query a specific path within a given domain of the Mac OS Filesystem.
 *
 *  You may get for instance the Users Home Directory by setting the domain to
 *  kUserDomain and the kind to ''.
 *
 * Pls. refer the Mac OS FileManager Documents for appropriate Values.
 *
 * @param inDomain the Domain to determine the path to (e.g. kUserDomain,
 *         kOnSystemDisk, kSystemDomain, ...)
 * @param inKind The kind of Path to query. Pls refer the appropriate constants 
 *        on the Mac OS Filemanager Ref documentation (e.g. kDocumentsFolderType, 
 *        kApplicationSupportFolderType, ...)
 *         
 * @return a C++ string which contains the Users ApplicationSupportPath 
 *         (e.g. /Users/hdusel/Library/Application Support). The string is empty 
 *         if the requested path does not exists.
 */
/* ========================================================================== */
static const string getMacPath(short inDomain, OSType inKind)
{
    FSRef fsref;
    OSErr err;

    if ((err = ::FSFindFolder(inDomain, inKind, FALSE, &fsref)) != noErr)
        return kEmptyString;

    return getNameOfFile(&fsref).c_str();
}

/* ========================================================================== */
/** Get the name of The *Users* Application Support Folder
 * @return the Users ApplicationSupportPath (e.g. /Users/hdusel/Library/Application Support)
 */
/* ========================================================================== */
const string getUsersAppSupportFolderPath()
{
    return getMacPath(kUserDomain, kApplicationSupportFolderType);
}

/* ========================================================================== */
/** Get the name of The *Users* Document Folder
 * @return the Users ApplicationSupportPath (e.g. /Users/hdusel/Documents)
 */
/* ========================================================================== */
const string getUsersDocumentsFolderPath()
{
    return getMacPath(kUserDomain, kDocumentsFolderType);
}

/* ========================================================================== */
/** Get the name of The *Users* Desktop Folder
 * @return the Users ApplicationSupportPath (e.g. /Users/hdusel/Desktop)
 */
/* ========================================================================== */
const string getUsersDesktopFolderPath()
{
    return getMacPath(kUserDomain, kDesktopFolderType);
}

/* ========================================================================== */
/** Get the name of The *Users* Home Directury Folder
 * @return the Users ApplicationSupportPath (e.g. /Users/hdusel)
 */
/* ========================================================================== */
const string getCurrentUserHomeFolderPath ()
{
    return getMacPath(kOnSystemDisk, kCurrentUserFolderType);
}

/* ========================================================================== */
/** Get the name of The *Users* Desktop Folder
 * @return the Users ApplicationSupportPath (e.g. /Users)
 */
/* ========================================================================== */
const string getUserBaseFolderPath ()
{
    return getMacPath(kOnSystemDisk, kUsersFolderType);
}

/* ========================================================================== */
/** Get the name of The *System* Applications Folder
 * @return the Users ApplicationSupportPath (e.g. /Applications)
 */
/* ========================================================================== */
const string getApplicationFolderPath ()
{
    return getMacPath(kOnSystemDisk, kApplicationsFolderType);
}

/* ========================================================================== */
/** Get the name of The *Systems* Application Support Folder
 * @return the Users ApplicationSupportPath (e.g. /Library/Application Support)
 */
/* ========================================================================== */
const string getSystemAppSupportFolderPath()
{
    return getMacPath(kOnSystemDisk, kApplicationSupportFolderType);
}


