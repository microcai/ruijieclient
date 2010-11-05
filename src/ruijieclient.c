/*********************************************************************************
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun  Microcai                                *
 *********************************************************************************
 *
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

#include "ruijieclient.h"
#include "myerr.h"
#include "prase.h"
#include "../packet/supplicant.h"

// fake MAC, e.g. "00:11:D8:44:D5:0D"
//static char *m_fakeMAC = NULL;

// flag of afterward DHCP status

/* These info should be worked out by initialisation portion. */

/* Authenticate Status
 * 0: fail to find server
 * 1: fail to pass Authentication of user name
 * 2: fail to pass Authentication of MD5 sum
 * 3: success
 */

/* kill other processes */
static int
kill_all(char* process);
/*check root*/
static void
check_as_root()
{
#ifndef DEBUG
  if (geteuid() != 0)
    {
      err_sys("Ruijieclient must be run as root.");
    }
#endif
}

/* cleanup on exit when detected Ctrl+C */
static void
logoff(int signo)
{
  ruijie_stop_auth();
  _exit(0);
}

static ruijie_packet sender = { 0 };

// return 1 will abort the auth engine
static int ruijie_call_back(int reason,const char * current_packet,void*userptr)
{
  static char messageGBK[1500];
  static char messageUTF8[1500];
  int gbklen;

  switch(reason)
  {
	case RUIJIE_AUTH_FINDSERVER:
	  fputs("@@ Detecting Server ...... \n", stdout);
	  break;
	case RUIJIE_AUTH_NEEDNAME:
	  fprintf(stdout, "@@ Server Detected @ authorize as %s ...\n",sender.m_name);
	  break;
	case RUIJIE_AUTH_NEEDPASSWD:
	  fputs("@@ User name valid @@ encrpyt password...\n", stdout);
	  break;
	case RUIJIE_AUTH_SUCCESS:
	  fputs("@@ User Authorize!\n", stdout);
	  //开始剥取数据
	  if((gbklen = ruijie_get_server_msg(messageGBK,sizeof(messageGBK)))>1)
	  {
		  code_convert(messageUTF8,sizeof(messageUTF8),messageGBK,gbklen);
		  fputs(messageUTF8,stdout);
		  fputs("\n",stdout);
	  }
	  break;
	case RUIJIE_AUTH_FAILED:
	  fputs("@@ Authorize Failed!\n", stdout);
	  if((gbklen = ruijie_get_server_msg(messageGBK,sizeof(messageGBK)))>1)
	  {
		  code_convert(messageUTF8,sizeof(messageUTF8),messageGBK,gbklen);
		  fputs(messageUTF8,stdout);
		  fputs("\n",stdout);
	  }
	  break;
  }
  return 0;
}

int start_dhcp()
{
  return 0;
}

