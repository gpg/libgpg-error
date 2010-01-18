/* init.c - Initialize the GnuPG error library.
   Copyright (C) 2005 g10 Code GmbH

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
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <gpg-error.h>

#include "gettext.h"


/* Locale directory support.  */

#if HAVE_W32_SYSTEM
/* The TLS space definition. */
struct tls_space_s
{
  /* 119 bytes for an error message should be enough.  With this size
     we can assume that the allocation does not take up more than 128
     bytes per thread.  */
  char strerror_buffer[120];
};
static int tls_index;  /* Index for the TLS functions.  */ 

static char *get_locale_dir (void);
static void drop_locale_dir (char *locale_dir);

#else /*!HAVE_W32_SYSTEM*/

#define get_locale_dir() LOCALEDIR
#define drop_locale_dir(dir)

#endif /*!HAVE_W32_SYSTEM*/


/* Initialize the library.  This function should be run early.  */
gpg_error_t
gpg_err_init (void)
{
#ifdef ENABLE_NLS
  char *locale_dir;

  /* We only have to bind our locale directory to our text domain.  */

  locale_dir = get_locale_dir ();
  if (locale_dir)
    {
      bindtextdomain (PACKAGE, locale_dir);
      drop_locale_dir (locale_dir);
    }
#endif

  return 0;
}



#ifdef HAVE_W32_SYSTEM

#include <windows.h>

static HKEY
get_root_key(const char *root)
{
  HKEY root_key;

  if( !root )
    root_key = HKEY_CURRENT_USER;
  else if( !strcmp( root, "HKEY_CLASSES_ROOT" ) )
    root_key = HKEY_CLASSES_ROOT;
  else if( !strcmp( root, "HKEY_CURRENT_USER" ) )
    root_key = HKEY_CURRENT_USER;
  else if( !strcmp( root, "HKEY_LOCAL_MACHINE" ) )
    root_key = HKEY_LOCAL_MACHINE;
  else if( !strcmp( root, "HKEY_USERS" ) )
    root_key = HKEY_USERS;
  else if( !strcmp( root, "HKEY_PERFORMANCE_DATA" ) )
    root_key = HKEY_PERFORMANCE_DATA;
  else if( !strcmp( root, "HKEY_CURRENT_CONFIG" ) )
    root_key = HKEY_CURRENT_CONFIG;
  else
    return NULL;
  return root_key;
}

/****************
 * Return a string from the Win32 Registry or NULL in case of
 * error.  Caller must release the return value.   A NULL for root
 * is an alias for HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE in turn.
 * NOTE: The value is allocated with a plain malloc() - use free() and not
 * the usual xfree()!!!
 */
static char *
read_w32_registry_string( const char *root, const char *dir, const char *name )
{
  HKEY root_key, key_handle;
  DWORD n1, nbytes, type;
  char *result = NULL;

  if ( !(root_key = get_root_key(root) ) )
    return NULL;

  if( RegOpenKeyEx( root_key, dir, 0, KEY_READ, &key_handle ) )
    {
      if (root)
	return NULL; /* no need for a RegClose, so return direct */
      /* It seems to be common practise to fall back to HKLM. */
      if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &key_handle) )
	return NULL; /* still no need for a RegClose, so return direct */
    }

  nbytes = 1;
  if( RegQueryValueEx( key_handle, name, 0, NULL, NULL, &nbytes ) ) 
    {
      if (root)
        goto leave;
      /* Try to fallback to HKLM also vor a missing value.  */
      RegCloseKey (key_handle);
      if (RegOpenKeyEx (HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &key_handle) )
        return NULL; /* Nope.  */
      if (RegQueryValueEx( key_handle, name, 0, NULL, NULL, &nbytes))
        goto leave;
    }
  result = malloc ( (n1=nbytes+1) );
  if( !result )
    goto leave;
  if( RegQueryValueEx( key_handle, name, 0, &type, result, &n1 ) ) 
    {
      free(result); result = NULL;
      goto leave;
    }
  result[nbytes] = 0; /* make sure it is really a string  */

#ifndef HAVE_W32CE_SYSTEM  /* W32CE has no environment variables.  */
  if (type == REG_EXPAND_SZ && strchr (result, '%')) 
    {
      char *tmp;
      
      n1 += 1000;
      tmp = malloc (n1+1);
      if (!tmp)
        goto leave;
      nbytes = ExpandEnvironmentStrings (result, tmp, n1);
      if (nbytes && nbytes > n1) 
        {
          free (tmp);
          n1 = nbytes;
          tmp = malloc (n1 + 1);
          if (!tmp)
            goto leave;
          nbytes = ExpandEnvironmentStrings (result, tmp, n1);
          if (nbytes && nbytes > n1)
            {
              free (tmp); /* oops - truncated, better don't expand at all */
              goto leave;
            }
          tmp[nbytes] = 0;
          free (result);
          result = tmp;
        }
      else if (nbytes) /* okay, reduce the length */
        {
          tmp[nbytes] = 0;
          free (result);
          result = malloc (strlen (tmp)+1);
          if (!result)
            result = tmp;
          else {
            strcpy (result, tmp);
            free (tmp);
          }
        }
      else /* error - don't expand */
        {
          free (tmp);
        }
    }
