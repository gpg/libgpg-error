# configure.ac for libgpg-error
# Copyright (C) 2003 g10 Code GmbH
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
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

# (Process this file with autoconf to produce a configure script.)

# Version number: Remember to change it immediately *after* a release.
AC_INIT(libgpg-error, 0.1, bug-gnupg@gnupg.org)
# LT Version numbers, remember to change them just *before* a release.
#   (Code changed:			REVISION++)
#   (Interfaces added/removed/changed:	CURRENT++, REVISION=0)
#   (Interfaces added:			AGE++)
#   (Interfaces removed:		AGE=0)
LIBGPG_ERROR_LT_CURRENT=0
LIBGPG_ERROR_LT_AGE=0
LIBGPG_ERROR_LT_REVISION=0
AC_SUBST(LIBGPG_ERROR_LT_CURRENT)
AC_SUBST(LIBGPG_ERROR_LT_AGE)
AC_SUBST(LIBGPG_ERROR_LT_REVISION)

AC_PREREQ(2.57)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/err-sources.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_AWK

# Checks for libraries.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.11.5)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Substitution used for gpg-error-config
GPG_ERROR_CONFIG_LIBS="-lgpg-error"
GPG_ERROR_CONFIG_CFLAGS=""
AC_SUBST(GPG_ERROR_CONFIG_LIBS)
AC_SUBST(GPG_ERROR_CONFIG_CFLAGS)
AC_CONFIG_FILES([src/gpg-error-config], [chmod +x src/gpg-error-config])

# Checks for library functions.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([po/Makefile.in m4/Makefile])
AC_CONFIG_FILES([src/Makefile tests/Makefile])
AC_OUTPUT
