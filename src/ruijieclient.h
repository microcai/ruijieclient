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

/*  mystar.h & mystar.c are writton by NetXRay */

#ifndef RUIJIECLIENT_H
#define RUIJIECLIENT_H

#ifdef HAVE_CONFIG_H
#include  <config.h>
#else
#error <please run configure>
#endif



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/in.h>
#include <net/ethernet.h>

#define MAX_MSG_LEN 1024
#define MAX_U_MSG_LEN MAX_MSG_LEN*2

#define CONF_NAME "ruijie.conf"

#define TMP_FILE "/tmp/ruijieclient_tmp"

extern char config_file[256];

typedef struct __ruijie_packet
{
  int m_pcap_no;
  /*
   * DHCP mode:
   * 0: Off
   * 1: On, DHCP before authentication
   * 2: On, DHCP after authentication
   * 3: On, DHCP after DHCP authentication and re-authentication
   *
   */

  int m_dhcpmode;
  // auth mode: 0:standard 1:Star private
  int m_authenticationMode;

  // echo interval, 0 means disable echo
  int m_echoInterval;
  // Intelligent Reconnect 0:disable, 1: enable.
  int m_intelligentReconnect;

  // fake version, e.g. "3.22"
  char *m_fakeVersion;


  char  m_name[32]; // user name
  char  m_password[32]; // password
  char 	m_nic[32]; // net adapter name

  in_addr_t m_ip;
  in_addr_t m_mask;
  in_addr_t m_gate; //Default gateway
  in_addr_t m_dns;
  in_addr_t m_pinghost;

} ruijie_packet;


#endif /* RUIJIECLIENT_H */
