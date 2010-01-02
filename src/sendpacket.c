/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun  microcai                                *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
 *
 * Many thanks to netxray@byhh
 *
 * AUTHORS:
 *   Gong Han  <gong AT fedoraproject.org> from CSE@FJNU CN
 *   Chen Tingjun <chentingjun AT gmail.com> from POET@FJNU CN
 *   microcai <microcai AT sina.com > from ZSTU
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


#include "sendpacket.h"
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/route.h>
#include <poll.h>
#include <errno.h>
#include <dlfcn.h>
#include "md5.h"
#include "blog.h"// broadcast packet for finding server


// echo packet incorporating user name and MD5 sum
static
uint8_t ackPackage[0x3E8] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,        // Destination MAC
  0x00,0x00,0x00,0x00,0x00,0x00,        // Source MAC
  0x88,0x8E,                            // Ethertype = 0x888E (8021X)
  0x01,                                 // Version = 1
  0x00,                                 // Packet Type 0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04
  0x00,0x00,                            // Packet Body Length
  0x02,                                 // Response
  0x00,                                 // id
  0x00,0x00,                            // length
  0x00                                  // type  1 identify  4 MD5-Challenge
};


//Ruijie OEM Extra （V2.56）  by soar
static uint8_t RuijieExtra[] = {
// OEM Extra
// 0 --> 22
  0xff,0xff,0x37,0x77, // Encode( 0x00,0x00,0x13,0x11 )
                       // 求反并头尾颠倒.add by lsyer
  0xff,                // Encode( 0x01/00  EnableDHCP flag )
                       // 0xff:Static IP  0x3f:DHCP
  0x00,0x00,0x00,0x00, // Encode( IP )
  0x00,0x00,0x00,0x00, // Encode( SubNetMask )
  0x00,0x00,0x00,0x00, // Encode( NetGate )
  0x00,0x00,0x00,0x00, // Encode( DNS )
  0x00,0x00,           // Checksum( )
// 23 --> 58
// ASCII 8021x.exe
  0x00,0x00,0x13,0x11,0x38,0x30,0x32,0x31,0x78,0x2E,0x65,0x78,0x65,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,
// 59 --> 62
  0x00,0x00,0x00,0x02, // 8021x.exe File Version (2.56.00)
  //63                 // base16 code.add by lsyer
  0x00,                // unknow flag
  //64 --> 77     // 01 0x33
  0x00,0x00,0x13,0x11,0x00,0x28,0x1A,0x28,0x00,0x00,0x13,0x11,0x17,0x22,// Const strings
   // 78 --> 109
  // 32bits spc. Random strings
  0x46,0x38,0x42,0x35,0x32,0x42,0x42,0x37,0x44,0x38,0x32,0x36,0x41,0x34,0x33,0x32,
//    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  // 32bits spc. Random strings
  0x34,0x43,0x30,0x30,0x30,0x33,0x38,0x38,0x34,0x39,0x38,0x36,0x33,0x39,0x66,0x34,
//  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,

  //110 --> 120
  0x1a,0x0c, // unknow flag
  0x00,0x00,0x13,0x11,0x18,0x06,0x00,0x00,0x00,// Const strings
  // 121
  0x00,               // DHCP and first time flag

  // V2.56 (and upper?) added
  // 122 --> 129
  0x1A,0x0E,0x00,0x00,0x13,0x11,0x2D,0x08,  // Const strings

  // 130 --> 143
  0x00,0x00,0x00,0x00,0x00,0x00,            // True NIC MAC
  0x1A,0x08,0x00,0x00,0x13,0x11,0x2F,0x02,  // Const strings

  //u_int8_t ruijie_new_extra[]=
  //144 --> 151
  0x1A,0x09,0x00,0x00,0x13,0x11,0x35,0x03,  // Const strings
  //152
  0x00, //unknow
  //153 --> 160
  0x1a,0x18,0x00,0x00,0x13,0x11,0x36,0x12,   // Const strings
  //161 --> 176
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //177 --> 183
  0x1A,0x18,0x00,0x00,0x13,0x11,0x38,0x12,  // Const strings
  //184 --> 199
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  //200 --> 207
  0x1A,0x18,0x00,0x00,0x13,0x11,0x4d,0x82,  // Const strings


/************************************************************************************
 WE DON'T KNOW IT
 ************************************************************************************/

  //208 --> 239
  // 32bits spc. Random strings
  0x30,0x66,0x38,0x62,0x35,0x32,0x62,0x62,0x30,0x37,0x64,0x38,0x30,0x32,0x36,0x61,
//0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  // 32bits spc. Random strings
  0x34,0x33,0x32,0x34,0x33,0x35,0x30,0x66,0x33,0x32,0x62,0x37,0x33,0x30,0x37,0x34,
//  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  //240
  0x34,0x30,0x33,0x61,0x36,0x65,0x35,0x39,0x32,0x32,0x37,0x62,0x30,0x36,0x38,0x61,
//0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  // 32bits spc. Random strings
  0x36,0x35,0x32,0x64,0x34,0x34,0x34,0x32,0x32,0x33,0x62,0x34,0x30,0x36,0x33,0x63,
//  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  //272
  0x33,0x37,0x33,0x63,0x34,0x38,0x65,0x31,0x35,0x61,0x62,0x32,0x37,0x32,0x65,0x66,
//    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  // 32bits spc. Random strings
//  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  //304
  0x31,0x62,0x35,0x36,0x30,0x30,0x33,0x61,0x36,0x32,0x66,0x38,0x34,0x33,0x36,0x64,
//    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  // 32bits spc. Random strings
  0x31,0x31,0x32,0x63,0x37,0x66,0x30,0x34,0x30,0x32,0x65,0x65,0x35,0x36,0x63,0x64,
//  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
  0x32,0x34,0x38,0x62,0x36,0x36,0x62,0x30,0x34,0x37,0x64,0x36,0x37,0x30,0x39,0x37,
  //336
  0x1A,0x28,0x00,0x00,0x13,0x11,0x39,0x22   // Const strings

  //352
};