#endif /*HAVE_W32CE_SYSTEM*/

 leave:
  RegCloseKey( key_handle );
  return result;
}


#define REGKEY "Software\\GNU\\GnuPG"

static char *
get_locale_dir (void)
{
  char *instdir;
  char *p;
  char *dname;

  instdir = read_w32_registry_string ("HKEY_LOCAL_MACHINE", REGKEY,
				      "Install Directory");
  if (!instdir)
    return NULL;
  
  /* Build the key: "<instdir>/share/locale".  */
#define SLDIR "\\share\\locale"
  dname = malloc (strlen (instdir) + strlen (SLDIR) + 1);
  if (!dname)
    {
      free (instdir);
      return NULL;
    }
  p = dname;
  strcpy (p, instdir);
  p += strlen (instdir);
  strcpy (p, SLDIR);
  
  free (instdir);
  
  return dname;
}


static void
drop_locale_dir (char *locale_dir)
{
  free (locale_dir);
}


/* Return the tls object.  This function is guaranteed to return a
   valid non-NULL object.  */
#ifdef HAVE_W32CE_SYSTEM
static struct tls_space_s *
get_tls (void)
{
  struct tls_space_s *tls;

  tls = TlsGetValue (tls_index);
  if (!tls)
    {
      /* Called by a thread which existed before this DLL was loaded.
         Allocate the space.  */
      tls = LocalAlloc (LPTR, sizeof *tls);
      if (!tls)
        {
          /* No way to continue - commit suicide.  */
          abort ();
        }
      TlsSetValue (tls_index, tls);
    }
        
  return tls;
}
#endif /*HAVE_W32CE_SYSTEM*/

/* Return the value of the ERRNO variable.  This needs to be a
   function so that we can have a per-thread ERRNO.  This is used only
   on WindowsCE because that OS misses an errno.   */
#ifdef HAVE_W32CE_SYSTEM
int
_gpg_w32ce_get_errno (void)
{
  int err;

  err = GetLastError ();
  /* FIXME: Should we fold some W32 error codes into the same errno
     value? */
  return err;
}
#endif /*HAVE_W32CE_SYSTEM*/


/* Replacement strerror function for WindowsCE.  */
#ifdef HAVE_W32CE_SYSTEM
char *
_gpg_w32ce_strerror (int err)
{
  struct tls_space_s *tls = get_tls ();

  if (err == -1)
    err = _gpg_w32ce_get_errno ();
  if (!FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
                      MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                      tls->strerror_buffer, sizeof tls->strerror_buffer -1,
                      NULL))
    snprintf (tls->strerror_buffer, sizeof tls->strerror_buffer -1,
              "[w32err=%d]", err);
  return tls->strerror_buffer;    
}
#endif /*HAVE_W32CE_SYSTEM*/


void
gpg_err_set_errno (int err)
{
#ifdef HAVE_W32CE_SYSTEM
  SetLastError (err);
#else /*!HAVE_W32CE_SYSTEM*/
  errno = err;
#endif /*!HAVE_W32CE_SYSTEM*/
}


/* Entry point called by the DLL loader.  This is only used by
   WindowsCE for now; we might eventually use TLS to implement a
   thread safe strerror.  */
#ifdef HAVE_W32CE_SYSTEM
int WINAPI
DllMain (HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
  struct tls_space_s *tls;
  (void)reserved;

  switch (reason)
    {
    case DLL_PROCESS_ATTACH:
      tls_index = TlsAlloc ();
      if (tls_index == TLS_OUT_OF_INDEXES)
        return FALSE; 
      /* falltru.  */
    case DLL_THREAD_ATTACH:
      tls = LocalAlloc (LPTR, sizeof *tls);
      if (!tls)
        return FALSE;
      TlsSetValue (tls_index, tls);
      break;

    case DLL_THREAD_DETACH:
      tls = TlsGetValue (tls_index);
      if (tls)
        LocalFree (tls);
      break;

    case DLL_PROCESS_DETACH:
      tls = TlsGetValue (tls_index);
      if (tls)
        LocalFree (tls);
      TlsFree (tls_index);
      break;

    default:
      break;
    }
  
  return TRUE;
}
#endif /*HAVE_W32CE_SYSTEM*/

#else /*!HAVE_W32_SYSTEM*/

void
gpg_err_set_errno (int err)
{
  errno = err;
}

#endif /*!HAVE_W32_SYSTEM*/
