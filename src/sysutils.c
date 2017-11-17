/* sysutils.c - Platform specific helper functions
 * Copyright (C) 2017 g10 Code GmbH
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
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "gpgrt-int.h"



/* Return true if FD is valid.  */
int
_gpgrt_fd_valid_p (int fd)
{
  int d = dup (fd);
  if (d < 0)
    return 0;
  close (d);
  return 1;
}
