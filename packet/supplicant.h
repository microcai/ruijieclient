/**
 * @defgroup 认证函数
 * @author	 microcai.
 * @{
 * @name supplicant.h
 * @{
 */



/*
 * supplicant.h
 *
 *  Created on: 2009-12-11
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

#ifndef SUPPLICANT_H_
#define SUPPLICANT_H_

//Packet Type 0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04

#define RJ_PKT_ACK 00
#define RJ_PKT_START 01
#define RJ_PKT_STOP  02


#define EAP_REQUEST		1
#define EAP_RESPONSE    2
#define EAP_SUCCESS     3
#define EAP_FAILED		4

#define MAKEWORD(hibyte,lobyte) ((hibyte<<8 & 0xFF00)| (lobyte&0XFF))

/**
 * @brief 回调函数调用原因
 */
enum ruijie_auth_callback_reason
{
  RUIJIE_AUTH_SUCCESS = 0,
  /**<服务器通过认证*/

  RUIJIE_AUTH_FINDSERVER = 1,
  /**< 在查找服务器。返回-1停止查找。结束认证*/

  RUIJIE_AUTH_NEEDNAME = 2,
  /**< 服务器需要用名*/

  RUIJIE_AUTH_NEEDPASSWD = 3,
  /**<服务器需要密码*/

  RUIJIE_AUTH_FAILED = 4,
  /**<服务器拒绝认证*/
};

/**
 * @brief 认证模式
 */
enum ruijie_authmode
{
  RUIJIE_AUTHMODE_DHCP = 0x00000001,
  /**<dhcp 打开 (DHCP模式)*/

  RUIJIE_AUTHMODE_NOIP = 0x00000002 ,
  /**<不使用本机获得的 ip 地址和其他信息 (进行dhcp认证，此时本机没有可用
   * 的ip地址的时候使用)*/

  RUIJIE_AUTHMODE_NOECHOKEY = 0x00000004 ,
  /**<不要初始化 心跳包计数器 :)
   * (二次dhcp认证的时候用到)*/

  RUIJIE_AUTHMODE_PRIVATEBROADCAST =  0x00010000
  /**<使用锐捷私有广播地址，而不是标准的MAC层广播协议地址*/
};

/**
 * @brief 进行认证
 * @param[in] name 认证用户名
 * @param[in] passwd 明文 密码
 * @param[in] nic_name 使用的网卡名称，可以使用 pkt_first_link 获得首个可用的网卡名称
 * @param[in] authmode	认证模式 参考 ruijie_authmode
 * @param[in] authprogress	回调函数地址。
 * @param[in] userptr		传递给回调函数的数据
 *
 * @retval	0	success!
 * @retval  1	failed.....
 *
 * @see pkt_first_link ruijie_authmode
 */
int ruijie_start_auth(char * name, char*passwd, char* nic_name, int authmode,
    int (*authprogress)(int reason, const char * current_packet, void*userptr),
    void * userptr);

/**
 * @brief 停止认证
 * @return 应该总是反回0，如果不是，一定是不可恢复的错误。
 */
int ruijie_stop_auth();

/**
 * @brief 返回认证服务器发回的消息
 * @param[out]  raw_encode_message_out
 * 				原始编码的服务器消息。服务器消息具体是何种编码不做解释。多数情况下是GBK编码的
 * @param[in]	length 输出消息缓存的长度。
 * @return 消息的字节数。
 * @retval -1 length长度不够
 * @retval 0 服务器没有返回消息或者消息提取失败
 */
int ruijie_get_server_msg( char * raw_encode_message_out,int length);

/**
 * @brief 执行心跳发包
 * @return 成功发送返回0 ，发送失败或者发送的时候已经下线返回 -1
 *
 *
 * ruijie_echo 内部维护心跳计数，该数字在认证成功后从服务器发回的数据包中取得。
 * 然后每次发送都递增。最好在20s左右时间重复发送(调用即可)
 */
int ruijie_echo();
#endif /* SUPPLICANT_H_ */

/**//** @}*/ //

/**//** @}*/ //
