/* strerror.c - Describing an error code.
   Copyright (C) 2003 g10 Code GmbH

   This file is part of libgpg-error.

   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with libgpg-error; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include <gpg-error.h>

#include "gettext.h"
#include "err-codes.h"

/* Return a pointer to a string containing a description of the error
   code in the error value ERR.  This function is not thread-safe.  */
const char *
gpg_strerror (gpg_error_t err)
{
  gpg_err_code_t code = gpg_err_code (err);

  if (code & GPG_ERR_SYSTEM_ERROR)
    {
      int no = gpg_err_code_to_errno (code);
      if (no)
	return strerror (no);
      else
	code = GPG_ERR_UNKNOWN_ERRNO;
    }
  return dgettext (PACKAGE, msgstr + msgidx[msgidxof (code)]);
}


#ifdef HAVE_STRERROR_R
#ifdef STRERROR_R_CHAR_P
/* The GNU C library and probably some other systems have this weird
   variant of strerror_r.  */

/* Return a dynamically allocated string in *STR describing the system
   error NO.  If this call succeeds, return 1.  If this call fails due
   to a resource shortage, set *STR to NULL and return 1.  If this
   call fails because the error number is not valid, don't set *STR
   and return 0.  */
int
system_strerror_r (int no, char **str)
{
  int err;
  char *buffer;
  size_t buffer_len = 128;

  buffer = malloc (buffer_len);
  if (!buffer)
    {
      *str = NULL;
      return 1;
    }

  do
    {
      char *msg = strerror_r (no, buffer, buffer_len);

      if (!msg)
	{
	  /* Possibly this means that the error code is unknown.  */
	  free (buffer);
	  return 0;
	}
      else if (msg != buffer)
	{
	  free (buffer);
	  *str = strdup (msg);
	  return 1;
	}

      buffer[buffer_len - 1] = '\0';
      if (strlen (buffer) == buffer_len - 1)
	{
	  /* We might need more space.  */
	  size_t new_buffer_len = buffer_len * 2;
	  char *new_buffer;

	  if (new_buffer_len < buffer_len)
	    {
	      /* Overflow.  Now, this is serious.  */
	      free (buffer);
	      *str = NULL;
	      return 1;
	    }

	  new_buffer = realloc (buffer, 2 * buffer_len);
	  if (!new_buffer)
	    {
	      free (buffer);
	      *str = NULL;
	      return 1;
	    }
	  buffer = new_buffer;
	  buffer_len = new_buffer_len;
	}
      else
	{
	  *str = buffer;
	  return 1;
	}
    }
  while (1);
}

#else	/* STRERROR_R_CHAR_P */
/* Now the POSIX version.  */

/* Return a dynamically allocated string in *STR describing the system
   error NO.  If this call succeeds, return 1.  If this call fails due
   to a resource shortage, set *STR to NULL and return 1.  If this
   call fails because the error number is not valid, don't set *STR
   and return 0.  */
int
system_strerror_r (int no, char **str)
{
  int err;
  char *buffer;
  size_t buffer_len = 128;

  buffer = malloc (buffer_len);
  if (!buffer)
    {
      *str = NULL;
      return 1;
    }

  do
    {
      err = strerror_r (no, buffer, buffer_len);

      if (err == ERANGE)
	{
	  size_t new_buffer_len = buffer_len * 2;
	  char *new_buffer;

	  if (new_buffer_len < buffer_len)
	    {
	      /* Overflow.  Now, this is serious.  */
	      free (buffer);
	      *str = NULL;
	      return 1;
	    }

	  new_buffer = realloc (buffer, 2 * buffer_len);
	  if (!new_buffer)
	    {
	      free (buffer);
	      *str = NULL;
	      return 1;
	    }
	  buffer = new_buffer;
	  buffer_len = new_buffer_len;
	}
    }
  while (err == ERANGE);

  if (err == EINVAL)
    {
      /* This system error is not known.  */
      free (buffer);
      return 0;
    }
  else if (err)	
    {
      /* strerror_r() failed, but we don't know why.  */
      free (buffer);
      *str = NULL;
      return 1;
    }

  *str = buffer;
  return 1;
}
#endif	/* STRERROR_R_CHAR_P */

#else	/* HAVE_STRERROR_H */
/* Without strerror_r(), we can still provide a non-thread-safe
   version.  Maybe we are even lucky and the system's strerror() is
   already thread-safe.  */

/* Return a dynamically allocated string in *STR describing the system
   error NO.  If this call succeeds, return 1.  If this call fails due
   to a resource shortage, set *STR to NULL and return 1.  If this
   call fails because the error number is not valid, don't set *STR
   and return 0.  */
int
system_strerror_r (int no, char **str)
{
  char *msg = strerror (no);

  if (!msg)
    {
      if (errno == EINVAL)
	return 0;
      else
	{
	  *str = NULL;
	  return 1;
	}
    }
  else
    {
      *str = strdup (msg);
      return 1;
    }
}
#endif


/* Return a pointer to a string containing a description of the error
   code in the error value ERR.  The buffer for the string is
   allocated with malloc(), and has to be released by the user.  On
   error, NULL is returned.  */
char *
gpg_strerror_r (gpg_error_t err)
{
  gpg_err_code_t code = gpg_err_code (err);

  if (code & GPG_ERR_SYSTEM_ERROR)
    {
      int no = gpg_err_code_to_errno (code);
      if (no)
	{
	  char *str;

	  if (system_strerror_r (no, &str))
	    return str;
	}
      code = GPG_ERR_UNKNOWN_ERRNO;
    }
  return strdup (dgettext (PACKAGE, msgstr + msgidx[msgidxof (code)]));
}
