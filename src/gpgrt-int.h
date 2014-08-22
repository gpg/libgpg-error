/* gpgrt-int.h - Internal definitions
 * Copyright (C) 2014 g10 Code GmbH
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

#ifndef _GPGRT_GPGRT_INT_H
#define _GPGRT_GPGRT_INT_H

#include "gpg-error.h"
#include "visibility.h"

/* Local error function prototypes.  */
const char *_gpg_strerror (gpg_error_t err);
int _gpg_strerror_r (gpg_error_t err, char *buf, size_t buflen);
const char *_gpg_strsource (gpg_error_t err);
gpg_err_code_t _gpg_err_code_from_errno (int err);
int _gpg_err_code_to_errno (gpg_err_code_t code);
gpg_err_code_t _gpg_err_code_from_syserror (void);
void _gpg_err_set_errno (int err);

const char *_gpg_error_check_version (const char *req_version);

gpg_err_code_t _gpgrt_lock_init (gpgrt_lock_t *lockhd);
gpg_err_code_t _gpgrt_lock_lock (gpgrt_lock_t *lockhd);
gpg_err_code_t _gpgrt_lock_unlock (gpgrt_lock_t *lockhd);
gpg_err_code_t _gpgrt_lock_destroy (gpgrt_lock_t *lockhd);
gpg_err_code_t _gpgrt_yield (void);


/* Local prototypes for estream.  */
int _gpgrt_es_init (void);



#endif /*_GPGRT_GPGRT_INT_H*/
