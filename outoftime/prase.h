/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) microcai (microcai@sina.com)             *
 \*******************************************************************************/

/*
 * prase.h
 *
 * AUTHORS:
 *   microcai <microcai@sina.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef PRASE_H_
#define PRASE_H_ 1

#ifdef __cplusplus
extern "C"
  {
#endif

enum parameter_types
{
  STUB, // just a stub.
  BOOL_short, // just supply or not to set flag
  BOOL_long, // must use =yes or =no
  BOOL_both, // please always use these one
  INTEGER, // parameter is a integer
  STRING, // parameter is a string.
  FUNCTION
// parameter is a call back function
};

struct parameter_tags
{
  const char * const prefix;
  const char * const parameter;
  const char * const discribe;
  const long parameter_len;
  const int prefix_len;
  const enum parameter_types type;
};
void
ParseParameters(int * argc, char ** argv[], struct parameter_tags p_[]);

#ifdef __cplusplus
}
#endif

#endif /* PRASE_H_ */
