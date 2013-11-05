# Configure paths for SDLmm
# David Hedbor, 2000-07-22
# stolen from SDL 
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_SDLMM([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for SDLmm, and define SDLMM_CXXFLAGS and SDLMM_LIBS
dnl
AC_DEFUN(AM_PATH_SDLMM,
[dnl 
dnl Get the cflags and libraries from the ismap-config script
dnl
AC_ARG_WITH(sdlmm-prefix,[  --with-sdlmm-prefix=PFX   Prefix where SDLmm is installed (optional)],
            sdlmm_prefix="$withval", sdlmm_prefix="")
AC_ARG_WITH(sdlmm-exec-prefix,[  --with-sdlmm-exec-prefix=PFX Exec prefix where SDLmm is installed (optional)],
            sdlmm_exec_prefix="$withval", sdlmm_exec_prefix="")
AC_ARG_ENABLE(sdlmmtest, [  --disable-sdlmmtest       Do not try to compile and run a test SDLmm program],
		    , enable_sdlmmtest=yes)

  if test x$sdlmm_exec_prefix != x ; then
     sdlmm_args="$sdlmm_args --exec-prefix=$sdlmm_exec_prefix"
     if test x${SDLMM_CONFIG+set} != xset ; then
        SDLMM_CONFIG=$sdlmm_exec_prefix/bin/sdlmm-config
     fi
  fi
  if test x$sdlmm_prefix != x ; then
     sdlmm_args="$sdlmm_args --prefix=$sdlmm_prefix"
     if test x${SDLMM_CONFIG+set} != xset ; then
        SDLMM_CONFIG=$sdlmm_prefix/bin/sdlmm-config
     fi
  fi

  AC_PATH_PROG(SDLMM_CONFIG, sdlmm-config, no)
  min_sdlmm_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING(for SDLmm version >= $min_sdlmm_version)
  no_sdlmm=""
  if test "$SDLMM_CONFIG" = "no" ; then
    no_sdlmm=yes
  else
    SDLMM_CXXFLAGS=`$SDLMM_CONFIG $sdlmmconf_args --cflags`
    SDLMM_LIBS=`$SDLMM_CONFIG $sdlmmconf_args --libs`

    sdlmm_major_version=`$SDLMM_CONFIG $sdlmm_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sdlmm_minor_version=`$SDLMM_CONFIG $sdlmm_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sdlmm_micro_version=`$SDLMM_CONFIG $sdlmm_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_sdlmmtest" = "xyes" ; then
      ac_save_CXXFLAGS="$CXXFLAGS"
      ac_save_LIBS="$LIBS"
      CXXFLAGS="$CXXFLAGS $SDLMM_CXXFLAGS"
      LIBS="$LIBS $SDLMM_LIBS"
dnl
dnl Now check if the installed SDLmm is sufficiently new. (Also sanity
dnl checks the results of sdlmm-config to some extent
dnl
      rm -f conf.sdlmmtest
      AC_TRY_RUN([
#include <cstdio>
#include <cstring>
#include "SDLmm/sdlmm.h"
char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((std::strlen (str) + 1) * sizeof(char));
      std::strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.sdlmmtest");
  */
  { FILE *fp = fopen("conf.sdlmmtest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_sdlmm_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_sdlmm_version");
     return 1;
   }

   if (($sdlmm_major_version > major) ||
      (($sdlmm_major_version == major) && ($sdlmm_minor_version > minor)) ||
      (($sdlmm_major_version == major) && ($sdlmm_minor_version == minor) && ($sdlmm_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'sdlmm-config --version' returned %d.%d.%d, but the minimum version\n", $sdlmm_major_version, $sdlmm_minor_version, $sdlmm_micro_version);
      printf("*** of SDLmm required is %d.%d.%d. If sdlmm-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If sdlmm-config was wrong, set the environment variable SDLMM_CONFIG\n");
      printf("*** to point to the correct copy of sdlmm-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_sdlmm=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CXXFLAGS="$ac_save_CXXFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_sdlmm" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$SDLMM_CONFIG" = "no" ; then
       echo "*** The sdlmm-config script installed by SDLmm could not be found"
       echo "*** If SDLmm was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the SDLMM_CONFIG environment variable to the"
       echo "*** full path to sdlmm-config."
     else
       if test -f conf.sdlmmtest ; then
        :
       else
          echo "*** Could not run SDLmm test program, checking why..."
          CXXFLAGS="$CXXFLAGS $SDLMM_CXXFLAGS"
          LIBS="$LIBS $SDLMM_LIBS"
          AC_TRY_LINK([
#include <cstdio>
#include "sdlmm.h"
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding SDLmm or finding the wrong"
          echo "*** version of SDLmm. If it is not finding SDLmm, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means SDLmm was incorrectly installed"
          echo "*** or that you have moved SDLmm since it was installed. In the latter case, you"
          echo "*** may want to edit the sdlmm-config script: $SDLMM_CONFIG" ])
          CXXFLAGS="$ac_save_CXXFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     SDLMM_CXXFLAGS=""
     SDLMM_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(SDLMM_CXXFLAGS)
  AC_SUBST(SDLMM_LIBS)
  rm -f conf.sdlmmtest
])

# From Braden McDaniel
# http://endoframe.com

dnl @synopsis AX_CHECK_GL
dnl
dnl Check for an OpenGL implementation.  If GL is found, the required compiler
dnl and linker flags are included in the output variables "GL_CFLAGS" and
dnl "GL_LIBS", respectively.  
dnl 
dnl Modifications for OS X by Alex Holkner
dnl
dnl @version 1.8
dnl @author Braden McDaniel <address@bogus.example.com>
dnl
AC_DEFUN([AX_CHECK_GL],
[AC_REQUIRE([AC_PATH_X])dnl
AC_REQUIRE([ACX_PTHREAD])dnl

  # Find header first
  AC_CHECK_HEADER(GL/gl.h, OPENGL_HEADER="<GL/gl.h>")
  AC_CHECK_HEADER(OpenGL/gl.h, OPENGL_HEADER="<OpenGL/gl.h>")
  AC_DEFINE_UNQUOTED([OPENGL_HEADER], $OPENGL_HEADER, 
    [The OpenGL header file for this system])

  AC_LANG_PUSH(C)

  AX_LANG_COMPILER_MS
  if test X$ax_compiler_ms = Xno; then
    GL_CFLAGS="${PTHREAD_CFLAGS}"
    GL_LIBS="${PTHREAD_LIBS} -lm"
  fi

  #
  # Use x_includes and x_libraries if they have been set (presumably by
  # AC_PATH_X).
  #
  if test "X$no_x" != "Xyes"; then
    if test -n "$x_includes"; then
      GL_CFLAGS="-I${x_includes} ${GL_CFLAGS}"
    fi
    if test -n "$x_libraries"; then
      GL_LIBS="-L${x_libraries} -lX11 ${GL_LIBS}"
    fi
  fi

  AC_CHECK_HEADERS([windows.h])

  AC_CACHE_CHECK([for OpenGL library], [ax_cv_check_gl_libgl],
  [ax_cv_check_gl_libgl="no"
  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${GL_CFLAGS} ${CPPFLAGS}"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-framework_OpenGL -lopengl32 -lGL"
  for ax_lib in ${ax_check_libs}; do
    ax_lib=`echo $ax_lib | sed -e 's/-framework_/-framework /'`
    if test X$ax_compiler_ms = Xyes; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${GL_LIBS} ${ax_save_LIBS}"
    AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include ${OPENGL_HEADER}]],
                     [[glBegin(0)]])],
    [ax_cv_check_gl_libgl="${ax_try_lib}"; break])
  done
  LIBS=${ax_save_LIBS}
  CPPFLAGS=${ax_save_CPPFLAGS}])

  if test "X${ax_cv_check_gl_libgl}" = "Xno"; then
    no_gl="yes"
    GL_CFLAGS=""
    GL_LIBS=""
  else
    GL_LIBS="${ax_cv_check_gl_libgl} ${GL_LIBS}"
  fi
  AC_LANG_POP(C)

AC_SUBST([GL_CFLAGS])
AC_SUBST([GL_LIBS])
])dnl


dnl @synopsis AX_CHECK_GLU
dnl
dnl Check for GLU.  If GLU is found, the required preprocessor and linker flags
dnl are included in the output variables "GLU_CFLAGS" and "GLU_LIBS",
dnl respectively.  
dnl
dnl @version 1.2
dnl @author Braden McDaniel <address@bogus.example.com>
dnl
AC_DEFUN([AX_CHECK_GLU],
[AC_REQUIRE([AX_CHECK_GL])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
  
  # Find header first
  AC_CHECK_HEADER(GL/glu.h, GLU_HEADER="<GL/glu.h>")
  AC_CHECK_HEADER(OpenGL/glu.h, GLU_HEADER="<OpenGL/glu.h>")
  AC_DEFINE_UNQUOTED([GLU_HEADER], $GLU_HEADER, 
    [The GLU header file for this system])
  
  
  GLU_CFLAGS="${GL_CFLAGS}"

  AC_CACHE_CHECK([for OpenGL Utility library], [ax_cv_check_glu_libglu],
  [ax_cv_check_glu_libglu="no"
  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${GL_CFLAGS} ${CPPFLAGS}"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-framework_OpenGL -lglu32 -lGLU"
  for ax_lib in ${ax_check_libs}; do
    ax_lib=`echo $ax_lib | sed -e 's/-framework_/-framework /'`
    if test X$ax_compiler_ms = Xyes; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${GL_LIBS} ${ax_save_LIBS}"
    #
    # libGLU typically links with libstdc++ on POSIX platforms. However,
    # setting the language to C++ means that test program source is named
    # "conftest.cc"; and Microsoft cl doesn't know what to do with such a
    # file.
    #
    AC_LANG_PUSH([C++])
    if test X$ax_compiler_ms = Xyes; then
      AC_LANG_PUSH([C])
    fi
    AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include ${GLU_HEADER}]],
                     [[gluBeginCurve(0)]])],
    [ax_cv_check_glu_libglu="${ax_try_lib}"; break])
    if test X$ax_compiler_ms = Xyes; then
      AC_LANG_POP([C])
    fi
    AC_LANG_POP([C++])
  done
  LIBS=${ax_save_LIBS}
  CPPFLAGS=${ax_save_CPPFLAGS}])
  if test "X${ax_cv_check_glu_libglu}" = "Xno"; then
    no_glu="yes"
    GLU_CFLAGS=""
    GLU_LIBS=""
  else
    GLU_LIBS="${ax_cv_check_glu_libglu} ${GL_LIBS}"
  fi

AC_SUBST([GLU_CFLAGS])
AC_SUBST([GLU_LIBS])
])



dnl @synopsis AX_CHECK_GLUT
dnl
dnl Check for GLUT.  If GLUT is found, the required compiler and linker flags
dnl are included in the output variables "GLUT_CFLAGS" and "GLUT_LIBS",
dnl respectively. If GLUT is not found, "no_glut" is set to "yes".
dnl
dnl @version 1.8
dnl @author Braden McDaniel <address@bogus.example.com>
dnl
AC_DEFUN([AX_CHECK_GLUT],
[AC_REQUIRE([AX_CHECK_GLU])dnl
AC_REQUIRE([AC_PATH_XTRA])dnl

  # Find header first
  AC_CHECK_HEADER(GL/glut.h, GLUT_HEADER="<GL/glut.h>")
  AC_CHECK_HEADER(GLUT/glut.h, GLUT_HEADER="<GLUT/glut.h>")
  AC_DEFINE_UNQUOTED([GLUT_HEADER], $GLUT_HEADER, 
    [The GLUT header file for this system])

  GLUT_CFLAGS=${GLU_CFLAGS}
  GLUT_LIBS=${GLU_LIBS}

  #
  # If X is present, assume GLUT depends on it.
  #
  if test "X${no_x}" != "Xyes"; then
    GLUT_LIBS="${X_PRE_LIBS} -lXmu -lXi ${X_EXTRA_LIBS} ${GLUT_LIBS}"
  fi

  AC_LANG_PUSH(C)

  ax_save_CPPFLAGS="${CPPFLAGS}"
  CPPFLAGS="${GLUT_CFLAGS} ${CPPFLAGS}"

  AC_CACHE_CHECK([for GLUT library], [ax_cv_check_glut_libglut],
  [ax_cv_check_glut_libglut="no"
  ax_save_LIBS="${LIBS}"
  LIBS=""
  ax_check_libs="-framework_GLUT_-framework_Cocoa -lglut32 -lglut"
  for ax_lib in ${ax_check_libs}; do
    ax_lib=`echo $ax_lib | sed -e 's/_/ /g'`
    if test X$ax_compiler_ms = Xyes; then
      ax_try_lib=`echo $ax_lib | sed -e 's/^-l//' -e 's/$/.lib/'`
    else
      ax_try_lib="${ax_lib}"
    fi
    LIBS="${ax_try_lib} ${GLUT_LIBS} ${ax_save_LIBS}"
    AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[
# if HAVE_WINDOWS_H && defined(_WIN32)
#   include <windows.h>
# endif
# include ${GLUT_HEADER}]],
                     [[glutMainLoop()]])],
    [ax_cv_check_glut_libglut="${ax_try_lib}"; break])

  done
  LIBS=${ax_save_LIBS}
  ])
  CPPFLAGS="${ax_save_CPPFLAGS}"
  AC_LANG_POP(C)

  if test "X${ax_cv_check_glut_libglut}" = "Xno"; then
    no_glut="yes"
    GLUT_CFLAGS=""
    GLUT_LIBS=""
  else
    GLUT_LIBS="${ax_cv_check_glut_libglut} ${GLUT_LIBS}"
  fi

AC_SUBST([GLUT_CFLAGS])
AC_SUBST([GLUT_LIBS])
])dnl
dnl @synopsis AX_LANG_COMPILER_MS
dnl
dnl Check whether the compiler for the current language is Microsoft.
dnl
dnl This macro is modeled after _AC_LANG_COMPILER_GNU in the GNU Autoconf
dnl implementation.
dnl
dnl @version 1.0
dnl @author Braden McDaniel <address@bogus.example.com>
dnl
AC_DEFUN([AX_LANG_COMPILER_MS],
[AC_CACHE_CHECK([whether we are using the Microsoft _AC_LANG compiler],
                [ax_cv_[]_AC_LANG_ABBREV[]_compiler_ms],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([], [[#ifndef _MSC_VER
       choke me
#endif
]])],
                   [ax_compiler_ms=yes],
                   [ax_compiler_ms=no])
ax_cv_[]_AC_LANG_ABBREV[]_compiler_ms=$ax_compiler_ms
])])

AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all, and "pthread-config"
# which is a program returning the flags for the Pth emulation library.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt pthread-config"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++
# pthread-config: use pthread-config program (for GNU Pth library)

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                pthread-config)
                AC_CHECK_PROG(acx_pthread_config, pthread-config, yes, no)
                if test x"$acx_pthread_config" = xno; then continue; fi
                PTHREAD_CFLAGS="`pthread-config --cflags`"
                PTHREAD_LIBS="`pthread-config --ldflags` `pthread-config --libs`"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: JOINABLE attribute is called UNDETACHED.
        AC_MSG_CHECKING([for joinable pthread attribute])
        attr_name=unknown
        for attr in PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED; do
            AC_TRY_LINK([#include <pthread.h>], [int attr=$attr;],
                        [attr_name=$attr; break])
        done
        AC_MSG_RESULT($attr_name)
        if test "$attr_name" != PTHREAD_CREATE_JOINABLE; then
            AC_DEFINE_UNQUOTED(PTHREAD_CREATE_JOINABLE, $attr_name,
                               [Define to necessary symbol if this constant
                                uses a non-standard name on your system.])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
            *-aix* | *-freebsd* | *-darwin*) flag="-D_THREAD_SAFE";;
            *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
            PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD

