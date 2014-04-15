# configure.ac for libgpg-error
# Copyright (C) 2003, 2004, 2006, 2010, 2013 g10 Code GmbH
#
# This file is part of libgpg-error.
#
# libgpg-error is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# libgpg-error is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.

# (Process this file with autoconf to produce a configure script.)
# The following lines are used by ./autogen.sh.
AC_PREREQ(2.59)
min_automake_version="1.11"

# To build a release you need to create a tag with the version number
# (git tag -s libgpg-error-n.m) and run "./autogen.sh --force".
# Please bump the version number immediately after the release, do
# another commit, and a push so that the git magic is able to work.
# See below for the LT versions.
m4_define([mym4_version_major], [1])
m4_define([mym4_version_minor], [13])

# Below is m4 magic to extract and compute the revision number, the
# decimalized short revision number, a beta version string, and a flag
# indicating a development version (mym4_isgit). Note that the m4
# processing is done by autoconf and not during the configure run.
m4_define([mym4_version], [mym4_version_major.mym4_version_minor])
m4_define([mym4_revision],
          m4_esyscmd([git rev-parse --short HEAD | tr -d '\n\r']))
m4_define([mym4_revision_dec],
          m4_esyscmd_s([echo $((0x$(echo ]mym4_revision[|head -c 4)))]))
m4_define([mym4_betastring],
       m4_esyscmd_s([git describe --match 'libgpg-error-[0-9].*[0-9]' --long|\
                        awk -F- '$4!=0{print"-beta"$4}']))
m4_define([mym4_isgit],m4_if(mym4_betastring,[],[no],[yes]))
m4_define([mym4_full_version],[mym4_version[]mym4_betastring])

AC_INIT([libgpg-error],[mym4_full_version],[http://bugs.gnupg.org])
# LT Version numbers, remember to change them just *before* a release.
#   (Code changed:			REVISION++)
#   (Interfaces added/removed/changed:	CURRENT++, REVISION=0)
#   (Interfaces added:			AGE++)
#   (Interfaces removed:		AGE=0)
# Note that added error codes don't constitute an interface change.
LIBGPG_ERROR_LT_CURRENT=11
LIBGPG_ERROR_LT_AGE=11
LIBGPG_ERROR_LT_REVISION=0
################################################

AC_SUBST(LIBGPG_ERROR_LT_CURRENT)
AC_SUBST(LIBGPG_ERROR_LT_AGE)
AC_SUBST(LIBGPG_ERROR_LT_REVISION)

VERSION_NUMBER=m4_esyscmd(printf "0x%02x%02x00" mym4_version_major \
                          mym4_version_minor)
AC_SUBST(VERSION_NUMBER)

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/err-sources.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES

# We need to know about the host architecture to avoid spurious
# warnings.
AC_CANONICAL_HOST
AB_INIT

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_AWK
AC_CHECK_TOOL(AR, ar, :)
AC_GNU_SOURCE

# Set some internal variables depending on the platform for later use.
have_w32_system=no
have_w64_system=no
have_w32ce_system=no
case "${host}" in
    x86_64-*mingw32*)
        have_w32_system=yes
        have_w64_system=yes
        ;;
    *-mingw32ce*)
        have_w32_system=yes
        have_w32ce_system=yes
        ;;
    *-mingw32*)
        have_w32_system=yes
        ;;
    *)
       ;;
esac


if test "$have_w32_system" != yes; then
  gl_THREADLIB_EARLY
fi


LT_PREREQ([2.2.6])
LT_INIT([win32-dll disable-static])
LT_LANG([Windows Resource])


# We need to compile and run a program on the build machine.
dnl The AC_PROG_CC_FOR_BUILD macro in the AC archive is broken for
dnl autoconf 2.57.
dnl AC_PROG_CC_FOR_BUILD
AC_MSG_CHECKING(for cc for build)
if test "$cross_compiling" = "yes"; then
  CC_FOR_BUILD="${CC_FOR_BUILD-cc}"
else
  CC_FOR_BUILD="${CC_FOR_BUILD-$CC}"
fi
AC_MSG_RESULT($CC_FOR_BUILD)
AC_ARG_VAR(CC_FOR_BUILD,[build system C compiler])


AH_BOTTOM([
/* Force using of NLS for W32 even if no libintl has been found.  This is
   okay because we have our own gettext implementation for W32.  */
#if defined(HAVE_W32_SYSTEM) && !defined(ENABLE_NLS)
#define ENABLE_NLS 1
#endif
/* For building we need to define these macro.  */
#define GPG_ERR_ENABLE_GETTEXT_MACROS 1
#define GPG_ERR_ENABLE_ERRNO_MACROS 1
])


# Note, that autogen.sh greps for the next line.
AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT([external])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h locale.h])
AC_FUNC_STRERROR_R
case "${host_os}" in
     solaris*)
     # All versions of Solaris from 2.4 have a thread-safe strerror().
     # Since Solaris 10, in addition strerror_r() exists.
     ;;
     *)
     AC_CHECK_FUNC([strerror_r], [],
AC_MSG_WARN([[Without strerror_r, gpg_strerror_r might not be thread-safe]]))
     ;;
esac

AC_CHECK_FUNCS([flockfile])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST


# Check for thread library.
#
# Windows has always thread support; thus we don't bother to test for
# it as it may lead to false results when cross building.
if test "$have_w32_system" = yes; then
  AC_DEFINE([USE_WINDOWS_THREADS], [1])
  LIBTHREAD=
  LTLIBTHREAD=
  LIBMULTITHREAD=
  LTLIBMULTITHREAD=
  THREADLIB_CPPFLAGS=""
  AC_SUBST([LIBTHREAD])
  AC_SUBST([LTLIBTHREAD])
  AC_SUBST([LIBMULTITHREAD])
  AC_SUBST([LTLIBMULTITHREAD])
else
  gl_LOCK
  if test "$gl_threads_api" = posix; then
    AC_CHECK_SIZEOF(pthread_mutex_t,,[AC_INCLUDES_DEFAULT
                                      #include <pthread.h>])
  fi
fi


# Substitution used for gpg-error-config
GPG_ERROR_CONFIG_LIBS="-lgpg-error"
if test "x$LIBTHREAD" != x; then
  GPG_ERROR_CONFIG_LIBS="${GPG_ERROR_CONFIG_LIBS} ${LIBTHREAD}"
fi
if test "x$LIBMULTITHREAD" != x; then
  GPG_ERROR_CONFIG_MT_LIBS="${LIBMULTITHREAD}"
else
  GPG_ERROR_CONFIG_MT_LIBS=""
fi
GPG_ERROR_CONFIG_CFLAGS=""
if test "x$THREADLIB_CPPFLAGS" != x; then
  GPG_ERROR_CONFIG_MT_CFLAGS="${THREADLIB_CPPFLAGS}"
else
  GPG_ERROR_CONFIG_MT_CFLAGS=""
fi
GPG_ERROR_CONFIG_ISUBDIRAFTER=""
GPG_ERROR_CONFIG_HOST="$host"
AC_SUBST(GPG_ERROR_CONFIG_LIBS)
AC_SUBST(GPG_ERROR_CONFIG_CFLAGS)
AC_SUBST(GPG_ERROR_CONFIG_MT_LIBS)
AC_SUBST(GPG_ERROR_CONFIG_MT_CFLAGS)
AC_SUBST(GPG_ERROR_CONFIG_ISUBDIRAFTER)
AC_SUBST(GPG_ERROR_CONFIG_HOST)


# Special defines for certain platforms
if test "$have_w32_system" = yes; then
    AC_DEFINE(HAVE_W32_SYSTEM,1,[Defined if we run on a W32 API based system])
    if test "$have_w64_system" = yes; then
      AC_DEFINE(HAVE_W64_SYSTEM,1,[Defined if we run on 64 bit W32 API system])
    fi
    if test "$have_w32ce_system" = yes; then
      AC_DEFINE(HAVE_W32CE_SYSTEM,1,[Defined if we run on WindowsCE])
      GPG_ERROR_CONFIG_ISUBDIRAFTER="gpg-extra"
    fi
fi
AM_CONDITIONAL(HAVE_W32_SYSTEM, test "$have_w32_system" = yes)
AM_CONDITIONAL(HAVE_W64_SYSTEM, test "$have_w64_system" = yes)
AM_CONDITIONAL(HAVE_W32CE_SYSTEM, test "$have_w32ce_system" = yes)
AM_CONDITIONAL(CROSS_COMPILING, test x$cross_compiling = xyes)

AC_DEFINE_UNQUOTED(HOST_TRIPLET_STRING, "$host", [The host triplet])


#
# Provide information about the build.
#
BUILD_REVISION="mym4_revision"
AC_SUBST(BUILD_REVISION)
AC_DEFINE_UNQUOTED(BUILD_REVISION, "$BUILD_REVISION",
                   [GIT commit id revision used to build this package])

changequote(,)dnl
BUILD_FILEVERSION=`echo "$PACKAGE_VERSION"|sed 's/\([0-9.]*\).*/\1./;s/\./,/g'`
changequote([,])dnl
BUILD_FILEVERSION="${BUILD_FILEVERSION}0,mym4_revision_dec"
AC_SUBST(BUILD_FILEVERSION)

BUILD_TIMESTAMP=`date -u +%Y-%m-%dT%H:%M+0000 2>/dev/null || date`
AC_SUBST(BUILD_TIMESTAMP)
AC_DEFINE_UNQUOTED(BUILD_TIMESTAMP, "$BUILD_TIMESTAMP",
                   [The time this package was configured for a build])



AC_ARG_ENABLE(languages,
[  --disable-languages     do not build support for other languages than C])
AM_CONDITIONAL([LANGUAGES_SOME], [test "x$enable_languages" != xno])


# Substitution
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([po/Makefile.in m4/Makefile])
AC_CONFIG_FILES([src/Makefile tests/Makefile])
AC_CONFIG_FILES([lang/Makefile lang/cl/Makefile lang/cl/gpg-error.asd])
AC_CONFIG_FILES([src/versioninfo.rc])
AC_CONFIG_FILES([src/gpg-error-config], [chmod +x src/gpg-error-config])

AC_OUTPUT

echo "
        $PACKAGE_NAME-$PACKAGE_VERSION prepared for make

        Revision: mym4_revision  (mym4_revision_dec)
        Platform: $host

"
