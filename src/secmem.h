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

#if __GNUC__ >= 3
#define LIKELY( expr )    __builtin_expect( !!(expr), 1 )
#define UNLIKELY( expr )  __builtin_expect( !!(expr), 0 )
#else
#define LIKELY( expr )    (!!(expr))
#define UNLIKELY( expr )  (!!(expr))
#endif

typedef union
{
  int a;
  short b;
  char c[1];
  long d;
  uint64_t e;
  float f;
  double g;
} PROPERLY_ALIGNED_TYPE;

/* To avoid that a compiler optimizes certain memset calls away, these
   macros may be used instead. */
#define wipememory2(_ptr,_set,_len) do { \
              volatile char *_vptr=(volatile char *)(_ptr); \
              size_t _vlen=(_len); \
              unsigned char _vset=(_set); \
              fast_wipememory2(_vptr,_vset,_vlen); \
              while(_vlen) { *_vptr=(_vset); _vptr++; _vlen--; } \
                  } while(0)
#define wipememory(_ptr,_len) wipememory2(_ptr,0,_len)

#define FASTWIPE_T uint64_t
#define FASTWIPE_MULT (0x0101010101010101ULL)

/* Following architectures can handle unaligned accesses fast.  */
#if defined(HAVE_GCC_ATTRIBUTE_PACKED) && \
    defined(HAVE_GCC_ATTRIBUTE_ALIGNED) && \
    defined(HAVE_GCC_ATTRIBUTE_MAY_ALIAS) && \
    (defined(__i386__) || defined(__x86_64__) || \
     defined(__powerpc__) || defined(__powerpc64__) || \
     (defined(__arm__) && defined(__ARM_FEATURE_UNALIGNED)) || \
     defined(__aarch64__))
#define fast_wipememory2_unaligned_head(_ptr,_set,_len) /*do nothing*/
typedef struct fast_wipememory_s
{
  FASTWIPE_T a;
} __attribute__((packed, aligned(1), may_alias)) fast_wipememory_t;
#else
#define fast_wipememory2_unaligned_head(_vptr,_vset,_vlen) do { \
              while(UNLIKELY((size_t)(_vptr)&(sizeof(FASTWIPE_T)-1)) && _vlen) \
                { *_vptr=(_vset); _vptr++; _vlen--; } \
                  } while(0)
typedef struct fast_wipememory_s
{
  FASTWIPE_T a;
} fast_wipememory_t;
#endif

/* fast_wipememory2 may leave tail bytes unhandled, in which case tail bytes
   are handled by wipememory2. */
#define fast_wipememory2(_vptr,_vset,_vlen) do { \
              FASTWIPE_T _vset_long = _vset; \
              fast_wipememory2_unaligned_head(_vptr,_vset,_vlen); \
              if (_vlen < sizeof(FASTWIPE_T)) \
                break; \
              _vset_long *= FASTWIPE_MULT; \
              do { \
                volatile fast_wipememory_t *_vptr_long = \
                  (volatile void *)_vptr; \
                _vptr_long->a = _vset_long; \
                _vlen -= sizeof(FASTWIPE_T); \
                _vptr += sizeof(FASTWIPE_T); \
              } while (_vlen >= sizeof(FASTWIPE_T)); \
                  } while (0)

#endif /* G10_SECMEM_H */
