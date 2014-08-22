/* visibility.c - Wrapper for all public functions.
 * Copyright (C) 2014  g10 Code GmbH
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
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <stdarg.h>

#define _GPGRT_INCL_BY_VISIBILITY_C 1
#include "gpgrt-int.h"

const char *
gpg_strerror (gpg_error_t err)
{
  return _gpg_strerror (err);
}

int
gpg_strerror_r (gpg_error_t err, char *buf, size_t buflen)
{
  return _gpg_strerror_r (err, buf, buflen);
}

const char *
gpg_strsource (gpg_error_t err)
{
  return _gpg_strsource (err);
}

gpg_err_code_t
gpg_err_code_from_errno (int err)
{
  return _gpg_err_code_from_errno (err);
}

int
gpg_err_code_to_errno (gpg_err_code_t code)
{
  return _gpg_err_code_to_errno (code);
}

gpg_err_code_t
gpg_err_code_from_syserror (void)
{
  return _gpg_err_code_from_syserror ();
}

void
gpg_err_set_errno (int err)
{
  _gpg_err_set_errno (err);
}


const char *
gpg_error_check_version (const char *req_version)
{
  return _gpg_error_check_version (req_version);
}


gpg_err_code_t
gpgrt_lock_init (gpgrt_lock_t *lockhd)
{
  return _gpgrt_lock_init (lockhd);
}

gpg_err_code_t
gpgrt_lock_lock (gpgrt_lock_t *lockhd)
{
  return _gpgrt_lock_lock (lockhd);
}

gpg_err_code_t
gpgrt_lock_unlock (gpgrt_lock_t *lockhd)
{
  return _gpgrt_lock_unlock (lockhd);
}

gpg_err_code_t
gpgrt_lock_destroy (gpgrt_lock_t *lockhd)
{
  return _gpgrt_lock_destroy (lockhd);
}

gpg_err_code_t
gpgrt_yield (void)
{
  return _gpgrt_yield ();
}
