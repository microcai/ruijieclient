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
using Gee;
using ruijie;

RC_Config 	conf ;
MainLoop 	loop ;


int main(string[] args){

	
	//TODO some init
	conf = new RC_Config();
	
	string[] 	split_name	= args[0].split("/");
	string 	program_name 	= split_name[split_name.length - 1];
	string 	command 	= string.joinv(" ", args);
	
	for (int i = 1; i < args.length; i++) {
		string arg = args[i];

		string[] split_arg = arg.split("=");
		switch (split_arg[0]) {
			case "-h":
			case "--help":
				show_usage(program_name);
				return 0;
			case "-v":
			case "--version":
				show_version();
				return 0;
			case "-d":
				return runas_daemon();
			case "--confFile":
				//conf.load_from_file(split_arg[1]);
				break;
			case "--userName":		conf["userName"]	=split_arg[1];break;
			case "--userPassword":		conf["userPassword"]	=split_arg[1];break;
			case "--authMode":		conf["authMode"]	=split_arg[1];break;
			case "--NIC":			conf["NIC"]		=split_arg[1];break;
			case "--echoInterval":		conf["echoInterval"]	=split_arg[1];break;
			case "--autoReconnect":	conf["autoReconnect"]	=split_arg[1];break;
			case "--fakeVersion":		conf["fakeVersion"]	=split_arg[1];break;
			case "--DHCPmode":		conf["DHCPmode"]	=split_arg[1];break;
			case "--pingHost":		conf["pingHost"]	=split_arg[1];break;
			default:
				stdout.printf("%s: Unknown option %s\n", program_name, arg);
				show_usage(program_name);
				return 1;
			}
		}
	
	loop = new MainLoop(null, false);
	
	Connection connect = new Connection();
	connect.request = Request.Auth ;
	check_echo();
	loop.run();
	return 0;
}

int check_echo(){
	int echo_time=conf["echoInterval"].to_int();
	if ( echo_time > 5 ){
		var time = new TimeoutSource.seconds(echo_time);
		time.set_callback(send_echo);
		time.attach(loop.get_context());
		return 0;
	}
	return 0;
}
//this is just a show case.
bool send_echo(){
	stdout.printf("sending_echoing~\n");
	echo();
	return true;
}


int runas_daemon(){
	//TODO daemon mode
	return 0 ;
}
void show_version(){
	stdout.printf("Programe under hard work , useless.\n");
	//TODO eh....say something here.
}

void show_usage(string program_name) {
	stdout.printf("Usage:\n");
	stdout.printf("  %s [--version] [--help]\n", program_name);
	//TODO .....
}

