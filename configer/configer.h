/*
 * configer.h : header file for all the config parameter op
 *
 *  Created on: 2009-12-11
 *      Author: microcai <microcai AT sina.com > from ZSTU
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
 *
 */
#ifndef CONFIGER_H_
#define CONFIGER_H_

#include <netinet/in.h>
/***************************************************
 * The config method has the following override rules
 * cli > dbus > ~(xml/ini) > /etc(xml/ini) *
 ****************************************************/

struct ruijieclient_config_data{
  char          name[64];
  char          passwd[64];
  char          nic[16];
  char          fakeversion[16];
  int           authmode;
#define RUIJIE_AUTH_STD         0x000000
#define RUIJIE_AUTH_PRIVATE     0x000100
#define RUIJIE_DHCP_OFF         0
#define RUIJIE_DHCP_ON          0x000001
#define RUIJIE_REAUTH_DUCP      0x010000
  int           fakemask;
#define RUIJIE_HASFAKE_IP       0x00000001
#define RUIJIE_HASFAKE_MASK     0x00000002
#define RUIJIE_HASFAKE_ROUTE    0x00000004
#define RUIJIE_HASFAKE_DNS      0x00000008
#define RUIJIE_HASFAKE_VERSION  0x00000010
  in_addr_t     fakeip;
  in_addr_t     fakeaddrmask;
  in_addr_t     fakeroute;
  in_addr_t     fakedns;

  int           validmask;
#define RUIJIE_VALID_PINGHOST   0x00000001
#define RUIJIE_VALID_CTRLFILE   0x00000002
#define RUIJIE_VALID_ECHOINTERVAL 0x00000004
  in_addr_t     pinghost;
  int           ctrl_file; // use pipe file to control
  int           echo_interval;
};

#endif /* CONFIGER_H_ */

