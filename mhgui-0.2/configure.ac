dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta - will *NOT* be overwritten

AC_INIT(MHGUI Library, 0.2, linux@brachttal.net)
AM_INIT_AUTOMAKE(mhgui, 0.2)
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

dnl Checks for libraries
if [ ! test "x$enable_debug" != "xyes"]; then
  AC_DEFINE(DEBUG, 1, [Define to enable debug build])
  CXXFLAGS="${CXXFLAGS} -g"
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

if test "X$build_os" == "Xcygwin"; then
  echo "We run on Cygwin: Disabling linking with X libs,"
  echo " because we only support Cygwin using the OpenGL bindings from"
  echo " the package 'opengl', which does not depend on X."
  echo " FreeGLUT in Cygwin would use only slow Mesa Software Emulation."
  with_x=no
fi

dnl Check for OpenGL libraries
AX_CHECK_GL
if test "X${ax_cv_check_gl_libgl}" = "Xno"; then
  echo 
  echo "*****************************************"
  echo "* Abort!                                *"
  echo "* You need support for OpenGL in mhgui! *"
  echo "*****************************************"
  exit
else
  AX_CHECK_GLU
  if test "X${ax_cv_check_glu_libglu}" = "Xno"; then
    echo 
    echo "**************************************"
    echo "* Abort!                             *"
    echo "* You need support for GLU in mhgui! *"
    echo "**************************************"
    exit
  else
    AX_CHECK_FREEGLUT
    if test "X${ax_cv_check_freeglut_libfreeglut}" != "Xno"; then
      AC_DEFINE(USE_FREEGLUT, 1,[Define to enable FreeGLUT])
    else
      AX_CHECK_GLUT
      if test "X${ax_cv_check_glut_libglut}" != "Xno"; then
        AC_DEFINE(USE_GLUT, 1,[Define to enable GLUT])
      else
        echo 
        echo "****************************************************"
        echo "* Abort!                                           *"
        echo "* You need support for FREEGLUT or GLUT in mhgui!  *"
	echo "* Hint: Perhaps you need some X11 development libs *"
	echo "*       like Xmu. Install them if needed.          *"
        echo "****************************************************"
        exit
      fi
    fi
  fi
fi

dnl Check for libpng
AX_CHECK_PNG
if test "X${ax_cv_check_libpng}" = "Xno"; then
  echo 
  echo "************************************************"
  echo "* Abort!                                       *"
  echo "* You need support for libpng in mhgui!        *"
  echo "* Get it from e.g. from http://www.libpng.org/ *"
  echo "************************************************"
  exit
fi

dnl Check for programs

dnl Check for pkg-config stuff
misc_modules="animorph >= 0.3"
pkg_modules="$misc_modules"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

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
include/mhgui/Makefile
src/Makefile
mhgui.pc
mhgui-uninstalled.pc
mhgui-env.sh
mhgui.DevPackage 
])
