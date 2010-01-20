/*
 * RC_CLI.vala
 *
 *  Created on: 2010-1-20
 *      Author: G.S.Alex < i AT gsalex.net > from HIT at Weihai *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
using Config ;
 
public int runas_cli(){
	print_pgm_info();
	print_auth_info();
	connection.notify["state"].connect(cli_state_change);
	connection.auth() ;
	if (connection.state == State.AuthSuccessed){
		loop.run();
	}
	return 0;
}

private void cli_state_change(){
	stdout.printf("state:%i\n",connection.state);
	switch (connection.state){
		case State.Authing_Search_Server :
			stdout.printf("Searching for server ...\n "); break;
		case State.Authing_Sending_UserName :
			stdout.printf("Server Found . Now sending user name ... \n")  ; break;
		case State.Authing_Sending_Password :
			stdout.printf("Sending password ... \n")  ; break;
		case State.AuthSuccessed :
			stdout.printf("Authenticate success.\n")  ; break;
		case State.AuthFailed :
			stdout.printf("Authenticate FAILED \n")  ; break;
		default: break;
	}
}

public void print_pgm_info(){
	stdout.printf("\n%s  %s \n", APP_NAME, APP_VERSION ); 
	stdout.printf("If any problem , connect %s .\n", AUTHORS);
	
}

public void print_auth_info(){
	stdout.printf("Use user %s for authenticate.\n", conf.user_name);
	stdout.printf("Net interface : %s \n", conf.NIC);
}
