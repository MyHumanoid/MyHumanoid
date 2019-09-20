## Process this file with automake to produce Makefile.in
## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

SUBDIRS = src data pixmaps

makehumandocdir = ${prefix}/doc/makehuman
makehumandoc_DATA = \
	README.txt\
	COPYING\
	COPYING.MIT\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO

OSX_DIST = os-x/MakeHuman.xcodeproj/project.pbxproj\
	   os-x/resources/AppIcon.icns\
	   os-x/resources/Credits.html\
	   os-x/resources/Info_10.3.9.plist\
	   os-x/resources/MainMenu.nib/classes.nib\
	   os-x/resources/MainMenu.nib/info.nib\
	   os-x/resources/MainMenu.nib/keyedobjects.nib\
	   os-x/src/MACFileUtils.cpp\
	   os-x/src/MACFileUtils.h\
	   os-x/src/MainController.h\
	   os-x/src/MainController.mm\
	   os-x/src/main.mm\
	   os-x/src/makehuman_Prefix.pch

EXTRA_DIST = $(makehumandoc_DATA)\
	     $(OSX_DIST)\
	     makehuman.cbp\
	     makehuman.dev\
	     makehuman.ico\
	     makehuman.nsi.in

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