int
FillVersion(ruijie_packet * this)
{
  unsigned int c_ver1, c_ver2;

  if (this->m_fakeVersion && sscanf(this->m_fakeVersion, "%u.%u", &c_ver1, &c_ver2))
    {
#ifdef DEBUG
      printf("## c_ver1=%u ## c_ver2=%u\n", c_ver1, c_ver2);
#endif
      RuijieExtra[0x3B] = c_ver1;
      RuijieExtra[0x3C] = c_ver2;
      if( c_ver1 > 3 && c_ver2 > 50 &&  this->m_state != 2 )
        {
          this->m_init_Echo_Key = htonl(0x0000102b);
        }
      if( c_ver1 > 3 && c_ver2 > 50 ) // > 3.5
        {
          RuijieExtra[68] = 0x01;
          RuijieExtra[69] = 0x33;
        }
      return 0;
    }
  else
    {
      return -1;
    }
}

/* comment out for further usage
 int
 FillFakeMAC(unsigned char * fMAC, char * m_fakeMAC)
 {

 #ifdef DEBUG
 int i;
 #endif

 if (m_fakeMAC && sscanf(m_fakeMAC, "%x:%x:%x:%x:%x:%x", &fMAC[0],
 &fMAC[1], &fMAC[2], &fMAC[3], &fMAC[4], &fMAC[5]))
 {
 #ifdef DEBUG
 printf("## MAC =");
 for (i = 0; i < 6; i++)
 printf(" %2X ", fMAC[i]);
 putchar('\n');
 #endif
 */
#ifdef USE_DYLIB
void *
FindLibPcap()
{
  char libpcap[32];
  void *p;
  p = dlopen("libpcap.so", RTLD_LAZY);
  if (p)
    return p;
  int version;
  for (version = 9; version > 0; --version)
    {
      sprintf(libpcap, "libpcap.so.0.%d", version);
      p = dlopen(libpcap, RTLD_LAZY);
      if (p)
        break;
    }
  if (p)
    return p;
  err_quit("Cannot load libpcap.so. Please install libpcap package\n");
}
#endif

