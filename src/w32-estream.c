/* w32-estream.c - es_poll support on W32.
 * Copyright (C) 2000 Werner Koch (dd9jn)
 * Copyright (C) 2001, 2002, 2003, 2004, 2007, 2010, 2016 g10 Code GmbH
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
 */

/*
 * This file is based on GPGME's w32-io.c started in 2001.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <io.h>
#include <windows.h>

#include "gpgrt-int.h"

/*
 * In order to support es_poll on Windows, we create a proxy shim that
 * we use as the estream I/O functions.  This shim creates reader and
 * writer threads that use the original I/O functions.
 */



/* Tracing/debugging support.  */
#if 0
#define TRACE(msg, ...)                                         \
  fprintf (stderr, msg, ## __VA_ARGS__)
#define TRACE_CTX(ctx, msg, ...)                                \
  fprintf (stderr, "%p: " msg "\n", ctx, ## __VA_ARGS__)
#define TRACE_ERR(ctx, err, msg, ...)	do {                    \
    char error_message[128];                                    \
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM                    \
                  | FORMAT_MESSAGE_IGNORE_INSERTS,              \
                  NULL,                                         \
                  err,                                          \
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    \
                  (LPTSTR) &error_message,                      \
                  sizeof error_message, NULL );                 \
    fprintf (stderr, "%p: " msg ": %s\n", ctx,                  \
             ## __VA_ARGS__, error_message);                    \
  } while (0)
#else
#define TRACE(msg, ...)			(void) 0
#define TRACE_CTX(ctx, msg, ...)	(void) 0
#define TRACE_ERR(ctx, err, msg, ...)	(void) 0
#endif



/* Calculate array dimension.  */
#ifndef DIM
#define DIM(array) (sizeof (array) / sizeof (*array))
#endif

#define READBUF_SIZE 4096
#define WRITEBUF_SIZE 4096


typedef struct estream_cookie_w32_pollable *estream_cookie_w32_pollable_t;

struct reader_context_s
{
  estream_cookie_w32_pollable_t pcookie;
  HANDLE thread_hd;

  gpgrt_lock_t mutex;

  int stop_me;
  int eof;
  int eof_shortcut;
  int error;
  int error_code;

  /* This is manually reset.  */
  HANDLE have_data_ev;
  /* This is automatically reset.  */
  HANDLE have_space_ev;
  /* This is manually reset but actually only triggered once.  */
  HANDLE close_ev;

  size_t readpos, writepos;
  char buffer[READBUF_SIZE];
};

struct writer_context_s
{
  estream_cookie_w32_pollable_t pcookie;
  HANDLE thread_hd;

  gpgrt_lock_t mutex;

  int stop_me;
  int error;
  int error_code;

  /* This is manually reset.  */
  HANDLE have_data;
  HANDLE is_empty;
  HANDLE close_ev;
  size_t nbytes;
  char buffer[WRITEBUF_SIZE];
};

/* Cookie for pollable objects.  */
struct estream_cookie_w32_pollable
{
  unsigned int modeflags;

  struct cookie_io_functions_s next_functions;
  void *next_cookie;

  struct reader_context_s *reader;
  struct writer_context_s *writer;
};


static HANDLE
set_synchronize (HANDLE hd)
{
#ifdef HAVE_W32CE_SYSTEM
  return hd;
#else
  HANDLE new_hd;

  /* For NT we have to set the sync flag.  It seems that the only way
     to do it is by duplicating the handle.  Tsss...  */
  if (!DuplicateHandle (GetCurrentProcess (), hd,
			GetCurrentProcess (), &new_hd,
			EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, 0))
    {
      TRACE_ERR (NULL, GetLastError (), "DuplicateHandle failed");
      /* FIXME: Should translate the error code.  */
      _gpg_err_set_errno (EIO);
      return INVALID_HANDLE_VALUE;
    }

  CloseHandle (hd);
  return new_hd;
#endif
}


static DWORD CALLBACK
reader (void *arg)
{
  struct reader_context_s *ctx = arg;
  int nbytes;
  ssize_t nread;
  TRACE_CTX (ctx, "reader starting");

  for (;;)
    {
      _gpgrt_lock_lock (&ctx->mutex);
      /* Leave a 1 byte gap so that we can see whether it is empty or
	 full.  */
      while ((ctx->writepos + 1) % READBUF_SIZE == ctx->readpos)
	{
	  /* Wait for space.  */
	  if (!ResetEvent (ctx->have_space_ev))
	    TRACE_ERR (ctx, GetLastError (), "ResetEvent failed");
	  _gpgrt_lock_unlock (&ctx->mutex);
	  TRACE_CTX (ctx, "waiting for space");
	  WaitForSingleObject (ctx->have_space_ev, INFINITE);
	  TRACE_CTX (ctx, "got space");
	  _gpgrt_lock_lock (&ctx->mutex);
        }
      assert (((ctx->writepos + 1) % READBUF_SIZE != ctx->readpos));
      if (ctx->stop_me)
	{
	  _gpgrt_lock_unlock (&ctx->mutex);
	  break;
        }
      nbytes = (ctx->readpos + READBUF_SIZE
		- ctx->writepos - 1) % READBUF_SIZE;
      assert (nbytes);
      if (nbytes > READBUF_SIZE - ctx->writepos)
	nbytes = READBUF_SIZE - ctx->writepos;
      _gpgrt_lock_unlock (&ctx->mutex);

      TRACE_CTX (ctx, "reading up to %d bytes", nbytes);

      nread = ctx->pcookie->next_functions.public.func_read
        (ctx->pcookie->next_cookie, ctx->buffer + ctx->writepos, nbytes);
      TRACE_CTX (ctx, "got %d bytes", nread);
      if (nread < 0)
        {
          ctx->error_code = (int) errno;
          /* NOTE (W32CE): Do not ignore ERROR_BUSY!  Check at
             least stop_me if that happens.  */
          if (ctx->error_code == ERROR_BROKEN_PIPE)
            {
              ctx->eof = 1;
              TRACE_CTX (ctx, "got EOF (broken pipe)");
            }
          else
            {
              ctx->error = 1;
              TRACE_ERR (ctx, ctx->error_code, "read error");
            }
          break;
        }

      _gpgrt_lock_lock (&ctx->mutex);
      if (ctx->stop_me)
	{
	  _gpgrt_lock_unlock (&ctx->mutex);
	  break;
        }
      if (!nread)
	{
	  ctx->eof = 1;
	  TRACE_CTX (ctx, "got eof");
	  _gpgrt_lock_unlock (&ctx->mutex);
	  break;
        }

      ctx->writepos = (ctx->writepos + nread) % READBUF_SIZE;
      if (!SetEvent (ctx->have_data_ev))
	TRACE_ERR (ctx, GetLastError (), "SetEvent (%p) failed",
                   ctx->have_data_ev);
      _gpgrt_lock_unlock (&ctx->mutex);
    }
  /* Indicate that we have an error or EOF.  */
  if (!SetEvent (ctx->have_data_ev))
    TRACE_ERR (ctx, GetLastError (), "SetEvent (%p) failed",
               ctx->have_data_ev);

  TRACE_CTX (ctx, "waiting for close");
  WaitForSingleObject (ctx->close_ev, INFINITE);

  CloseHandle (ctx->close_ev);
  CloseHandle (ctx->have_data_ev);
  CloseHandle (ctx->have_space_ev);
  CloseHandle (ctx->thread_hd);
  _gpgrt_lock_destroy (&ctx->mutex);
  _gpgrt_free (ctx);

  return 0;
}


static struct reader_context_s *
create_reader (estream_cookie_w32_pollable_t pcookie)
{
  struct reader_context_s *ctx;
  SECURITY_ATTRIBUTES sec_attr;
  DWORD tid;

  memset (&sec_attr, 0, sizeof sec_attr);
  sec_attr.nLength = sizeof sec_attr;
  sec_attr.bInheritHandle = FALSE;

  ctx = calloc (1, sizeof *ctx);
  if (!ctx)
    {
      return NULL;
    }

  ctx->pcookie = pcookie;

  ctx->have_data_ev = CreateEvent (&sec_attr, TRUE, FALSE, NULL);
  if (ctx->have_data_ev)
    ctx->have_space_ev = CreateEvent (&sec_attr, FALSE, TRUE, NULL);
  if (ctx->have_space_ev)
    ctx->close_ev = CreateEvent (&sec_attr, TRUE, FALSE, NULL);
  if (!ctx->have_data_ev || !ctx->have_space_ev || !ctx->close_ev)
    {
      TRACE_ERR (ctx, GetLastError (), "CreateEvent failed");
      if (ctx->have_data_ev)
	CloseHandle (ctx->have_data_ev);
      if (ctx->have_space_ev)
	CloseHandle (ctx->have_space_ev);
      if (ctx->close_ev)
	CloseHandle (ctx->close_ev);
      _gpgrt_free (ctx);
      return NULL;
    }

  ctx->have_data_ev = set_synchronize (ctx->have_data_ev);
  _gpgrt_lock_init (&ctx->mutex);

#ifdef HAVE_W32CE_SYSTEM
  ctx->thread_hd = CreateThread (&sec_attr, 64 * 1024, reader, ctx,
				 STACK_SIZE_PARAM_IS_A_RESERVATION, &tid);
#else
  ctx->thread_hd = CreateThread (&sec_attr, 0, reader, ctx, 0, &tid);
#endif

  if (!ctx->thread_hd)
    {
      TRACE_ERR (ctx, GetLastError (), "CreateThread failed");
      _gpgrt_lock_destroy (&ctx->mutex);
      if (ctx->have_data_ev)
	CloseHandle (ctx->have_data_ev);
      if (ctx->have_space_ev)
	CloseHandle (ctx->have_space_ev);
      if (ctx->close_ev)
	CloseHandle (ctx->close_ev);
      _gpgrt_free (ctx);
      return NULL;
    }
  else
    {
#if 0
      /* We set the priority of the thread higher because we know that
         it only runs for a short time.  This greatly helps to
         increase the performance of the I/O.  */
      SetThreadPriority (ctx->thread_hd, get_desired_thread_priority ());
#endif
    }

  return ctx;
}


/* Prepare destruction of the reader thread for CTX.  Returns 0 if a
   call to this function is sufficient and destroy_reader_finish shall
   not be called.  */
static void
destroy_reader (struct reader_context_s *ctx)
{
  _gpgrt_lock_lock (&ctx->mutex);
  ctx->stop_me = 1;
  if (ctx->have_space_ev)
    SetEvent (ctx->have_space_ev);
  _gpgrt_lock_unlock (&ctx->mutex);

#ifdef HAVE_W32CE_SYSTEM
  /* Scenario: We never create a full pipe, but already started
     reading.  Then we need to unblock the reader in the pipe driver
     to make our reader thread notice that we want it to go away.  */

  if (ctx->file_hd != INVALID_HANDLE_VALUE)
    {
      if (!DeviceIoControl (ctx->file_hd, GPGCEDEV_IOCTL_UNBLOCK,
			NULL, 0, NULL, 0, NULL, NULL))
	{
	  TRACE_ERR (ctx, GetLastError (), "unblock control call failed");
	}
    }
#endif

  /* XXX is it feasible to unblock the thread?  */

  /* After setting this event CTX is void. */
  SetEvent (ctx->close_ev);
}


/*
 * Read function for pollable objects.
 */
static gpgrt_ssize_t
func_w32_pollable_read (void *cookie, void *buffer, size_t count)
{
  estream_cookie_w32_pollable_t pcookie = cookie;
  gpgrt_ssize_t nread;
  struct reader_context_s *ctx;

  ctx = pcookie->reader;
  if (ctx == NULL)
    {
      pcookie->reader = ctx = create_reader (pcookie);
      if (!ctx)
        {
          _gpg_err_set_errno (EBADF);
          return -1;
        }
    }

  TRACE_CTX (ctx, "pollable read buffer=%p, count=%u", buffer, count);

  if (ctx->eof_shortcut)
    return 0;

  _gpgrt_lock_lock (&ctx->mutex);
  TRACE_CTX (ctx, "readpos: %d, writepos %d", ctx->readpos, ctx->writepos);
  if (ctx->readpos == ctx->writepos && !ctx->error)
    {
      /* No data available.  */
      int eof = ctx->eof;
      _gpgrt_lock_unlock (&ctx->mutex);

      if (pcookie->modeflags & O_NONBLOCK && ! eof)
        {
          _gpg_err_set_errno (EAGAIN);
          return -1;
        }

      TRACE_CTX (ctx, "waiting for data");
      WaitForSingleObject (ctx->have_data_ev, INFINITE);
      TRACE_CTX (ctx, "data available");
      _gpgrt_lock_lock (&ctx->mutex);
    }

  if (ctx->readpos == ctx->writepos || ctx->error)
    {
      _gpgrt_lock_unlock (&ctx->mutex);
      ctx->eof_shortcut = 1;
      if (ctx->eof)
	return 0;
      if (!ctx->error)
	{
	  TRACE_CTX (ctx, "EOF but ctx->eof flag not set");
	  return 0;
	}
      _gpg_err_set_errno (ctx->error_code);
      return -1;
    }

  nread = ctx->readpos < ctx->writepos
    ? ctx->writepos - ctx->readpos
    : READBUF_SIZE - ctx->readpos;
  if (nread > count)
    nread = count;
  memcpy (buffer, ctx->buffer + ctx->readpos, nread);
  ctx->readpos = (ctx->readpos + nread) % READBUF_SIZE;
  if (ctx->readpos == ctx->writepos && !ctx->eof)
    {
      if (!ResetEvent (ctx->have_data_ev))
	{
	  TRACE_ERR (ctx, GetLastError (), "ResetEvent failed");
	  _gpgrt_lock_unlock (&ctx->mutex);
	  /* FIXME: Should translate the error code.  */
	  _gpg_err_set_errno (EIO);
	  return -1;
	}
    }
  if (!SetEvent (ctx->have_space_ev))
    {
      TRACE_ERR (ctx, GetLastError (), "SetEvent (%p) failed",
                 ctx->have_space_ev);
      _gpgrt_lock_unlock (&ctx->mutex);
      /* FIXME: Should translate the error code.  */
      _gpg_err_set_errno (EIO);
      return -1;
    }
  _gpgrt_lock_unlock (&ctx->mutex);

  return nread;
}


/* The writer does use a simple buffering strategy so that we are
   informed about write errors as soon as possible (i. e. with the the
   next call to the write function.  */
static DWORD CALLBACK
writer (void *arg)
{
  struct writer_context_s *ctx = arg;
  ssize_t nwritten;

  TRACE_CTX (ctx, "writer starting");

  for (;;)
    {
      _gpgrt_lock_lock (&ctx->mutex);
      if (ctx->stop_me && !ctx->nbytes)
	{
	  _gpgrt_lock_unlock (&ctx->mutex);
	  break;
        }
      if (!ctx->nbytes)
	{
	  if (!SetEvent (ctx->is_empty))
	    TRACE_ERR (ctx, GetLastError (), "SetEvent failed");
	  if (!ResetEvent (ctx->have_data))
	    TRACE_ERR (ctx, GetLastError (), "ResetEvent failed");
	  _gpgrt_lock_unlock (&ctx->mutex);
	  TRACE_CTX (ctx, "idle");
	  WaitForSingleObject (ctx->have_data, INFINITE);
	  TRACE_CTX (ctx, "got data to write");
	  _gpgrt_lock_lock (&ctx->mutex);
        }
      if (ctx->stop_me && !ctx->nbytes)
	{
	  _gpgrt_lock_unlock (&ctx->mutex);
	  break;
        }
      _gpgrt_lock_unlock (&ctx->mutex);

      TRACE_CTX (ctx, "writing up to %d bytes", ctx->nbytes);

      nwritten = ctx->pcookie->next_functions.public.func_write
        (ctx->pcookie->next_cookie, ctx->buffer, ctx->nbytes);
      TRACE_CTX (ctx, "wrote %d bytes", nwritten);
      if (nwritten < 1)
        {
          /* XXX */
          if (errno == ERROR_BUSY)
            {
              /* Probably stop_me is set now.  */
              TRACE_CTX (ctx, "pipe busy (unblocked?)");
              continue;
            }

          ctx->error_code = errno;
          ctx->error = 1;
          TRACE_ERR (ctx, ctx->error_code, "write error");
          break;
        }

      _gpgrt_lock_lock (&ctx->mutex);
      ctx->nbytes -= nwritten;
      _gpgrt_lock_unlock (&ctx->mutex);
    }
  /* Indicate that we have an error.  */
  if (!SetEvent (ctx->is_empty))
    TRACE_ERR (ctx, GetLastError (), "SetEvent failed");

  TRACE_CTX (ctx, "waiting for close");
  WaitForSingleObject (ctx->close_ev, INFINITE);

  if (ctx->nbytes)
    TRACE_CTX (ctx, "still %d bytes in buffer at close time", ctx->nbytes);

  CloseHandle (ctx->close_ev);
  CloseHandle (ctx->have_data);
  CloseHandle (ctx->is_empty);
  CloseHandle (ctx->thread_hd);
  _gpgrt_lock_destroy (&ctx->mutex);
  _gpgrt_free (ctx);

  return 0;
}


static struct writer_context_s *
create_writer (estream_cookie_w32_pollable_t pcookie)
{
  struct writer_context_s *ctx;
  SECURITY_ATTRIBUTES sec_attr;
  DWORD tid;

  memset (&sec_attr, 0, sizeof sec_attr);
  sec_attr.nLength = sizeof sec_attr;
  sec_attr.bInheritHandle = FALSE;

  ctx = calloc (1, sizeof *ctx);
  if (!ctx)
    {
      return NULL;
    }

  ctx->pcookie = pcookie;

  ctx->have_data = CreateEvent (&sec_attr, TRUE, FALSE, NULL);
  if (ctx->have_data)
    ctx->is_empty  = CreateEvent (&sec_attr, TRUE, TRUE, NULL);
  if (ctx->is_empty)
    ctx->close_ev = CreateEvent (&sec_attr, TRUE, FALSE, NULL);
  if (!ctx->have_data || !ctx->is_empty || !ctx->close_ev)
    {
      TRACE_ERR (ctx, GetLastError (), "CreateEvent failed");
      if (ctx->have_data)
	CloseHandle (ctx->have_data);
      if (ctx->is_empty)
	CloseHandle (ctx->is_empty);
      if (ctx->close_ev)
	CloseHandle (ctx->close_ev);
      _gpgrt_free (ctx);
      return NULL;
    }

  ctx->is_empty = set_synchronize (ctx->is_empty);
  _gpgrt_lock_init (&ctx->mutex);

#ifdef HAVE_W32CE_SYSTEM
  ctx->thread_hd = CreateThread (&sec_attr, 64 * 1024, writer, ctx,
				 STACK_SIZE_PARAM_IS_A_RESERVATION, &tid);
#else
  ctx->thread_hd = CreateThread (&sec_attr, 0, writer, ctx, 0, &tid );
#endif

  if (!ctx->thread_hd)
    {
      TRACE_ERR (ctx, GetLastError (), "CreateThread failed");
      _gpgrt_lock_destroy (&ctx->mutex);
      if (ctx->have_data)
	CloseHandle (ctx->have_data);
      if (ctx->is_empty)
	CloseHandle (ctx->is_empty);
      if (ctx->close_ev)
	CloseHandle (ctx->close_ev);
      _gpgrt_free (ctx);
      return NULL;
    }
  else
    {
#if 0
      /* We set the priority of the thread higher because we know
	 that it only runs for a short time.  This greatly helps to
	 increase the performance of the I/O.  */
      SetThreadPriority (ctx->thread_hd, get_desired_thread_priority ());
#endif
    }

  return ctx;
}


static void
destroy_writer (struct writer_context_s *ctx)
{
  _gpgrt_lock_lock (&ctx->mutex);
  ctx->stop_me = 1;
  if (ctx->have_data)
    SetEvent (ctx->have_data);
  _gpgrt_lock_unlock (&ctx->mutex);

  /* Give the writer a chance to flush the buffer.  */
  WaitForSingleObject (ctx->is_empty, INFINITE);

#ifdef HAVE_W32CE_SYSTEM
  /* Scenario: We never create a full pipe, but already started
     writing more than the pipe buffer.  Then we need to unblock the
     writer in the pipe driver to make our writer thread notice that
     we want it to go away.  */

  if (!DeviceIoControl (ctx->file_hd, GPGCEDEV_IOCTL_UNBLOCK,
			NULL, 0, NULL, 0, NULL, NULL))
    {
      TRACE_ERR (ctx, GetLastError (), "unblock control call failed");
    }
#endif

  /* After setting this event CTX is void.  */
  SetEvent (ctx->close_ev);
}


/*
 * Write function for pollable objects.
 */
static gpgrt_ssize_t
func_w32_pollable_write (void *cookie, const void *buffer, size_t count)
{
  estream_cookie_w32_pollable_t pcookie = cookie;
  struct writer_context_s *ctx;

  if (count == 0)
    return 0;

  ctx = pcookie->writer;
  if (ctx == NULL)
    {
      pcookie->writer = ctx = create_writer (pcookie);
      if (!ctx)
        return -1;
    }

  _gpgrt_lock_lock (&ctx->mutex);
  TRACE_CTX (ctx, "pollable write buffer: %p, count: %d, nbytes: %d",
         buffer, count, ctx->nbytes);
  if (!ctx->error && ctx->nbytes)
    {
      /* Bytes are pending for send.  */

      /* Reset the is_empty event.  Better safe than sorry.  */
      if (!ResetEvent (ctx->is_empty))
	{
	  TRACE_ERR (ctx, GetLastError (), "ResetEvent failed");
	  _gpgrt_lock_unlock (&ctx->mutex);
	  /* FIXME: Should translate the error code.  */
	  _gpg_err_set_errno (EIO);
	  return -1;
	}
      _gpgrt_lock_unlock (&ctx->mutex);

      if (pcookie->modeflags & O_NONBLOCK)
        {
          TRACE_CTX (ctx, "would block");
          _gpg_err_set_errno (EAGAIN);
          return -1;
        }

      TRACE_CTX (ctx, "waiting for empty buffer");
      WaitForSingleObject (ctx->is_empty, INFINITE);
      TRACE_CTX (ctx, "buffer is empty");
      _gpgrt_lock_lock (&ctx->mutex);
    }

  if (ctx->error)
    {
      _gpgrt_lock_unlock (&ctx->mutex);
      if (ctx->error_code == ERROR_NO_DATA)
        _gpg_err_set_errno (EPIPE);
      else
        _gpg_err_set_errno (EIO);
      return -1;
    }

  /* If no error occurred, the number of bytes in the buffer must be
     zero.  */
  assert (!ctx->nbytes);

  if (count > WRITEBUF_SIZE)
    count = WRITEBUF_SIZE;
  memcpy (ctx->buffer, buffer, count);
  ctx->nbytes = count;

  /* We have to reset the is_empty event early, because it is also
     used by the select() implementation to probe the channel.  */
  if (!ResetEvent (ctx->is_empty))
    {
      TRACE_ERR (ctx, GetLastError (), "ResetEvent failed");
      _gpgrt_lock_unlock (&ctx->mutex);
      /* FIXME: Should translate the error code.  */
      _gpg_err_set_errno (EIO);
      return -1;
    }
  if (!SetEvent (ctx->have_data))
    {
      TRACE_ERR (ctx, GetLastError (), "SetEvent failed");
      _gpgrt_lock_unlock (&ctx->mutex);
      /* FIXME: Should translate the error code.  */
      _gpg_err_set_errno (EIO);
      return -1;
    }
  _gpgrt_lock_unlock (&ctx->mutex);

  return (int) count;
}


int
_gpgrt_w32_poll (gpgrt_poll_t *fds, size_t nfds, int timeout)
{
  HANDLE waitbuf[MAXIMUM_WAIT_OBJECTS];
  int waitidx[MAXIMUM_WAIT_OBJECTS];
  int code;
  int nwait;
  int i;
  int any;
  int count;

#if 0
 restart:
#endif

  TRACE ("poll on [ ");
  any = 0;
  nwait = 0;
  count = 0;
  for (i = 0; i < nfds; i++)
    {
      struct estream_cookie_w32_pollable *pcookie;

      if (fds[i].ignore)
	continue;

      if (fds[i].stream->intern->kind != BACKEND_W32_POLLABLE)
        {
          /* This stream does not support polling.  */
          fds[i].got_err = 1;
          continue;
        }

      pcookie = fds[i].stream->intern->cookie;

      if (fds[i].want_read || fds[i].want_write)
	{
          /* XXX: What if one wants read and write, is that supported?  */
	  if (fds[i].want_read)
	    {
	      struct reader_context_s *ctx = pcookie->reader;
              TRACE ("%d/read ", i);
              if (ctx == NULL)
                {
                  pcookie->reader = ctx = create_reader (pcookie);
                  if (!ctx)
                    {
                      /* FIXME:  Is the error code appropriate?  */
                      _gpg_err_set_errno (EBADF);
                      return -1;
                    }
                }

              if (nwait >= DIM (waitbuf))
                {
                  TRACE ("oops ]: Too many objects for WFMO!\n");
                  /* FIXME: Should translate the error code.  */
                  _gpg_err_set_errno (EIO);
                  return -1;
                }
              waitidx[nwait] = i;
              waitbuf[nwait++] = ctx->have_data_ev;
	      any = 1;
            }
	  else if (fds[i].want_write)
	    {
	      struct writer_context_s *ctx = pcookie->writer;
              TRACE ("%d/write ", i);
              if (ctx == NULL)
                {
                  pcookie->writer = ctx = create_writer (pcookie);
                  if (!ctx)
                    {
                      /* FIXME:  Is the error code appropriate?  */
                      _gpg_err_set_errno (EBADF);
                      return -1;
                    }
                }

              if (nwait >= DIM (waitbuf))
                {
                  TRACE ("oops ]: Too many objects for WFMO!");
                  /* FIXME: Should translate the error code.  */
                  _gpg_err_set_errno (EIO);
                  return -1;
                }
              waitidx[nwait] = i;
              waitbuf[nwait++] = ctx->is_empty;
	      any = 1;
            }
        }
    }
  TRACE ("]\n");
  if (!any)
    return 0;

  code = WaitForMultipleObjects (nwait, waitbuf, 0,
                                 timeout == -1 ? INFINITE : timeout);
  if (code >= WAIT_OBJECT_0 && code < WAIT_OBJECT_0 + nwait)
    {
      /* This WFMO is a really silly function: It does return either
	 the index of the signaled object or if 2 objects have been
	 signalled at the same time, the index of the object with the
	 lowest object is returned - so and how do we find out how
	 many objects have been signaled???.  The only solution I can
	 imagine is to test each object starting with the returned
	 index individually - how dull.  */
      any = 0;
      for (i = code - WAIT_OBJECT_0; i < nwait; i++)
	{
	  if (WaitForSingleObject (waitbuf[i], 0) == WAIT_OBJECT_0)
	    {
	      assert (waitidx[i] >=0 && waitidx[i] < nfds);
              /* XXX: What if one wants read and write, is that
                 supported?  */
              if (fds[waitidx[i]].want_read)
                fds[waitidx[i]].got_read = 1;
              else if (fds[waitidx[i]].want_write)
                fds[waitidx[i]].got_write = 1;
	      any = 1;
	      count++;
	    }
	}
      if (!any)
	{
	  TRACE ("no signaled objects found after WFMO\n");
	  count = -1;
	}
    }
  else if (code == WAIT_TIMEOUT)
    TRACE ("WFMO timed out\n");
  else if (code == WAIT_FAILED)
    {
      TRACE_ERR (NULL, GetLastError (), "WFMO failed");
#if 0
      if (GetLastError () == ERROR_INVALID_HANDLE)
	{
	  int k;
	  int j = handle_to_fd (waitbuf[i]);

	  TRACE ("WFMO invalid handle %d removed\n", j);
	  for (k = 0 ; k < nfds; k++)
	    {
	      if (fds[k].fd == j)
		{
		  fds[k].want_read = fds[k].want_write = 0;
		  goto restart;
                }
            }
	  TRACE (" oops, or not???\n");
        }
#endif
      count = -1;
    }
  else
    {
      TRACE ("WFMO returned %d\n", code);
      count = -1;
    }

  if (count > 0)
    {
      TRACE ("poll OK [ ");
      for (i = 0; i < nfds; i++)
	{
	  if (fds[i].ignore)
	    continue;
	  if (fds[i].got_read || fds[i].got_write)
	    TRACE ("%c%d ", fds[i].want_read ? 'r' : 'w', i);
        }
      TRACE ("]\n");
    }

  if (count < 0)
    {
      /* FIXME: Should determine a proper error code.  */
      _gpg_err_set_errno (EIO);
    }

  return count;
}



/*
 * Implementation of pollable I/O on Windows.
 */

/*
 * Constructor for pollable objects.
 */
int
_gpgrt_w32_pollable_create (void *_GPGRT__RESTRICT *_GPGRT__RESTRICT cookie,
                            unsigned int modeflags,
                            struct cookie_io_functions_s next_functions,
                            void *next_cookie)
{
  estream_cookie_w32_pollable_t pcookie;
  int err;

  pcookie = _gpgrt_malloc (sizeof *pcookie);
  if (!pcookie)
    err = -1;
  else
    {
      pcookie->modeflags = modeflags;
      pcookie->next_functions = next_functions;
      pcookie->next_cookie = next_cookie;
      pcookie->reader = NULL;
      pcookie->writer = NULL;
      *cookie = pcookie;
      err = 0;
    }

  return err;
}


/*
 * Seek function for pollable objects.
 */
static int
func_w32_pollable_seek (void *cookie, gpgrt_off_t *offset, int whence)
{
  estream_cookie_w32_pollable_t pcookie = cookie;
  (void) pcookie;
  (void) offset;
  (void) whence;
  /* XXX */
  _gpg_err_set_errno (EOPNOTSUPP);
  return -1;
}


/*
 * The IOCTL function for pollable objects.
 */
static int
func_w32_pollable_ioctl (void *cookie, int cmd, void *ptr, size_t *len)
{
  estream_cookie_w32_pollable_t pcookie = cookie;
  cookie_ioctl_function_t func_ioctl = pcookie->next_functions.func_ioctl;

  if (cmd == COOKIE_IOCTL_NONBLOCK)
    {
      if (ptr)
        pcookie->modeflags |= O_NONBLOCK;
      else
        pcookie->modeflags &= ~O_NONBLOCK;
      return 0;
    }

  if (func_ioctl)
    return func_ioctl (pcookie->next_cookie, cmd, ptr, len);

  _gpg_err_set_errno (EOPNOTSUPP);
  return -1;
}


/*
 * The destroy function for pollable objects.
 */
static int
func_w32_pollable_destroy (void *cookie)
{
  estream_cookie_w32_pollable_t pcookie = cookie;

  if (cookie)
    {
      if (pcookie->reader)
        destroy_reader (pcookie->reader);
      if (pcookie->writer)
        destroy_writer (pcookie->writer);
      pcookie->next_functions.public.func_close (pcookie->next_cookie);
      _gpgrt_free (pcookie);
    }
  return 0;
}

/*
 * Access object for the pollable functions.
 */
struct cookie_io_functions_s _gpgrt_functions_w32_pollable =
  {
    {
      func_w32_pollable_read,
      func_w32_pollable_write,
      func_w32_pollable_seek,
      func_w32_pollable_destroy,
    },
    func_w32_pollable_ioctl,
  };