int
main(int argc, char* argv[])
{
  // this is a top crucial change that eliminated all global variables

  /* message buffer define*/
  // utf-8 msg buf. note that each utf-8 character takes 4 bytes
  char u_msgBuf[MAX_U_MSG_LEN];

  // system command
  char cmd[32] = "dhclient -4"; //ipv4 only

  int noip_afterauth = 1;
  long setdaemon = 0;
  long nodaemon = 0;
  long kill_ruijieclient = 0;
  long flag_nokill = 0;
  int try_time = 5;
  long showversion = 0;
  char pinghost[32] = "";

  sender.m_echoInterval = 20;

  //gonhan 大哥，不要动这个结构体的code style,eclipse 的自动格式化总是把这个搞的更难看
  struct parameter_tags param[] =
  {
    {"-D", (char*)&nodaemon," -D\t\t DO NOT fork as a deamon",sizeof(nodaemon),2, BOOL_both},
    {"--daemon", (char*)&setdaemon,"    --daemon\t run as a daemon(default)",sizeof(setdaemon),8, BOOL_both},
    {"--dhcpmode",(char*)&sender.m_dhcpmode,"    --dhcpmode=? dhcpmode, default is 0\n\t\t 0:disable\n\t\t 1:DHCP before auth\n\t\t 2:DHCP after auth,\n\t\t 3:dhcp-authenticate and re-authenticate after DHCP",sizeof(sender.m_dhcpmode),10,INTEGER},
    {"-K", (char*)&kill_ruijieclient ," -k,-K\t\t kill all RuijieClient daemon",sizeof(kill_ruijieclient),2, BOOL_both},
    {"-k", (char*)&kill_ruijieclient ,0,sizeof(kill_ruijieclient),2, BOOL_both},
    {"-n", sender.m_nic ,0,sizeof(sender.m_nic),2, STRING},
    {"--nic", sender.m_nic ," -n,--nic\t specify an identifier of net adapter",sizeof(sender.m_nic),5, STRING},
    {"-p",sender.m_password ,0,sizeof(sender.m_password),2,STRING},
    {"--passwd",sender.m_password," -p,--passwd\t specify password",sizeof(sender.m_password),6,STRING},
    {"--pinghost",pinghost,"    --pinghost\t the host to be pinged(by default is your default gateway)\n\t\t RuijieClient uses this to detect network state",sizeof(pinghost),10,STRING},
    {"--try", (char*)&try_time ,"    --try=?\t Try number of times of reconnection,-1 = infinite",sizeof(try_time),5,INTEGER},
    {"-u",sender.m_name ,0,sizeof(sender.m_name),2,STRING},
    {"--user",sender.m_name," -u,--user\t specify user name",sizeof(sender.m_name),6,STRING},
    {"-s",(char*)&sender.m_authenticationMode,0,sizeof(sender.m_authenticationMode),2,BOOL_both},
    {"--std",(char*)&sender.m_authenticationMode," -s,--std\t use standard discovery MAC address",sizeof(sender.m_authenticationMode),5,BOOL_both},
    {"-i",(char*)&sender.m_echoInterval,"-i    \t echo interval, default 20s",sizeof(sender.m_echoInterval),2,INTEGER},
    {"--version", (char*)&showversion ,"    --version\t show the version of RuijieClient",sizeof(showversion),9, BOOL_both},
    {"-M", (char*)&flag_nokill ,0,sizeof(flag_nokill),2, BOOL_both},
    {0}
  };

  // Parse command line parameters
  ParseParameters(&argc, &argv, param);
  if (showversion)
    err_quit("%s", PACKAGE_VERSION);

  //if '-k,K' is passed as argument then kill all other ruijieclients which are running
  if (kill_ruijieclient)
    {
      if (kill_all("ruijieclient"))
        err_quit(
            "Can not kill ruijieclient, permission denied or no such process");
      exit(EXIT_SUCCESS);
    }

  if (pinghost[0])
    sender.m_pinghost = inet_addr(pinghost);

  //NOTE:check if we had get all the config
  CheckConfig(&sender);
#ifndef DEBUG
  // kill all other ruijieclients which are running
  if (!flag_nokill){
	  kill_all("ruijieclient");
  }
  kill_all("xgrsu 2> /dev/null");
#endif
  strcat(cmd, sender.m_nic);

  signal(SIGHUP, logoff);
  signal(SIGINT, logoff);
  signal(SIGQUIT, logoff);
  signal(SIGABRT, logoff);
  signal(SIGTERM, logoff);
  signal(SIGSTOP, logoff);
  signal(SIGTSTP, logoff);

  //print copyright and bug report message
  printf("\n\n%s - a powerful ruijie Supplicant for UNIX, base on mystar.\n"
    "Copyright © %s %s\n"
    "Copyright © %s %s\n\n"
    "Please see/send bug report to \n%s\n"
    "or mail to %s \n\n", PACKAGE,
    "2009-2009","Microcai, G.S.Alex, Gong Han, Chen Tingjun, and others",
    "2009-2010","薇菜工作室 and G.S.Alex",
    "http://code.google.com/p/ruijieclient/issues/list",
    PACKAGE_BUGREPORT);
  do
	{
	  int dhcpstate[4] =
		{ 1, 1, 0, 0 };

	  int dhcped = dhcpstate[sender.m_dhcpmode];

	  /*
	   * DHCP mode:
	   * 0: Off
	   * 1: On, DHCP mode
	   * 2: On, DHCP after authentication
	   * 3: On, DHCP after DHCP authentication and re-authentication with new ip
	   */
	  int dhcpkey[] =
		{ 0, 1, 1, 5 };
	  do
		{
		  if (ruijie_start_auth(sender.m_name, sender.m_password, sender.m_nic,
			                    sender.m_authenticationMode << 16
			                        & dhcpkey[sender.m_dhcpmode] & dhcped < 1,
			                    ruijie_call_back, 0)) return -1;
		  if (sender.m_dhcpmode != 3)
			sender.m_dhcpmode &= 0xFFFFFFFC;
		  else if (!dhcped)
			{
			  sender.m_dhcpmode = 0;
			}
		  else
			{
			  ruijie_echo();
			}
		  if (!dhcped) dhcped = start_dhcp() ? 0 : 1;
		} while (sender.m_dhcpmode && dhcped == 1);

	  if (sender.m_echoInterval <= 0)
		{
		  pkt_close();
		  return 0; //user has echo disabled
		}
	  // continue echoing
	  if (nodaemon)
		fputs("Keeping sending echo...\nPress Ctrl+C to logoff \n", stdout);
	  else
		{
		  fprintf(stdout,
			      "Daemonize and Keeping sending echo...\nUse %s -K to quit",
			      PACKAGE_TARNAME);
		  if (daemon(0, 0))
			{
			  err_quit("Init daemon error!");
			}
		}

	  // start ping monitoring
	  /* Why the hell we should send echo packet immediately?
	   * so, sleep ! wa haha
	   */
	  while (!ruijie_echo())
		{
		  //Accelerate the speed of detecting
		  //       WaitPacket(&sender,sender.m_echoInterval);
		  sleep(sender.m_echoInterval);
		}
	} while (sender.m_intelligentReconnect == 1);
  return 1;
}

static int
kill_all(char * process)
{
  char cmd[256] = "";
  int cmd_return = 0;

  sprintf(cmd, "killall -2 %s", process);
  cmd_return = system(cmd);
  if (cmd_return < 0)
    {
      err_sys("Killall Failure !");
    }
  return cmd_return;
}