void init_ruijie_packet(ruijie_packet*this)
{
  memset(this,0,sizeof(ruijie_packet));
  // Initialize non-zero variable.
  this->m_echoInterval = this->m_intelligentReconnect = this->m_authenticationMode =this->m_dhcpmode = -1;
  this->m_init_Echo_Key = htonl(0x1b8b4563);
#ifdef USE_DYLIB

  //search libpcap.so.* files
  this->libpcap= FindLibPcap();

  //Get the number
  this->pcap_open_live = dlsym(this->libpcap,"pcap_open_live");
#define pcap_open_live(z,x,c,v,b) this->pcap_open_live(z,x,c,v,b)

  this->pcap_fileno = dlsym(this->libpcap,"pcap_fileno");
#define pcap_fileno(x) this->pcap_fileno(x)

  this->pcap_geterr = dlsym(this->libpcap,"pcap_geterr");
#define pcap_geterr(x) this->pcap_geterr(x)

  this->pcap_compile = dlsym(this->libpcap,"pcap_compile");
#define pcap_compile(z,x,c,v,b) this->pcap_compile(z,x,c,v,b)

  this->pcap_setfilter = dlsym(this->libpcap,"pcap_setfilter");
#define pcap_setfilter(x,c) this->pcap_setfilter(x,c)

  this->pcap_freecode = dlsym(this->libpcap,"pcap_freecode");
#define pcap_freecode(x) this->pcap_freecode(x)

  this->pcap_next_ex = dlsym(this->libpcap,"pcap_next_ex");
#define pcap_next_ex(z,x,c) this->pcap_next_ex(z,x,c)

  this->pcap_sendpacket = dlsym(this->libpcap,"pcap_sendpacket");
#define pcap_sendpacket(z,x,c) this->pcap_sendpacket(z,x,c)

  this->pcap_close = dlsym(this->libpcap,"pcap_close");
#define pcap_close(x) this->pcap_close(x)

#endif
}

int
WaitPacket(ruijie_packet *this, int timeout)
{
  struct pollfd pfd;
  pfd.fd = this->m_pcap_no;
  pfd.events = POLLIN;

  return poll(&pfd, 1, timeout);
}


#define FILTER_STR "ether[12:2]=0x888e and ether dst %02x:%02x:%02x:%02x:%02x:%02x"

