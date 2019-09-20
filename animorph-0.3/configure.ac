dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta - will *NOT* be overwritten

AC_INIT(Animorph Library, 0.3, linux@brachttal.net)
AM_INIT_AUTOMAKE(animorph, 0.3)
AM_CONFIG_HEADER(config.h)

dnl Disable libtool 1.5 support for languages we don't use
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_GCJ_CONFIG], [:])dnl

AM_MAINTAINER_MODE

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_CXX
AC_HEADER_STDC
AC_PROG_LIBTOOL

dnl Check for option to enable debug
AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE(debug,
        [  --enable-debug=[no/yes]        enables debug build (default=no)],,
                 enable_debug=no)

dnl Check for programs

dnl Checks for libraries
if [ ! test "x$enable_debug" != "xyes"]; then
  AC_DEFINE(DEBUG, 1, [Define to enable debug build])
  CXXFLAGS="${CXXFLAGS} -g"
  pkg_modules="StopClock >= 0.1"
  AC_MSG_RESULT(yes)
  # check for pkg-config stuff
  PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
  AC_SUBST(PACKAGE_CFLAGS)
  AC_SUBST(PACKAGE_LIBS)  
else
  AC_MSG_RESULT(no)
fi

dnl Check for headers

dnl Check for functions

dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DIRS in config.h
packagedatadir=share/${PACKAGE}
packagedocdir=doc/${PACKAGE}
packagepixmapsdir=share/pixmaps/${PACKAGE}

dnl Subst PACKAGE_DATA_DIR.
NO_PREFIX_PACKAGE_DATA_DIR="${packagedatadir}"
AC_SUBST(NO_PREFIX_PACKAGE_DATA_DIR)
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_DOC_DIR.
NO_PREFIX_PACKAGE_DOC_DIR="${packagedocdir}"
AC_SUBST(NO_PREFIX_PACKAGE_DOC_DIR)
PACKAGE_DOC_DIR="${packageprefix}/${packagedocdir}"
AC_SUBST(PACKAGE_DOC_DIR)

dnl Subst PACKAGE_PIXMAPS_DIR.
NO_PREFIX_PACKAGE_PIXMAPS_DIR="${packagepixmapsdir}"
AC_SUBST(NO_PREFIX_PACKAGE_PIXMAPS_DIR)
PACKAGE_PIXMAPS_DIR="${packageprefix}/${packagepixmapsdir}"
AC_SUBST(PACKAGE_PIXMAPS_DIR)

dnl Subst PACKAGE_SOURCE_DIR
PACKAGE_SOURCE_DIR="${packagesrcdir}"
AC_SUBST(PACKAGE_SOURCE_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}","")
AC_DEFINE_UNQUOTED(SYSTEM_DATA_DIR, "${packageprefix}/${systemdatadir}","")
AC_DEFINE_UNQUOTED(PACKAGE_DOC_DIR, "${packageprefix}/${packagedocdir}","")
AC_DEFINE_UNQUOTED(PACKAGE_PIXMAPS_DIR, "${packageprefix}/${packagepixmapsdir}","")
AC_DEFINE_UNQUOTED(SYSTEM_PIXMAPS_DIR, "${packageprefix}/${systempixmapsdir}","")
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}","")

AC_OUTPUT([
Makefile
include/Makefile
include/animorph/Makefile
src/Makefile
animorph.pc
animorph-uninstalled.pc
animorph-env.sh
animorph.DevPackage
])
