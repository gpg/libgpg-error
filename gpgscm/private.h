/* TinyScheme-based test driver.
 * Copyright (C) 2016 g10 code GmbH
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
 */

#ifndef GPGSCM_PRIVATE_H
#define GPGSCM_PRIVATE_H


#include "../src/gpgrt.h"

/*
 * Internal i18n macros.
 */
#ifdef ENABLE_NLS
# ifdef HAVE_W32_SYSTEM
#  include "gettext.h"
# else
#  include <libintl.h>
# endif
# define _(a) gettext (a)
# ifdef gettext_noop
#  define N_(a) gettext_noop (a)
# else
#  define N_(a) (a)
# endif
#else  /*!ENABLE_NLS*/
# define _(a) (a)
# define N_(a) (a)
#endif /*!ENABLE_NLS */

#define xfree(a)         gpgrt_free ((a))
#define xtrymalloc(a)    gpgrt_malloc ((a))
#define xtrycalloc(a,b)  gpgrt_calloc ((a),(b))
#define xtryrealloc(a,b) gpgrt_realloc ((a),(b))


static GPGRT_INLINE void *
xmalloc (size_t n)
{
  void *p = gpgrt_malloc (n);
  if (!p)
    log_fatal ("malloc failed: %s\n", strerror (errno));
  return p;
}

static GPGRT_INLINE char *
xstrdup (const char *s)
{
  char *p = gpgrt_malloc (strlen (s) + 1);
  strcpy (p, s);
  return p;
}


extern int verbose;


#endif /* GPGSCM_PRIVATE_H */
