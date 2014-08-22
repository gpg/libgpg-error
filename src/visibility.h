/* visibility.h - Set visibility attribute
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

#ifndef _GPGRT_VISIBILITY_H
#define _GPGRT_VISIBILITY_H

/* Include the main header here so that public symbols are mapped to
   the internal underscored ones.  */
#ifdef _GPGRT_INCL_BY_VISIBILITY_C
# include "gpgrt-int.h"
#endif


/* Our use of the ELF visibility feature works by passing
   -fvisibiliy=hidden on the command line and by explicitly marking
   all exported functions as visible.

   NOTE: When adding new functions, please make sure to add them to
         gpg-error.vers and gpg-error.def.in as well.  */

#ifdef _GPGRT_INCL_BY_VISIBILITY_C

# ifdef GPGRT_USE_VISIBILITY
#  define MARK_VISIBLE(name) \
     extern __typeof__ (name) name __attribute__ ((visibility("default")));
# else
#  define MARK_VISIBLE(name) /* */
# endif

MARK_VISIBLE (gpg_strerror)
MARK_VISIBLE (gpg_strerror_r)
MARK_VISIBLE (gpg_strsource)
MARK_VISIBLE (gpg_err_code_from_errno)
MARK_VISIBLE (gpg_err_code_to_errno)
MARK_VISIBLE (gpg_err_code_from_syserror)
MARK_VISIBLE (gpg_err_set_errno)

MARK_VISIBLE (gpg_error_check_version)

MARK_VISIBLE (gpgrt_lock_init)
MARK_VISIBLE (gpgrt_lock_lock)
MARK_VISIBLE (gpgrt_lock_unlock)
MARK_VISIBLE (gpgrt_lock_destroy)
MARK_VISIBLE (gpgrt_yield)



#undef MARK_VISIBLE

#else /*!_GPGRT_INCL_BY_VISIBILITY_C*/

/* To avoid accidental use of the public functions inside Libgpg-error,
   we redefine them to catch such errors.  */

#define gpg_strerror                _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_strerror_r              _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_strsource               _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_err_code_from_errno     _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_err_code_to_errno       _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_err_code_from_syserror  _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpg_err_set_errno           _gpgrt_USE_UNDERSCORED_FUNCTION

#define gpg_error_check_version     _gpgrt_USE_UNDERSCORED_FUNCTION

#define gpgrt_lock_init             _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpgrt_lock_lock             _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpgrt_lock_unlock           _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpgrt_lock_destroy          _gpgrt_USE_UNDERSCORED_FUNCTION
#define gpgrt_yield                 _gpgrt_USE_UNDERSCORED_FUNCTION



#endif /*!_GPGRT_INCL_BY_VISIBILITY_C*/

#endif /*_GPGRT_VISIBILITY_H*/
