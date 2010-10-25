/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun  microcai  sthots                                      *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
 *
 * AUTHORS:
 *   Gong Han  <gong AT fedoraproject.org> from CSE@FJNU CN
 *   Chen Tingjun <chentingjun AT gmail.com> from POET@FJNU CN
 *   microcai <microcai AT sina DOT com > for ZSTU
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
//#define DEBUG_NOXML
#ifdef HAVE_CONFIG_H
#include  <config.h>
#else
#error <please run configure>
#endif

#include <dlfcn.h>
#include <ifaddrs.h>

#include "ruijieclient.h"

static char fakeVersion[8];
//static char fakeMAC[32];

/*Check whether we have got enough configuration info*/
void
CheckConfig(ruijie_packet* l)
{
#define  ccerr(x) err_quit(x "\n")

  if (l->m_echoInterval == -1)
	l->m_echoInterval = 20;
  if (l->m_authenticationMode == -1)
	l->m_authenticationMode = 0;
  if (l->m_intelligentReconnect == -1)
	l->m_intelligentReconnect = 1;
  if (l->m_dhcpmode == -1)
	l->m_dhcpmode = 0;

  if( !l->m_fakeVersion)
    l->m_fakeVersion = "3.99";

  if ((l->m_name == NULL) || (l->m_name[0] == 0))
    ccerr("invalid name");
  if ((l->m_password == NULL) || (l->m_password[0] == 0))
    ccerr("invalid password");
  if ((l->m_authenticationMode < 0) || (l->m_authenticationMode > 1))
    ccerr("invalid authenticationMode");
  if ((strcmp(l->m_nic, "") == 0) || (strcmp(l->m_nic, "any") == 0))
    ccerr("invalid nic");
  if ((l->m_echoInterval < 0) || (l->m_echoInterval > 100))
    ccerr("invalid echo interval");
  if ((l->m_intelligentReconnect < 0) || (l->m_intelligentReconnect > 1))
    ccerr("invalid intelligentReconnect");

#undef ccerr

#ifdef DEBUG
  char buf[80];
  inet_ntop(AF_INET, &l->m_ip, buf, 80);

  puts("-- CONF INFO");
  printf("## m_name=%s\n", l->m_name);
  printf("## m_password=%s\n", l->m_password);
  printf("## m_nic=%s\n", l->m_nic);
  printf("## m_authenticationMode=%d\n", l->m_authenticationMode);
  printf("## m_echoInterval=%d\n", l->m_echoInterval);
  printf("## m_intelligentReconnect=%d\n", l->m_intelligentReconnect);// NOT supported now!!
  printf("## m_fakeVersion=%s\n", l->m_fakeVersion);
  printf("## m_fakeAddress=%s\n", buf);
  //    printf("## m_fakeMAC=%s\n",l->m_fakeMAC);
  puts("-- END");
#endif
}
