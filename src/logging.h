/* logging.h - Definitions for logging.c
 * Copyright (C) 1998-2001, 2003-2006, 2009-2010,
 *               2017  Free Software Foundation, Inc.
 * Copyright (C) 1998-1999, 2001-2006, 2008-2017  Werner Koch
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
 *
 * This file was originally a part of GnuPG.
 */

#ifndef _GPGRT_LOGGING_H
#define _GPGRT_LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <gpg-error.h>
#include "mischelp.h"
#include "w32help.h"

int  log_get_errorcount (int clear);
void log_inc_errorcount (void);
void log_set_file( const char *name );
void log_set_fd (int fd);
void log_set_socket_dir_cb (const char *(*fnc)(void));
void log_set_pid_suffix_cb (int (*cb)(unsigned long *r_value));
void log_set_prefix (const char *text, unsigned int flags);
const char *log_get_prefix (unsigned int *flags);
int log_test_fd (int fd);
int  log_get_fd(void);
estream_t log_get_stream (void);

#ifdef GPGRT_HAVE_MACRO_FUNCTION
  void bug_at (const char *file, int line, const char *func)
               GPGRT_ATTR_NORETURN;
  void _log_assert (const char *expr, const char *file, int line,
                    const char *func) GPGRT_ATTR_NORETURN;
# define BUG() bug_at( __FILE__ , __LINE__, __FUNCTION__)
# define log_assert(expr)                                       \
  ((expr)                                                       \
   ? (void) 0                                                   \
   : _log_assert (#expr, __FILE__, __LINE__, __FUNCTION__))
#else /*!GPGRT_HAVE_MACRO_FUNCTION*/
  void bug_at (const char *file, int line);
  void _log_assert (const char *expr, const char *file, int line);
# define BUG() bug_at( __FILE__ , __LINE__ )
# define log_assert(expr)                                       \
  ((expr)                                                       \
   ? (void) 0                                                   \
   : _log_assert (#expr, __FILE__, __LINE__))
#endif /*!GPGRT_HAVE_MACRO_FUNCTION*/

/* Flag values for log_set_prefix. */
#define GPGRT_LOG_WITH_PREFIX  1
#define GPGRT_LOG_WITH_TIME    2
#define GPGRT_LOG_WITH_PID     4
#define GPGRT_LOG_RUN_DETACHED 256
#define GPGRT_LOG_NO_REGISTRY  512

/* Log levels as used by log_log.  */
enum jnlib_log_levels {
    GPGRT_LOG_BEGIN,
    GPGRT_LOG_CONT,
    GPGRT_LOG_INFO,
    GPGRT_LOG_WARN,
    GPGRT_LOG_ERROR,
    GPGRT_LOG_FATAL,
    GPGRT_LOG_BUG,
    GPGRT_LOG_DEBUG
};
void log_log (int level, const char *fmt, ...) GPGRT_ATTR_PRINTF(2,3);
void log_logv (int level, const char *fmt, va_list arg_ptr);
void log_logv_with_prefix (int level, const char *prefix,
                           const char *fmt, va_list arg_ptr);
void log_string (int level, const char *string);
void log_bug (const char *fmt, ...)    GPGRT_ATTR_NR_PRINTF(1,2);
void log_fatal (const char *fmt, ...)  GPGRT_ATTR_NR_PRINTF(1,2);
void log_error (const char *fmt, ...)  GPGRT_ATTR_PRINTF(1,2);
void log_info (const char *fmt, ...)   GPGRT_ATTR_PRINTF(1,2);
void log_debug (const char *fmt, ...)  GPGRT_ATTR_PRINTF(1,2);
void log_debug_with_string (const char *string, const char *fmt,
                            ...) GPGRT_ATTR_PRINTF(2,3);
void log_printf (const char *fmt, ...) GPGRT_ATTR_PRINTF(1,2);
void log_flush (void);

/* Print a hexdump of BUFFER.  With TEXT passes as NULL print just the
   raw dump, with TEXT being an empty string, print a trailing
   linefeed, otherwise print an entire debug line with TEXT followed
   by the hexdump and a final LF.  */
void log_printhex (const char *text, const void *buffer, size_t length);

void log_clock (const char *string);


#endif /*_GPGRT_LOGGING_H*/