int
GetNicParam(ruijie_packet *this)
{
  char filter_buf[256];
  char p_errbuf[PCAP_ERRBUF_SIZE];

  struct bpf_program filter_code;
  struct ifreq rif;

  memset(&rif, 0, sizeof(rif));

  // retrieve MAC address of corresponding net adapter's
  strcpy(rif.ifr_name, this->m_nic);
  int tmp = socket(AF_INET, SOCK_DGRAM, 0);

  if ((this->m_ip == 0 && this->m_dhcpmode == 0) || this->m_state == 2)
    {
      ioctl(tmp, SIOCGIFADDR, &rif);
//      memcpy(&(this->m_ip), ifaddr->ifa_addr->sa_data + 2, 4);
      memcpy(&(this->m_ip), rif.ifr_addr.sa_data + 2, 4);
    }
  // else m_ip has been initialized in SetConfig()

  if (this->m_dhcpmode==0 ||this->m_state==2 )
    {
      ioctl(tmp, SIOCGIFNETMASK, &rif);
      memcpy(&(this->m_mask), rif.ifr_addr.sa_data + 2, 4);
    }
  else
    {
      this->m_mask = inet_addr("255.255.255.0");
    }
  #ifdef SIOCGIFHWADDR
  // Linux, Solaris, etc

  ioctl(tmp, SIOCGIFHWADDR, &rif);
//  memcpy(this->m_ETHHDR + ETHER_ADDR_LEN,ifaddr->ifa_addr->sa_data,
    //    ETHER_ADDR_LEN);

  memcpy(this->m_ETHHDR + ETHER_ADDR_LEN, rif.ifr_hwaddr.sa_data,
      ETHER_ADDR_LEN);

  #else

  // BSD, Mac OS X
  struct ifaddrs *ifap;
  const struct ifaddrs *p;

  if (!getifaddrs(&ifap))
    {
      for (p = ifap; p; p = p->ifa_next)
        {
          if (p->ifa_name && p->ifa_name[0] && !strcmp(
              (const char*) p->ifa_name, this->m_nic))
            {

              const struct sockaddr_dl * sdl = (struct sockaddr_dl*)p->ifa_addr;
              //::memset(macaddr, 0, sizeof macaddr);
              memcpy(this->m_ETHHDR + ETHER_ADDR_LEN, sdl->sdl_data + sdl->sdl_nlen,6);
              break;
            }
        }
      freeifaddrs(ifap);
    }
#endif

  memcpy(RuijieExtra + 130, this->m_ETHHDR + ETHER_ADDR_LEN, 6);

  close(tmp);

  if (this->m_pcap)
    pcap_close(this->m_pcap);
  if ((this->m_pcap = pcap_open_live(this->m_nic, 65536, 0, 500, p_errbuf))
      == NULL)
    {
      err_msg("pcap_open_live: %s\n", p_errbuf);

      return 1;
    }
  this->m_pcap_no = pcap_fileno(this->m_pcap); // we can poll() it in the following code.

  // set the filter. Here I'm sure filter_buf is big enough.
  snprintf(filter_buf, sizeof(filter_buf), FILTER_STR, this->m_ETHHDR[6],
      this->m_ETHHDR[7], this->m_ETHHDR[8], this->m_ETHHDR[9],
      this->m_ETHHDR[10], this->m_ETHHDR[11]);

  if (pcap_compile(this->m_pcap, &filter_code, filter_buf, 0, this->m_mask)
      == -1)
    {
      err_msg("pcap_compile(): %s", pcap_geterr(this->m_pcap));
      pcap_close(this->m_pcap);
      return 1;
    }
  if (pcap_setfilter(this->m_pcap, &filter_code) == -1)
    {
      err_msg("pcap_setfilter(): %s", pcap_geterr(this->m_pcap));
      pcap_close(this->m_pcap);
      return 1;
    }
  pcap_freecode(&filter_code); // avoid  memory-leak

  return 0;
}

int
SendFindServerPacket(ruijie_packet *this)
{
  uint8_t BoradCastAddr[2][6] =
    {
      { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x03 },
      { 0x01, 0xD0, 0xF8, 0x00, 0x00, 0x03 }
    };


  static
  uint8_t broadPackage[0x3E8] =
  {
    0x00,0x00,0x00,0x00,0x00,0x00,        // Destination MAC
    0x00,0x00,0x00,0x00,0x00,0x00,        // Source MAC
    0x88,0x8E,                            // Ethertype = 0x888E (8021X)
    0x01,                                 // Version = 1
    0x01,                                 // Packet Type  0x00
                                          // 0x01,EAPOL-Start
                                          // 0x02 ;0x03 ;0x04
    0x00,0x00,                            // Packet Body Length
  };

  int i;
  if (!this->m_state)
    memcpy(broadPackage, BoradCastAddr[this->m_authenticationMode], 6);

  memcpy(broadPackage + ETH_ALEN, this->m_ETHHDR + ETH_ALEN, ETH_ALEN); // fill local MAC

  this->m_ruijieExtra = RuijieExtra;

  // check blog.c and bloc.h for details
  InitializeBlog(this);
  FillNetParamater(this);
  this->m_gate = get_gateway();
  if(this->m_pinghost==0)
    this->m_pinghost = this->m_gate;

  memcpy(broadPackage + 18, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Searching for server...\n", stdout);

  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, broadPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0: // timeout
        continue; // retry
      case -1:
        break;
      default: // it should be 1
        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 0x01 && this->pkt_data[0x16] == 0x01)
          {
            // get server's MAC address.
            memcpy(this->m_ETHHDR, this->pkt_data + ETH_ALEN, ETH_ALEN);
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];

            return 0;
          }
        continue;
        }
    }
  return -1;
}

