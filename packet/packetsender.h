/**
 * @defgroup 数据包发送
 * @author	 microcai.
 * @{
 * @name packetsender.h
 * @{
 */

/*
 * packetsender.h
 *
 *  Created on: 2009-12-9
 *      Author: <microcai AT sina.com > from ZSTU *
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

#ifndef PACKETSENDER_H_
#define PACKETSENDER_H_

#define ETH_MTU 1500
#define ETH_PROTO_8021X 0x888E

#define PKT_PG_HWADDR 1
#define PKT_PG_IPADDR 2
#define PKT_PG_IPMASK 3
#define PKT_PG_DEFAULTROUTE     4
#define PKT_PG_DNS              5


#define HIBYTE(word) (( ((word) & 0xFF00 ) >>8) & 0xFF)
#define LOBYTE(word) ( word & 0xFF)

/**
 * @brief 打开libpcap库
 * @retval 0 库打开成功 (编译连接到libpcap库的时候总是返回 0 )
 * @retval -1 无法找到库
 *
 * 请在调用其他任何发包函数前调用本函数。
 */
int open_lib();

/**
 * @brief 取得本机第一个网络适配器名称
 * @param[out] nic 指向一个接收缓冲区，保存接受到的网卡名称
 * 					该缓冲区一定要足够大，至少容得下网卡名称外加一个 NULL 字符
 * @return 成功返回0，失败返回 -1
 */
int pkt_first_link(char *nic);

/**
 * @brief 打开指定的网络适配器
 * @param[in] nic_name 网卡名称字符串。该字符串在 Linux 下具有
 * 					 ethX 的形(X为网卡编号)。可以由 pkt_first_link 获得
 * @return 成功返回0 , 失败返回-1.
 *
 * pkt_open_link 调用成功后才可以使用其他的发送和接收函数。偶在内部维护设备状况
 */
int pkt_open_link(const char * nic_name);
int pkt_build_ruijie(int lengh,const char* ruijiedata);
int pkt_build_8021x_ext(u_char code, u_char id, uint16_t length,const char* extra);
int pkt_build_8021x(u_char version, u_char type, uint16_t length,void*paylod,int payloadlen);
int pkt_build_ethernet(u_char*dest,u_char*src,uint16_t protocol);
int pkt_write_link();
int pkt_read_link(const u_char**packet);
int pkt_close();
char* pkt_lasterr();

#endif /* PACKETSENDER_H_ */

/**//** @}*/ //

/**//** @}*/ //
