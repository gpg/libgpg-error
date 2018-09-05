/* secmem.h -  internal definitions for secmem
 *	Copyright (C) 2000, 2001, 2002, 2003 Free Software Foundation, Inc.
 *
 * This file is part of Libgpg-error.
 *
 * Libgpg-error is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Libgpg-error is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <https://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1+
 *
 * This file was originally a part of libgcrypt.
 */

#ifndef G10_SECMEM_H
#define G10_SECMEM_H 1

void _gpgrt_secmem_init (size_t npool);
void _gpgrt_secmem_term (void);
void *_gpgrt_secmem_malloc (size_t size, int xhint) GPGRT_ATTR_MALLOC;
void *_gpgrt_secmem_realloc (void *a, size_t newsize, int xhint);
int  _gpgrt_secmem_free (void *a);
void _gpgrt_secmem_dump_stats (int extended);
void _gpgrt_secmem_set_auto_expand (unsigned int chunksize);
void _gpgrt_secmem_set_flags (unsigned flags);
unsigned _gpgrt_secmem_get_flags(void);
int _gpgrt_private_is_secure (const void *p);

/* Flags for _gpgrt_secmem_{set,get}_flags.  */
#define GPGRT_SECMEM_FLAG_NO_WARNING      (1 << 0)
#define GPGRT_SECMEM_FLAG_SUSPEND_WARNING (1 << 1)
#define GPGRT_SECMEM_FLAG_NOT_LOCKED      (1 << 2)
#define GPGRT_SECMEM_FLAG_NO_MLOCK        (1 << 3)
#define GPGRT_SECMEM_FLAG_NO_PRIV_DROP    (1 << 4)

#endif /* G10_SECMEM_H */
