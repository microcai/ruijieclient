/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun                                          *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
 * We just add something to make it more convinence.
 *
 * AUTHORS:
 *   Gong Han  <gong AT fedoraproject.org> from CSE@FJNU CN
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

#ifndef CONN_MONITOR_H
#define CONN_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 8192

struct route_info
{
  in_addr_t dstAddr;
  in_addr_t srcAddr;
  in_addr_t gateWay;
  char ifName[IF_NAMESIZE];
};

/* Ping the address to check connectivity of the network */
int
Ping(in_addr_t host_addr);

//int
//readNlSock(int sockFd, char *bufPtr, int seqNum, int pId);

/* analyse and return route information */
//void
//parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo);

/* get default gateway as a string */
in_addr_t
get_gateway();

#endif
