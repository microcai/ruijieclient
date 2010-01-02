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

#define EAP_REQUEST     1
#define EAP_RESPONSE    2
#define EAP_SUCCESS     3

#define MAKEWORD(hibyte,lobyte) ((hibyte<<8 & 0xFF00)| (lobyte&0XFF))

int ruijie_start_auth(char * name,char*passwd,char* nic_name,int authmode);
int ruijie_stop_auth();
int ruijie_get_server_msg( char * raw_encode_message_out,int * length);
int ruijie_echo();
#endif /* SUPPLICANT_H_ */
