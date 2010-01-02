/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun                                          *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <sys/poll.h>
#include "conn_monitor.h"
#include <netinet/ip_icmp.h>
#ifdef HAVE_LINUX_RTNETLINK_H
#include <linux/rtnetlink.h>
#endif

static u_int16_t checksum(u_int16_t *buffer, int size)
{
    uint32_t cksum = 0;

    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof ( unsigned short);
    }

    if (size)
        cksum += *buffer;

    cksum = (cksum >> 16) + (cksum & 0xffff);

    cksum += (cksum >> 16);

    return (uint16_t) (~cksum);
}

int
Ping(in_addr_t host_addr)
{
  struct sockaddr_in ad;

  ad.sin_family = AF_INET;
  ad.sin_port = 0;
  bzero(ad.sin_zero,sizeof(ad.sin_zero));
  ad.sin_addr.s_addr = host_addr;

  /*create a raw socket for sending and receiving ping/echo*/
  int sk = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
  struct icmp  echo;
  memset(&echo,0,sizeof(echo));
  echo.icmp_type = ICMP_ECHO;


  echo.icmp_cksum = checksum((uint16_t*)&echo,sizeof(echo));

  sendto(sk,&echo,sizeof(echo),0,(struct sockaddr*)&ad, INET_ADDRSTRLEN);

  struct pollfd pfd;
  pfd.fd = sk;
  pfd.events = POLLIN;
  switch(poll(&pfd,1,2000)) // 2 秒够的吧
  {
    case 0:
    case -1:
    close(sk);
#ifdef DEBUG
    puts("## pingd timed out\n");
#endif
    return -1;
    case 1:
#ifdef DEBUG
      {
        struct in_addr ip;
        ip.s_addr = host_addr;
        fprintf(stdout,"##ping %s alive\n",inet_ntoa(ip));
      }
#endif
      close(sk);
      return 0;
  }
  return -1;
}

#ifdef HAVE_LINUX_RTNETLINK_H

static int
readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
  struct nlmsghdr *nlHdr;
  int readLen = 0, msgLen = 0;
  do
    {
      /* Relieved response from kernel */
      if ((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0)
        {
          perror("SOCK READ: ");
          return -1;
        }

      nlHdr = (struct nlmsghdr *) bufPtr;
      /* check validation of header */
      if ((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))
        {
          perror("Error in recieved packet");
          return -1;
        }

      /* Check if the its the last message */
      if (nlHdr->nlmsg_type == NLMSG_DONE)
        {
          break;
        }
      else
        {
          /* Else move the pointer to buffer appropriately */
          bufPtr += readLen;
          msgLen += readLen;
        }

      /* Check if its a multi part message */
      if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0)
        {
          /* return if its not */
          break;
        }
    }
  while ((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
  return msgLen;
}

// analyse and return route information
static int
parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo)
{
  struct rtmsg *rtMsg;
  struct rtattr *rtAttr;
  int rtLen;
  char *tempBuf = NULL;

  struct in_addr dst;
  struct in_addr gate;

  tempBuf = (char *) malloc(100);
  rtMsg = (struct rtmsg *) NLMSG_DATA(nlHdr);
  // If the route is not for AF_INET or does not belong to main routing table
  //then return.
  if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
    return 0;
  /* get the rtattr field */
  rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
  rtLen = RTM_PAYLOAD(nlHdr);
  for (;RTA_OK(rtAttr,rtLen); rtAttr = RTA_NEXT(rtAttr,rtLen))
    {
      switch (rtAttr->rta_type)
        {
      case RTA_OIF:
        if_indextoname(*(int *) RTA_DATA(rtAttr), rtInfo->ifName);
        break;
      case RTA_GATEWAY:
        rtInfo->gateWay = *(u_int *) RTA_DATA(rtAttr);
        break;
      case RTA_PREFSRC:
        rtInfo->srcAddr = *(u_int *) RTA_DATA(rtAttr);
        break;
      case RTA_DST:
        rtInfo->dstAddr = *(u_int *) RTA_DATA(rtAttr);
        break;
        }
    }
  //2007-12-10
  dst.s_addr = rtInfo->dstAddr;
  if (strstr((char *) inet_ntoa(dst), "0.0.0.0"))
    {
      gate.s_addr = rtInfo->gateWay;
#ifdef DEBUG
      printf("## gateway: %s\n", (char *) inet_ntoa(gate));
#endif
      return 1;
    }
  free(tempBuf);
  return 0;
}
#endif

in_addr_t
get_gateway()
{

  in_addr_t gateway=0;

#ifdef HAVE_LINUX_RTNETLINK_H

  struct nlmsghdr *nlMsg;
  struct rtmsg *rtMsg;
  struct route_info *rtInfo;
  char msgBuf[BUFSIZE];


  int sock, len, msgSeq = 0;

  /* create Socket */
  if ((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
    {
      perror("Socket Creation: ");
      return -1;
    }

  /* Initialize the buffer */
  memset(msgBuf, 0, BUFSIZE);

  /* point the header and the msg structure pointers into the buffer */
  nlMsg = (struct nlmsghdr *) msgBuf;
  rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

  /* Fill in the nlmsg header*/
  nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
  nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .

  nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
  nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
  nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.

  /* Send the request */
  if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0)
    {
      printf("Write To Socket Failed...\n");
      return -1;
    }

  /* Read the response */
  if ((len = readNlSock(sock, msgBuf, msgSeq, getpid())) < 0)
    {
      printf("Read From Socket Failed...\n");
      return -1;
    }
  /* Parse and print the response */
  rtInfo = (struct route_info *) malloc(sizeof(struct route_info));
  for (;NLMSG_OK(nlMsg,len); nlMsg = NLMSG_NEXT(nlMsg,len))
    {
      memset(rtInfo, 0, sizeof(struct route_info));
      if (parseRoutes(nlMsg, rtInfo))
        {
          gateway = rtInfo->gateWay;
          break;
        }
    }
  free(rtInfo);
  close(sock);
#endif
  return gateway;
  //TODO: add support for non-Linux users

}

