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

#if defined(HAVE_LIBXML2) && defined(DEBUG_NOXML)
#undef HAVE_LIBXML2
#endif

#ifdef HAVE_LIBXML2
//#define LIBXML_TREE_ENABLED 1
#include <libxml/xmlstring.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#endif

#include "ruijieclient.h"

static char fakeVersion[8];
//static char fakeMAC[32];

/*used by cfg_tags*/
enum cfg_tag_flag{
  /*Do not set val if val[0]!=0*/
  CFG_TAGS_NOTSET_IF_NOT_NULL = 1,
#define CFG_TAGS_NOTSET_IF_NOT_NULL CFG_TAGS_NOTSET_IF_NOT_NULL
};

/*These structure is used to simplify the work of parsing config file */
struct cfg_tags{
  const char const *    key;
  char *                val;
  const char const *    description;
  int                   val_size;
  enum cfg_tag_flag     flag;
};

/*Check whether we have got enough configuration info*/
void
CheckConfig(ruijie_packet* l)
{
#ifdef HAVE_LIBXML2
#define  ccerr(x) err_quit(x " found in ruijie.conf!\n")
#else
#define  ccerr(x) err_quit(x "\n")
#endif

  if (l->m_nocofigfile)
    {
      if (l->m_echoInterval == -1)
        l->m_echoInterval = 20;
      if (l->m_authenticationMode == -1)
        l->m_authenticationMode = 0;
      if (l->m_intelligentReconnect == -1)
        l->m_intelligentReconnect = 1;
      if (l->m_dhcpmode == -1)
        l->m_dhcpmode = 0;
    }

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

#if defined(LIBXML_TREE_ENABLED) && defined(HAVE_LIBXML2)

void get_element(xmlNode * a_node,struct cfg_tags * tg)
{
  xmlNode *cur_node = NULL;
  char *node_content, *node_name;
  struct cfg_tags * t;
  for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
    {
      t = tg;
      node_name = (typeof(node_name))cur_node->name;
      while (t->key)
        {
          if (!strcmp(node_name, t->key))
            {
              if(cur_node->children)
                node_content = cur_node->children->content;
              else break;
            if (t->flag == CFG_TAGS_NOTSET_IF_NOT_NULL)
                {
                  if (t->val[0]==0)
                    strncpy(t->val, node_content,t->val_size);
                }
              else
                strncpy(t->val, node_content,t->val_size);
              t->val[t->val_size - 1] = 0;
              break;
            }
          t++;
        }
      get_element(cur_node->children, tg);
    }
}
#else

static int
get_profile_string(FILE *fp, char *AppName, const char const *KeyName,char *KeyValue)
{
  int KEYVALLEN = 80;
  char appname[KEYVALLEN], keyname[KEYVALLEN];
  char buf[KEYVALLEN], *c;
  int found = 0; /* 1 AppName 2 KeyName */

  fseek(fp, 0, SEEK_SET);

  sprintf(appname, "[%s]", AppName);
  memset(keyname, 0, sizeof(keyname));
  while (!feof(fp) && fgets(buf, KEYVALLEN, fp) != NULL)
    {
      //if( l_trim( buf )==0 )
      //        continue;

      if (found == 0)
        {
          if (buf[0] != '[')
            {
              continue;
            }
          else if (strncmp(buf, appname, strlen(appname)) == 0)
            {
              found = 1;
              continue;
            }
        }
      else if (found == 1)
        {
          if (buf[0] == '#')
            {
              continue;
            }
          else if (buf[0] == '[')
            {
              break;
            }
          else
            {
              if ((c = (char*) strchr(buf, '=')) == NULL)
                continue;
              memset(keyname, 0, sizeof(keyname));
              sscanf(buf, "%[^=]", keyname);
              if (strcmp(keyname, KeyName) == 0)
                {
                  sscanf(++c, "%[^\n]", KeyValue);
                  found = 2;
                  break;
                }
              else
                {
                  continue;
                }
            }
        }
    }
  if (found == 2)
    return (0);
  else
    return (-1);
}

#endif

static char name[32];
static char password[32];
static char nic[32]="eth0";
static char AuthenticationMode[32]="0";
static char EchoInterval[32]="25";
static char IntelligentReconnect[32]="1";
static char AutoConnect[32]="0";
static char FakeVersion[32]="3.33";
static char DHCPmode[32]="0";
static char FakeAddress[32];
static char PingHost[32];

static struct cfg_tags cfgtags[]=
  {
#define DEF_ITEM(k,v) { k ,v, 0 , sizeof( v )},
#define DEF_ITEM_d(k,v,d) { k ,v, d , sizeof( v )},
#define DEF_ITEM_flag(k,v) { k ,v, 0,  sizeof( v ),CFG_TAGS_NOTSET_IF_NOT_NULL},
#define DEF_ITEM_flag_d(k,v,d) { k ,v, d,  sizeof( v ),CFG_TAGS_NOTSET_IF_NOT_NULL},

    DEF_ITEM_flag("Name",name)
#define USERNAME 0

    DEF_ITEM_flag("Password",password)
#define PASSWORD 1

    DEF_ITEM_d("NIC",nic,"Network Adapter Name")
#define NIC 2

    DEF_ITEM_d("AuthenticationMode",AuthenticationMode,
        "0: Standard, 1: Private")
#define AUATHENTICATIONMODE 3

    DEF_ITEM("EchoInterval",EchoInterval)
#define ECHOINTERVAL 4

    DEF_ITEM_d("IntelligentReconnect",IntelligentReconnect,
        "IntelligentReconnect: "
        "0: Disable IntelligentReconnect, 1: Enable IntelligentReconnect ")

#define INTELLIGENTRECONNECT 5
    DEF_ITEM_d("AutoConnect",AutoConnect,"AutoConnect: "
        "0: Disable AutoConnect, 1: Enable AutoConnect (only available in"
        " gruijieclient) ")
#define AUTOCONNET 6

    DEF_ITEM_d("FakeVersion",FakeVersion,"Fake Version for cheating server")
#define FAKEVERSION 7

    DEF_ITEM_d("FakeAddress",FakeAddress,"Fake IP for cheating server")
#define FAKEADDRESS 8
    DEF_ITEM_d("DHCPmode",DHCPmode,"DHCP mode 0: Disable, "
        "1: Enable DHCP before authentication, "
        "2: Enable DHCP after authentication "
        "3: DHCP after DHCP authentication and"
        "re-authentication(You should use this if your net env is DHCP)")
#define DHCPMODE 9

    DEF_ITEM_d("PingHost",PingHost,"Add if you don't want ruijieclient to ping the default gateway")
#define PINGHOST 10
    {0}

#undef DEF_ITEM
#undef DEF_ITEM_d
#undef DEF_ITEM_flag
#undef DEF_ITEM_flag_d
  };


static int
Gensetting(struct cfg_tags * t)
{
  int rc;
  char name[200];
  fprintf(stderr,"Please input UserName:");
  if(scanf("%s\n",name)==1)
	  t[USERNAME].val = name;
  t[PASSWORD].val = getpass("Please input Password:");

#ifdef HAVE_LIBXML2
  xmlDocPtr doc = NULL; /* document pointer */

  xmlNodePtr root_node = NULL, account_node = NULL, setting_node = NULL;//, msg_node = NULL;/* node pointers */

  // Creates a new document, a node and set it as a root node
  doc = xmlNewDoc(BAD_CAST"1.0");

  root_node = xmlNewNode(NULL, BAD_CAST CONF_NAME);
  xmlNewProp(root_node, BAD_CAST "version", BAD_CAST PACKAGE_VERSION);
  xmlAddChild(root_node, xmlNewComment((xmlChar *)
          "This is a sample configuration file of RuijieClient, "
          "change it appropriately according to your settings."));

  xmlDocSetRootElement(doc, root_node);
#else

  FILE * doc = fopen(config_file,"w");
  if (!doc)
  return -1;
  fprintf(doc,"[%s]\n",PACKAGE);
#endif

#ifdef HAVE_LIBXML2
  //creates a new node, which is "attached" as child node of root_node node.
  account_node = xmlNewChild(root_node, NULL, BAD_CAST "account", NULL);
#endif

  for( rc=0; rc < 2; rc++,++t)
    {
      if(t->description)
#ifdef HAVE_LIBXML2
        xmlAddChild(account_node, xmlNewComment((xmlChar *)t->description));
      xmlNewChild(account_node, NULL, BAD_CAST t->key, BAD_CAST t->val);
#else
      /*
       * NOTE: here, generate ini format file
       * and contains Name= and password=   *
       */
        fprintf(doc, "\n#%s\n%s=%s\n", t->description, t->key, t->val);
      else
        fprintf(doc, "%s=%s\n", t->key, t->val);
#endif
    }


#if defined(LIBXML_TREE_ENABLED) && defined(HAVE_LIBXML2)
  setting_node = xmlNewChild(root_node, NULL, BAD_CAST "settings", NULL);
#else
  fprintf(doc,"#This is %s\n","settings");
#endif
  /*
   * Not all machine name the first nic eth0
   * So we just have to retrieve the first nic's name
   */
  pkt_first_link(nic);
  printf("using nic %s\n",nic);//,ifaddr->ifa_next);
  t[NIC].val = nic;

  while( t->key)
    {
      if( t->description)
#if defined(LIBXML_TREE_ENABLED) && defined(HAVE_LIBXML2)
        xmlAddChild(setting_node, xmlNewComment((xmlChar *)t->description));
      xmlNewChild(setting_node, NULL, BAD_CAST t->key, BAD_CAST t->val);
#else
        fprintf(doc, "#%s\n%s=%s\n", t->description, t->key, t->val);
      else
        fprintf(doc, "%s=%s\n", t->key, t->val);
#endif
    t++;
    }
  //Dumping document to stdio or file
#if defined(LIBXML_TREE_ENABLED) && defined(HAVE_LIBXML2)
  rc = xmlSaveFormatFileEnc(config_file, doc, "UTF-8", 1);
  /*free the document */
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump(); // debug memory for regression tests
  if (rc == -1)
    return -1;
  return 0;
#else
  fprintf(doc,"\n");
  return fclose(doc);
#endif
}

void
GetConfig(ruijie_packet * l)
{

#ifdef HAVE_LIBXML2

  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION

  /*parse the file and get the DOM */
  doc = xmlReadFile(config_file, NULL, 0);
#else
  struct cfg_tags * t = cfgtags;
  FILE *doc = fopen(config_file, "r");
#endif

  if (doc == NULL)
    {
      puts("Could not parse or find file. A sample file will be generated "
        "automatically. Try 'gedit /etc/ruijie.conf'");
      if (Gensetting( cfgtags ) != -1)
        {
          puts("Configuration file has been generated.");
          exit(0);
        }
      else
        {
          err_quit("Configuration file fail in generating.");
        }
    }

#ifdef  HAVE_LIBXML2
  /*Get the root element node */
  root_element = xmlDocGetRootElement(doc);

  get_element(root_element, cfgtags);

  /*free the document */
  xmlFreeDoc(doc);

  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
  xmlCleanupParser();
#else

  while (t->key)
    {
      get_profile_string(doc, PACKAGE, t->key, t->val);
      t++;
    }
  fclose(doc);

#endif
  //NOTE: now , move things to  ruijiepacket * l
  if(l->m_name[0]==0)
    strcpy(l->m_name , cfgtags[USERNAME].val);
  //else m_name has been filled by cmdline
  if(l->m_password[0]==0)
    strcpy(l->m_password, cfgtags[PASSWORD].val );
  //else m_password has been filled by cmdline
  if(l->m_nic[0]==0)
    strcpy(l->m_nic,cfgtags[NIC].val);
  //else m_nic has been filled by cmdline
  if(l->m_echoInterval<=0)
    l->m_echoInterval = atoi(cfgtags[ECHOINTERVAL].val);
  //else m_echoInterval has been filled by cmdline
  if(!l->m_fakeVersion)
    l->m_fakeVersion = cfgtags[FAKEVERSION].val;
  //else m_fakeVersion has been filled by cmdline
  if(cfgtags[FAKEADDRESS].val[0] && l->m_ip==0)
    l->m_ip = inet_addr(cfgtags[FAKEADDRESS].val);
  //else fakeaddress has been filled by cmdline
  if(l->m_dhcpmode == -1)
    l->m_dhcpmode = atoi(cfgtags[DHCPMODE].val);
  //else m_dhcpmode has been filled by cmdline
  if(l->m_intelligentReconnect == -1)
    l->m_intelligentReconnect = atoi(cfgtags[INTELLIGENTRECONNECT].val);
  //else m_intelligentReconnect has been filled by cmdline
  if(l->m_authenticationMode==-1)
    l->m_authenticationMode = atoi(cfgtags[AUATHENTICATIONMODE].val);
  //else m_authenticationMode has been filled by cmdline
  if(PingHost[0])
    l->m_pinghost = inet_addr(PingHost);
  //else use default gateway
}

int GenSetting()
{
  open_lib();
  pkt_first_link(nic);
  return Gensetting(cfgtags);
}

