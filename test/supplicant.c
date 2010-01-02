//file to test that does the vapi works fine

#include <stdio.h>
#include <string.h>

int ruijie_start_auth(char * name,char*passwd,char* nic_name,int authmode){
	printf ("called:ruijie_start_auth\n");
	printf ("name:%s\n",name);
	printf ("pswd:%s\n",passwd);
	printf ("nicn:%s\n",nic_name);
	printf ("atmd:%d\n",authmode);
	return 1;
}


int ruijie_stop_auth(){
	printf ("called:ruijie_stop_auth\n");
	return 2;
}


int ruijie_get_server_msg( char * raw_encode_message_out,int length){
	
	printf ("called:ruijie_get_server_msg\n");
	strcpy(raw_encode_message_out , "return this message!");
	return length;
}


int ruijie_echo(){
	printf ("called:ruijie_echo\n");
	return 4;
}
