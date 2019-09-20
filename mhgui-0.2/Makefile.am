## Process this file with automake to produce Makefile.in
## Created by Anjuta - will *NOT* be overwritten

SUBDIRS = include src

libmhguidocdir = ${prefix}/doc/mhgui
libmhguidoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO

dist-devpak: all
	rm -rf $(PACKAGE)-$(VERSION)
	mkdir $(PACKAGE)-$(VERSION)
	mkdir $(PACKAGE)-$(VERSION)/lib
	mkdir $(PACKAGE)-$(VERSION)/include
	mkdir $(PACKAGE)-$(VERSION)/include/$(PACKAGE)
	if test -f $(PACKAGE).DevPackage; then \
		cp -p $(PACKAGE).DevPackage $(PACKAGE)-$(VERSION); \
	fi
	for header in include/$(PACKAGE)/*.h; do \
		if test -f $$header; then \
			cp -p $$header $(PACKAGE)-$(VERSION)/include/$(PACKAGE); \
		fi \
	done
	if test -f src/.libs/lib$(PACKAGE).a; then \
		cp -p src/.libs/lib$(PACKAGE).a $(PACKAGE)-$(VERSION)/lib; \
		ranlib $(PACKAGE)-$(VERSION)/lib/lib$(PACKAGE).a; \
		chmod 644 $(PACKAGE)-$(VERSION)/lib/lib$(PACKAGE).a; \
	fi
	if test -f README; then \
        	cp -p README $(PACKAGE)-$(VERSION)/README.txt; \
	fi
	if test -f COPYING; then \
        	cp -p COPYING $(PACKAGE)-$(VERSION)/COPYING.txt; \
	fi
	cd $(PACKAGE)-$(VERSION) && tar cfj ../$(PACKAGE)-$(VERSION).DevPak .
	rm -rf $(PACKAGE)-$(VERSION)

EXTRA_BIN_DISTS = dist-devpak

OSX_DIST = os-x/mhgui_framework.xcodeproj/project.pbxproj\
	   os-x/resources/Info.plist\
	   os-x/src/mhgui_framework_Prefix.pch
	   os-x/src/MouseEventTrap.h
	   os-x/src/MouseEventTrap.mm

EXTRA_DIST = $(libmhguidoc_DATA)\
	     $(OSX_DIST)
	     mhgui.pc.in\
	     mhgui-uninstalled.pc.in\
	     mhgui-env.sh.in\
	     libmhgui.cbp\
	     libmhgui.dev

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mhgui.pc

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done