int
SendNamePacket(ruijie_packet *this)
{
  //  extern char *m_name;
  int nameLen;

  nameLen = strlen(this->m_name);
  memcpy(ackPackage, this->m_ETHHDR, 12); // fill destined MAC and local MAC

  ackPackage[0x13] = this->m_lastID; //id, HERE as if it's alway 1 from ShiDa ??
  *(short *) (ackPackage + 0x10) = htons((short) (5 + nameLen));// length
  *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);// length
  ackPackage[0x16] = 0x01; //Type: Identify
  memcpy(ackPackage + 0x17, this->m_name, nameLen); // fill name

  memcpy(ackPackage + 0x17 + nameLen, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Sending user name...\n", stdout);
  int i;
  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, ackPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0: // timeout
        continue; // retry
      case -1:
        break;
      default: // it should be 1

        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 0x01 && this->pkt_data[0x16] == 0x04)
          {
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];
            this->m_MD5value_len = this->pkt_data[0x17];
            memcpy(this->m_MD5value, this->pkt_data + 0x18,
                this->m_MD5value_len);
            return 0;
          }
        continue;
        }
    }
  return -1;
}

int
SendPasswordPacket(ruijie_packet *this)
{

  // msg offset
  u_int16_t offset;

  unsigned char md5Data[256]=""; // password,md5 buffer
  unsigned char md5Dig[32]; // result of md5 sum
  int md5Len = 0;

  int nameLen, passwordLen;

  nameLen = strlen(this->m_name);
  passwordLen = strlen(this->m_password);

  memcpy(ackPackage, this->m_ETHHDR, 12);// fill destined MAC and local MAC

  ackPackage[0x13] = this->m_lastID; //id

  ackPackage[0x16] = 0x04; // Type: MD5-Challenge

  *(short *) (ackPackage + 0x10) = htons((short) (22 + nameLen)); // length
  *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);

  md5Data[md5Len++] = this->m_lastID;//ID
  memcpy(md5Data + md5Len, this->m_password, passwordLen);
  md5Len += passwordLen; // password

  memcpy(md5Data + md5Len, this->m_MD5value, this->m_MD5value_len);
  md5Len += this->m_MD5value_len; // private key

//  char d[256]="";
//  sprintf(d,"$1$%s$",this->m_password);
//  memcpy(d + 4 + passwordLen,this->m_MD5value,this->m_MD5value_len);
//  char * pp = (char*)crypt(md5Data,d);

  Computehash(md5Data, md5Len,md5Dig);

  ackPackage[0x17] = 16; // length of md5 sum is always 16.
  memcpy(ackPackage + 0x18, md5Dig, 16);
//  memcpy(ackPackage + 0x18, pp, 16);

  memcpy(ackPackage + 0x28, this->m_name, nameLen);

  memcpy(ackPackage + 0x28 + nameLen, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Sending password... \n", stdout);
  int i;
  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, ackPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0:
        // timeout
        continue; // retry
      case -1:
        break;
      default:
        // it should be 1
        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 3)
        // if succeed in sending password
          {
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];

            //get 心跳信息初始码
            //uTemp.ulValue = *(((u_long *)(pkt_data+0x9d)));
            offset = ntohs(*((u_int16_t*) (this->pkt_data + 0x10)));
            union
            {
              u_int32_t l;
              u_char s[4];
            } tmp;

