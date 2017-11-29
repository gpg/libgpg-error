# TinyScheme-based test driver.
#
# Copyright (C) 2016 g10 Code GmbH
#
# This file is part of Libgpg-error.
#
# Libgpg-error is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# Libgpg-error is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, see <https://www.gnu.org/licenses/>.

EXTRA_DIST = \
	LICENSE.TinySCHEME \
	Manual.txt \
	ffi.scm \
	init.scm \
	lib.scm \
	makefile.scm \
	repl.scm \
	t-child.scm \
	xml.scm \
	tests.scm \
	gnupg.scm \
	time.scm

AM_CFLAGS =

CLEANFILES =

bin_PROGRAMS = gpgscm
noinst_PROGRAMS = t-child

gpgscm_CFLAGS = -imacros scheme-config.h
gpgscm_SOURCES = main.c private.h ffi.c ffi.h ffi-private.h \
	scheme-config.h scheme.c scheme.h scheme-private.h \
	opdefines.h small-integers.h
gpgscm_LDADD = $(LDADD) $(LIBREADLINE) ../src/libgpg-error.la $(LTLIBINTL)

t_child_SOURCES = t-child.c

# Make sure that all libs are build before we use them.  This is
# important for parallel builds (i.e. make -j2).
$(PROGRAMS): ../src/libgpg-error.la

# Temporary disabled (2017-11-29)
#check-local: gpgscm$(EXEEXT) t-child$(EXEEXT)
#	EXEEXT=$(EXEEXT) GPGSCM_PATH=$(srcdir) \
#		./gpgscm$(EXEEXT) $(srcdir)/t-child.scm
