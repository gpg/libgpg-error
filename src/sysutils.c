/* sysutils.c - Platform specific helper functions
 * Copyright (C) 2017 g10 Code GmbH
 *
 * This file is part of libgpg-error.
 *
 * libgpg-error is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * libgpg-error is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <https://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_W32_SYSTEM
# include <windows.h>
#endif

#include "gpgrt-int.h"



/* Return true if FD is valid.  */
int
_gpgrt_fd_valid_p (int fd)
{
  int d = dup (fd);
  if (d < 0)
    return 0;
  close (d);
  return 1;
}


/* Our variant of getenv.  The returned string must be freed.  If the
 * environment variable does not exists NULL is retruned and ERRNO set
 * to 0.  */
char *
_gpgrt_getenv (const char *name)
{
  if (!name || !*name || strchr (name, '='))
    {
      _gpg_err_set_errno (EINVAL);
      return NULL;
    }

#ifdef HAVE_W32_SYSTEM
  {
    int len, size;
    char *result;

    len = GetEnvironmentVariable (name, NULL, 0);
    if (!len && GetLastError () == ERROR_ENVVAR_NOT_FOUND)
      {
        _gpg_err_set_errno (0);
        return NULL;
      }
  again:
    size = len;
    result = _gpgrt_malloc (size);
    if (!result)
      return NULL;
    len = GetEnvironmentVariable (name, result, size);
    if (len >= size)
      {
        /* Changed in the meantime - retry.  */
        _gpgrt_free (result);
        goto again;
      }
    if (!len && GetLastError () == ERROR_ENVVAR_NOT_FOUND)
      {
        /* Deleted in the meantime.  */
        _gpgrt_free (result);
        _gpg_err_set_errno (0);
        return NULL;
      }
    if (!len)
      {
        /* Other error.  FIXME: We need mapping fucntion. */
        _gpgrt_free (result);
        _gpg_err_set_errno (EIO);
        return NULL;
      }

    return result;
  }
#else /*!HAVE_W32_SYSTEM*/
  {
    const char *s = getenv (name);
    if (!s)
      {
        _gpg_err_set_errno (0);
        return NULL;
      }
    return _gpgrt_strdup (s);
  }
#endif /*!HAVE_W32_SYSTEM*/
}


/* Wrapper around setenv so that we can have the same function in
 * Windows and Unix.  In contrast to the standard setenv passing a
 * VALUE as NULL and setting OVERWRITE will remove the envvar.  */
int
_gpgrt_setenv (const char *name, const char *value, int overwrite)
{
  if (!name || !*name || strchr (name, '='))
    {
      _gpg_err_set_errno (EINVAL);
      return -1;
    }

#ifdef HAVE_W32_SYSTEM
  /* Windows maintains (at least) two sets of environment variables.
   * One set can be accessed by GetEnvironmentVariable and
   * SetEnvironmentVariable.  This set is inherited by the children.
   * The other set is maintained in the C runtime, and is accessed
   * using getenv and putenv.  We try to keep them in sync by
   * modifying both sets.  Note that gpgrt_getenv ignores the libc
   * values - however, too much existing code still uses getenv.  */
  {
    int exists;
    char tmpbuf[10];
    char *buf;

    if (!value && overwrite)
      {
        if (!SetEnvironmentVariable (name, NULL))
          {
            _gpg_err_set_errno (EINVAL);
            return -1;
          }
        if (getenv (name))
          {
            /* Ugly: Leaking memory.  */
            buf = _gpgrt_strdup (name);
            if (!buf)
              return -1;
            return putenv (buf);
          }
        return 0;
      }

    exists = GetEnvironmentVariable (name, tmpbuf, sizeof tmpbuf);
    if ((! exists || overwrite) && !SetEnvironmentVariable (name, value))
      {
        _gpg_err_set_errno (EINVAL); /* (Might also be ENOMEM.) */
        return -1;
      }
    if (overwrite || !getenv (name))
      {
        /* Ugly: Leaking memory.  */
        buf = _gpgrt_strconcat (name, "=", value, NULL);
        if (!buf)
          return -1;
        return putenv (buf);
      }
    return 0;
  }

#else /*!HAVE_W32_SYSTEM*/

# ifdef HAVE_SETENV

  {
    if (!value && overwrite)
      return unsetenv (name);
    else
      return setenv (name, value, overwrite);
  }

# else /*!HAVE_SETENV*/

# if __GNUC__
#   warning no setenv - using putenv but leaking memory.
# endif
  {
    char *buf;

    if (!value && overwrite)
      {
        if (getenv (name))
          {
            buf = _gpgrt_strdup (name);
            if (!buf)
              return -1;
            return putenv (buf);
          }
      }
    else if (overwrite || !getenv (name))
      {
        buf = _gpgrt_strconcat (name, "=", value, NULL);
        if (!buf)
          return -1;
        return putenv (buf);
      }

    return 0;
  }
# endif /*!HAVE_SETENV*/
#endif /*!HAVE_W32_SYSTEM*/
}
