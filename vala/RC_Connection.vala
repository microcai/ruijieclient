/*
 * RC_Connection.vala
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
 
using ruijie;
 
enum State {
	Unknow,
	Authing,
	AuthSuccessed,
	AuthFailed
}

enum Request {
	Auth,
	ReAuth,
	Logoff,
	Waiting
}

 
public static class Connection : Object {
	public int state {get ; private set ; default = State.Unknow ;}
	public int request { get ; set ; default = Request.Waiting ;}
	
	public Connection(){
		this.notify.connect((s, p) => {state_changed();});
		
	}
	
	private void state_changed(){
		switch (this.request){
			case Request.Auth:
				switch (this.state){
					case State.Unknow:
					case State.AuthFailed:
						do_auth();
						break;
				}
				break;
			case Request.ReAuth:
				do_auth();
				this.request = Request.Auth ;
				break;
			case Request.Logoff:
				switch (this.state){
					//TODO
				}
				do_logoff();
				break;
			case Request.Waiting:
				//nothing?
				break;
		}
	}
	
	private void do_auth(){
		//call auth fuc
		//TODO check conf 
		if (start_auth((char[])conf["userName"],(char[])conf["userPassword"],(char[])conf["NIC"],conf["authMode"].to_int())
			== 0){
			this.state = State.AuthSuccessed ;
			return;
		}
		this.state = State.AuthFailed ;
	}
	private void do_logoff(){
		//call logoff fuc
		stop_auth();
	}
	
}
 