//            printf("##key is %x %x %x %x",this->pkt_data[offset + 0x9],this->pkt_data[offset + 0xa],this->pkt_data[offset + 0xb],this->pkt_data[offset + 0xc]);

            tmp.s[0] = Alog(this->pkt_data[offset + 0x9]);//0xff
            tmp.s[1] = Alog(this->pkt_data[offset + 0xa]);//0xff
            tmp.s[2] = Alog(this->pkt_data[offset + 0xb]);//0x19
            tmp.s[3] = Alog(this->pkt_data[offset + 0xc]);//0x09

            this->m_Echo_diff = ntohl(tmp.l);
            // the initial serial number, a magic number!
//            this->m_init_Echo_Key = htonl(0x1b8b4563);

            return 0;
          }
        else if (this->pkt_data[0x12] == 4)
          {
            return 1; // if failed.
          }
        continue;
        }
    }
  return -1;
}
// Here, we exert IMAP to detect the status of connection,
// namely, the fairly common command 'ping'
int
IfOnline(ruijie_packet *this)
{
  if(this->m_pinghost)
    return Ping(this->m_pinghost);
  else
    return 0;
}
int
SendEchoPacket(ruijie_packet *this)
{
  // keep-alive echo packet
  static
  uint8_t echoPackage[] =
  {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x88,0x8E, //802.1x
    0x01,0xBF, 0x00,0x1E,
    0xFF,0xFF,0x37,0x77,0x7F,0x9F,0xF7,0xFF,0x00,0x00,0xFF,0xFF,0x37,0x77,
    0x7F,0x9F,0xF7,0xFF,0x00,0x00,0xFF,0xFF,0x37,0x77,0x7F,0x3F,0xFF
  };

  union
  {
    u_int32_t l;
    u_char s[4];

  } tmp;

  tmp.l = this->m_init_Echo_Key;

  echoPackage[0x22] = Alog(tmp.s[0]);
  echoPackage[0x23] = Alog(tmp.s[1]);
  echoPackage[0x24] = Alog(tmp.s[2]);
  echoPackage[0x25] = Alog(tmp.s[3]);

  tmp.l = htonl(ntohl(this->m_init_Echo_Key)  + this->m_Echo_diff) ;

  echoPackage[0x18] = Alog(tmp.s[0]);
  echoPackage[0x19] = Alog(tmp.s[1]);
  echoPackage[0x1a] = Alog(tmp.s[2]);
  echoPackage[0x1b] = Alog(tmp.s[3]);

  memcpy(echoPackage,this->m_ETHHDR,12);

  if (WaitPacket(this, 1000) != 0) // The server send a packet to inform you offline
    {
      // watch out! Here is the crucial part of determining interruption
      // of network connection
      pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data);
      if (this->pkt_data[0x12] == 0x04) // if server send us Failure message
        return -1; // if the connection has been interrupted.
    }
  pcap_sendpacket(this->m_pcap, echoPackage, sizeof(echoPackage));
  this->m_init_Echo_Key = htonl(ntohl(this->m_init_Echo_Key) + 1);
  return 0;
}

int
SendEndCertPacket(ruijie_packet *this)
{
  // end certification packet
  static
  uint8_t ExitPacket[0x3E8] =
  {
    0x00,0x00,0x00,0x00,0x00,0x00,        // Destination MAC
    0x00,0x00,0x00,0x00,0x00,0x00,        // Source MAC
    0x88,0x8E,                            // Ethertype = 0x888E (8021X)
    0x01,                                 // Version = 1
    0x02,                                 // Packet Type  0x00
                                          // 0x01,EAPOL-Start
                                          // 0x02 ;0x03 ;0x04
    0x00,0x00,                            // Packet Body Length
  };

  memcpy(ExitPacket, this->m_ETHHDR, 12);// fill destined MAC and local MAC

  memcpy(ExitPacket + 18, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Logouting... \n", stdout);
  return (pcap_sendpacket(this->m_pcap, ExitPacket, 0x80) == 0x80) ? 0 : -1;
}
int
FlushRecvBuf(ruijie_packet*this)
{
        char buf[1500];
        while(recvfrom(this->m_pcap_no,buf,1500,MSG_DONTWAIT,0,0)>0);
}
