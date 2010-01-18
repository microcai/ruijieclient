/*
 * RC_RuijieClient.vala
 *
 *  Created on: 2010-1-3
 *      Author: G.S.Alex < i AT gsalex.net > from HIT at Weihai *
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


using GLib;

RC_Config 	conf ;
MainLoop 	loop  ;
Connection	connection ;

int main(string[] args){

	
	//TODO some init
	loop = new MainLoop(null, false);
	connection = new Connection();
	conf = new RC_Config();
	
	string[] 	split_name	= args[0].split("/");
	string 	program_name 	= split_name[split_name.length - 1];
	//string 	command 	= string.joinv(" ", args);
	
	for (int i = 1; i < args.length; i++) {
		string arg = args[i];

		string[] split_arg = arg.split("=");
		switch (split_arg[0]) {
			case "-h":
			case "-?":
			case "--help":
				show_usage(program_name);
				return 0;
			case "-V":
			case "-v":
			case "--version":
				show_version();
				return 0;
			case "--confFile":		conf.conf_file_path	=split_arg[1];		break;
			case "-d":			conf.daemon_mode	=true        ;		break;
			case "--gui":			conf.gui_mode		=true        ;		break;
			case "--userName":		conf.user_name		=split_arg[1];		break;
			case "--userPassword":		conf.user_password	=split_arg[1];		break;
			case "--authMode":		conf.auth_mode		=split_arg[1].to_int();	break;
			case "--NIC":			conf.NIC		=split_arg[1];		break;
			case "--echoInterval":		conf.echo_interval	=split_arg[1].to_int();	break;
			case "--autoReconnect":	conf.auto_reconnect	=true        ;		break;
			case "--fakeVersion":		conf.fake_version	=split_arg[1];		break;
			case "--DHCPmode":		conf.DHCP_mode		=split_arg[1].to_int();	break;
			case "--pingHost":		conf.ping_host		=split_arg[1];		break;
			default:
				stdout.printf("%s: Unknown option %s\n", program_name, arg);
				show_usage(program_name);
				return 1;
			}
		}
	
	
	if(conf.daemon_mode ){
		return runas_daemon();
	}else if(conf.gui_mode){
		return runas_gui();
	}
	else{
		return runas_cli();
	}
}

int runas_cli(){
	try{
		connection.auth() ;
	}catch (AuthFailed e) {
		message("Auth Failed.");
		return 1;
	}
	loop.run();
	return 0;
}
int runas_daemon(){
	//TODO daemon mode
	return 0 ;
}
int runas_gui(){
	new Gui();
	return 0;
}


void show_version(){
	stdout.printf("Programe under hard work , useless.\n");
	//TODO eh....say something here.
}

void show_usage(string program_name) {
	show_version();
	stdout.printf("Usage:\n");
	stdout.printf("  %s [OPTIONS..]\n\n", program_name);
	stdout.printf("Help Options:\n");
	stdout.printf("  -?, --help                         Show help options\n\n");
	stdout.printf("Application Options:\n");
	stdout.printf("  i am toooo lazy to write the help information... ");
	stdout.printf("  ");
	stdout.printf("  ");
	//TODO .....
}

