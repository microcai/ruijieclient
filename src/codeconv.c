/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun                                          *
 \*******************************************************************************/

/*
 * Convert GB codes into UTF-8
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "codeconv.h"


int
code_convert(char *outbuf, size_t outlen, char *inbuf, size_t inlen)
{
#if defined( HAVE_ICONV_H) && defined (_ICONV_H)

  iconv_t cd;
  char **pin = &inbuf;
  char **pout = &outbuf;

  // initial iconv
  /*
   * TODO:We should judge the incoming text's encoding
   * and select the most suitable encoding.
   */
  cd = iconv_open("UTF-8", "GBK");
  if (cd == 0)
    return -1;

  memset(outbuf, '\0', outlen);
  if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
    {
      err_msg("code_convert failed.");
      return -1;
    }
  iconv_close(cd);

#endif

  return 0;
}



