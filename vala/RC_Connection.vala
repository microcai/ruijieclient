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

errordomain AuthFailed{
	Unknow
}

 
public static class Connection : Object {
	public int state {get ; private set ; default = State.Unknow ;}

	public Connection(){
		this.load_echo();
	}

	
	public void auth() throws AuthFailed {
		//call auth fuc
		//TODO check conf 
		if (start_auth( (char[])(conf.user_name) , (char[])(conf.user_password) , (char[])(conf.NIC) , conf.auth_mode )
			== 0){
			this.state = State.AuthSuccessed ;
			return;
		}
		this.state = State.AuthFailed ;
		throw new AuthFailed.Unknow("") ;
	}
	public void logoff(){
		//call logoff fuc
		stop_auth();
	}


//echo packet	
	private TimeoutSource echo_timer ;
	public void load_echo(){
		if ( conf == null ){
			return ;
		}
		int echo_time = conf.echo_interval;
		if ( echo_time >5 ){
			if ( this.echo_timer != null ){
				this.echo_timer.destroy();
			}
			this.echo_timer = new TimeoutSource.seconds(echo_time);
			this.echo_timer.set_callback(check_echo);
			this.echo_timer.attach( loop.get_context());
		}else{
			if ( this.echo_timer != null ){
				this.echo_timer.destroy();
			}
			this.echo_timer = null ;
		}
	}
	private bool check_echo(){
		message("echoing...");
		if( this.state == State.AuthSuccessed ){
			echo();
		}
		return true ;
	}
	
}


 
