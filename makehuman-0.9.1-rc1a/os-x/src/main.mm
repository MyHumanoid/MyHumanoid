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
 *  File   : main.mm
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *  Author : Hans-Peter Dusel <hdusel@tangerine-soft.de>
 *
 *  For individual developers look into the AUTHORS file.
 *   
 */

#import <Cocoa/Cocoa.h>
#import <CoreServices/CoreServices.h>

#import <libgen.h>
#import <unistd.h>
#import <GLUT/GLUT.h>
#import <Animorph/util.h>

#import <sys/stat.h>
#import <dirent.h>
#import "mhgui/MouseEventTrap.h"

int main(int argc, char *argv[])
{
    initMouseScrollWheelTrap();
    ::glutInit (&argc, argv);
    ::chdir(::dirname(argv[0]));
    return NSApplicationMain(argc,  (const char **) argv);
}
