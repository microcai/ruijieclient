//file to test that does the vapi works fine


using ruijie;

int main(){
	start_auth((char[])"username",(char[])"password",(char[])"eth012",3);
	
	stop_auth();
	
	char[] msg = new char[200];
	get_server_msg(msg,msg.length);
	StringBuilder sb = new StringBuilder();
	foreach(char c in msg) {
		if(c == '\0') break;
		sb.append_printf("%c", c);
	}
	stdout.printf("get_msg:%s\n",sb.str);

	
	echo();
	
	return 0;
}
