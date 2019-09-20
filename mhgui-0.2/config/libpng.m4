AC_DEFUN([AX_CHECK_PNG],
[AC_REQUIRE([AC_PATH_X])dnl

  AC_LANG_PUSH(C)

  AX_LANG_COMPILER_MS
  if test X$ax_compiler_ms = Xno; then
    GL_CFLAGS="${PTHREAD_CFLAGS}"
    GL_LIBS="${PTHREAD_LIBS} -lm"
  fi

  AC_CHECK_HEADERS([windows.h])

  AC_CACHE_CHECK([for libpng library], [ax_cv_check_libpng],
  [ax_cv_check_libpng="no"
  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${PNG_CFLAGS} ${CPPFLAGS}"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-lpng"
  for ax_lib in ${ax_check_libs}; do
    if test X$ax_compiler_ms = Xyes; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${PNG_LIBS} ${ax_save_LIBS}"
    AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include <png.h>]],
                     [[png_check_sig(NULL,0)]])],
    [ax_cv_check_libpng="${ax_try_lib}"; break])
  done
  LIBS=${ax_save_LIBS}
  CPPFLAGS=${ax_save_CPPFLAGS}])

  if test "X${ax_cv_check_libpng}" = "Xno"; then
    no_gl="yes"
    PNG_CFLAGS=""
    PNG_LIBS=""
  else
    PNG_LIBS="${ax_cv_check_libpng} ${PNG_LIBS}"
  fi
  AC_LANG_POP(C)

AC_SUBST([PNG_CFLAGS])
AC_SUBST([PNG_LIBS])
])dnl

