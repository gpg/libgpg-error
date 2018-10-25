# gpgrt.m4 - autoconf macro to detect libgpgrt
# Copyright (C) 2002, 2003, 2004, 2011, 2014, 2017, 2018 g10 Code GmbH
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# SPDX-License-Identifier: FSFULLR
#
# Last-changed: 2018-10-25
# Note: This is a kind of duplicate of gpg-error.m4 with uses the
# future name of libgpg-error to prepare for a smooth migration in
# some distant time.

dnl AM_PATH_GPGRT([MINIMUM-VERSION,
dnl               [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]])
dnl
dnl Test for libgpgrt and define GPGRT_CFLAGS, GPGRT_LIBS,
dnl GPGRT_MT_CFLAGS, and GPGRT_MT_LIBS.  The _MT_ variants are
dnl used for programs requiring real multi thread support.
dnl
dnl If a prefix option is not used, the config script is first
dnl searched in $SYSROOT/bin and then along $PATH.  If the used
dnl config script does not match the host specification the script
dnl is added to the gpg_config_script_warn variable.
dnl
AC_DEFUN([AM_PATH_GPGRT],
[ AC_REQUIRE([AC_CANONICAL_HOST])
  gpgrt_config_prefix=""
  dnl --with-libgpg-error-prefix=PFX is the preferred name for this option,
  dnl since that is consistent with how our three siblings use the directory/
  dnl package name in --with-$dir_name-prefix=PFX.
  AC_ARG_WITH(libgpg-error-prefix,
              AC_HELP_STRING([--with-libgpg-error-prefix=PFX],
                             [prefix where GPG Error is installed (optional)]),
              [gpgrt_config_prefix="$withval"])

  dnl Accept --with-gpg-error-prefix and make it work the same as
  dnl --with-libgpg-error-prefix above, for backwards compatibility,
  dnl but do not document this old, inconsistently-named option.
  AC_ARG_WITH(gpg-error-prefix,,
              [gpgrt_config_prefix="$withval"])

  dnl Also accept libgpgrt-prefix
  AC_ARG_WITH(libgpgrt-prefix,
              AC_HELP_STRING([--with-libgpgrt-prefix=PFX],
                             [prefix where GPG Runtime is installed (optional)]),
              [gpgrt_config_prefix="$withval"])

  if test x"${GPGRT_CONFIG}" = x ; then
     if test x"${gpgrt_config_prefix}" != x ; then
        GPGRT_CONFIG="${gpgrt_config_prefix}/bin/gpgrt-config"
     else
       case "${SYSROOT}" in
         /*)
           if test -x "${SYSROOT}/bin/gpgrt-config" ; then
             GPGRT_CONFIG="${SYSROOT}/bin/gpgrt-config"
           fi
           ;;
         '')
           ;;
          *)
           AC_MSG_WARN([Ignoring \$SYSROOT as it is not an absolute path.])
           ;;
       esac
     fi
  fi

  AC_PATH_PROG(GPGRT_CONFIG, gpgrt-config, no)
  min_gpgrt_version=ifelse([$1], ,1.33,$1)
  AC_MSG_CHECKING(for GPG Runtime - version >= $min_gpgrt_version)
  ok=no
  if test "$GPGRT_CONFIG" != "no" \
     && test -f "$GPGRT_CONFIG" ; then
    req_major=`echo $min_gpgrt_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)/\1/'`
    req_minor=`echo $min_gpgrt_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)/\2/'`
    gpgrt_config_version=`CC=$CC $GPGRT_CONFIG --version`
    major=`echo $gpgrt_config_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\).*/\1/'`
    minor=`echo $gpgrt_config_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\).*/\2/'`
    if test "$major" -gt "$req_major"; then
        ok=yes
    else
        if test "$major" -eq "$req_major"; then
            if test "$minor" -ge "$req_minor"; then
               ok=yes
            fi
        fi
    fi
  fi
  if test $ok = yes; then
    GPGRT_CFLAGS=`CC=$CC $GPGRT_CONFIG --cflags`
    GPGRT_LIBS=`CC=$CC $GPGRT_CONFIG --libs`
    GPGRT_MT_CFLAGS=`CC=$CC $GPGRT_CONFIG --variable=mtcflags 2>/dev/null`
    GPGRT_MT_CFLAGS="$GPGRT_CFLAGS${GPGRT_CFLAGS:+ }$GPGRT_MT_CFLAGS"
    GPGRT_MT_LIBS=`CC=$CC $GPGRT_CONFIG --variable=mtlibs 2>/dev/null`
    GPGRT_MT_LIBS="$GPGRT_LIBS${GPGRT_LIBS:+ }$GPGRT_MT_LIBS"
    AC_MSG_RESULT([yes ($gpgrt_config_version)])
    ifelse([$2], , :, [$2])
    gpgrt_config_host=`CC=$CC $GPGRT_CONFIG --variable=host 2>/dev/null || echo none`
    if test x"$gpgrt_config_host" != xnone ; then
      if test x"$gpgrt_config_host" != x"$host" ; then
  AC_MSG_WARN([[
***
*** The config script $GPGRT_CONFIG was
*** built for $gpgrt_config_host and thus may not match the
*** used host $host.
*** You may want to use the configure option --with-libgpgrt-prefix
*** to specify a matching config script or use \$SYSROOT.
***]])
        gpg_config_script_warn="$gpg_config_script_warn libgpgrt"
      fi
    fi
  else
    GPGRT_CFLAGS=""
    GPGRT_LIBS=""
    GPGRT_MT_CFLAGS=""
    GPGRT_MT_LIBS=""
    AC_MSG_RESULT(no)
    ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GPGRT_CFLAGS)
  AC_SUBST(GPGRT_LIBS)
  AC_SUBST(GPGRT_MT_CFLAGS)
  AC_SUBST(GPGRT_MT_LIBS)
])
