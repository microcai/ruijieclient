/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun                                          *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
 *
 * Many thanks to netxray@byhh
 *
 * AUTHORS:
 *   Gong Han  <gong AT fedoraproject.org> from CSE@FJNU CN
 *   Chen Tingjun <chentingjun AT gmail.com> from POET@FJNU CN
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* This file comes from APUE, slightly edited by NetXRay@byhh */

#include "myerr.h"

static void
err_doit(int, const char *, va_list);

/* Nonfatal error related to a system call.
 * Print a message and return. */
void
err_ret(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  return;
}

/* Fatal error related to a system call.
 * Print a message and terminate. */
void
err_sys(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */
void
err_dump(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(1, fmt, ap);
  va_end(ap);
  abort(); /* dump core and terminate */
  exit(EXIT_FAILURE); /* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */
void
err_msg(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, fmt, ap);
  va_end(ap);
  return;
}

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */
void
err_quit(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

/* Print a message and return to caller.
 * Caller specifies "errnoflag". */
static void
err_doit(int errnoflag, const char *fmt, va_list ap)
{
  int errno_save;
  char buf[4096];

  errno_save = errno; /* value caller might want printed */
  vsprintf(buf, fmt, ap);
  if (errnoflag)
    sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
  strcat(buf, "\n");
  fflush(stdout); /* in case stdout and stderr are the same */
  fputs(buf, stderr);
  fflush(NULL); /* flushes all stdio output streams */
  return;
}
