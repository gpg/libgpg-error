# Makefile.am for libgpg-error.
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

# We distribute the generated sources err-sources.h and err-codes.h,
# because they are needed to build the po directory, and they don't
# depend on the configuration anyway. 
EXTRA_DIST = mkstrtable.awk err-sources.h.in err-codes.h.in \
	mkerrnos.awk errnos.in mkerrcodes1.awk mkerrcodes2.awk \
	err-sources.h err-codes.h gpg-error-config.in gpg-error.m4
BUILT_SOURCES = err-sources.h err-codes.h code-to-errno.h code-from-errno.h
CLEANFILES = err-sources.h err-codes.h code-to-errno.h code-from-errno.h

bin_SCRIPTS = gpg-error-config
m4datadir = $(datadir)/aclocal
m4data_DATA = gpg-error.m4

include_HEADERS = gpg-error.h

lib_LTLIBRARIES = libgpg-error.la

libgpg_error_la_LDFLAGS = -version-info \
  @LIBGPG_ERROR_LT_CURRENT@:@LIBGPG_ERROR_LT_REVISION@:@LIBGPG_ERROR_LT_AGE@

libgpg_error_la_SOURCES = gpg-error.h gettext.h \
	strsource.c strerror.c code-to-errno.c code-from-errno.c

libgpg_error_la_LIBADD = @LTLIBINTL@

err-sources.h: Makefile mkstrtable.awk err-sources.h.in
	$(AWK) -f $(srcdir)/mkstrtable.awk $(srcdir)/err-sources.h.in >$@

err-codes.h: Makefile mkstrtable.awk err-codes.h.in
	$(AWK) -f $(srcdir)/mkstrtable.awk $(srcdir)/err-codes.h.in >$@

code-to-errno.h: Makefile mkerrnos.awk errnos.in
	$(AWK) -f $(srcdir)/mkerrnos.awk $(srcdir)/errnos.in >$@

code-from-errno.h: Makefile mkerrcodes1.awk errnos.in
	$(AWK) -f $(srcdir)/mkerrcodes1.awk $(srcdir)/errnos.in \
	| $(CPP) - | grep GPG_ERR_ | sort -n \
	| $(AWK) -f $(srcdir)/mkerrcodes2.awk >$@
