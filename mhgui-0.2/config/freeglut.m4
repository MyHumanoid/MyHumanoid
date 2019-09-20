AC_DEFUN([AX_CHECK_FREEGLUT],
[AC_REQUIRE([AX_CHECK_GLU])dnl
AC_REQUIRE([AC_PATH_XTRA])dnl

#
# If X is present, assume FREEGLUT depends on it.
#
if test "X${no_x}" != "Xyes"; then
  FREEGLUT_LIBS="${X_PRE_LIBS} -lXmu -lXi ${X_EXTRA_LIBS} ${FREEGLUT_LIBS}"
fi

AC_LANG_PUSH(C)

ax_save_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${FREEGLUT_CFLAGS} ${CPPFLAGS}"

AC_CACHE_CHECK([for FreeGLUT library], [ax_cv_check_freeglut_libfreeglut],
[ax_cv_check_freeglut_libfreeglut="no"
ax_save_LIBS="${LIBS}"
LIBS=""
ax_check_libs="-lfreeglut,-lfreeglut -lwinmm -lgdi32"
oldIFS=$IFS
IFS=,
for ax_lib in ${ax_check_libs}; do
  IFS=$oldIFS
  if test X$ax_compiler_ms = Xyes; then
    ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
  else
    ax_try_lib="${ax_lib}"
  fi
  LIBS="${ax_try_lib} ${FREEGLUT_LIBS} ${ax_save_LIBS}"
  AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include <GL/freeglut.h>]],
                   [[glutMainLoop()]])],
  [ax_cv_check_freeglut_libfreeglut="${ax_try_lib}"; break])

done
LIBS=${ax_save_LIBS}
])
CPPFLAGS="${ax_save_CPPFLAGS}"
AC_LANG_POP(C)

if test "X${ax_cv_check_freeglut_libfreeglut}" = "Xno"; then
  no_freeglut="yes"
  FREEGLUT_CFLAGS=""
  FREEGLUT_LIBS=""
else
  FREEGLUT_LIBS="${ax_cv_check_freeglut_libfreeglut} ${FREEGLUT_LIBS}"
fi

AC_SUBST([FREEGLUT_CFLAGS])
AC_SUBST([FREEGLUT_LIBS])
])dnl
