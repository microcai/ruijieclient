/*
 * RC_Connection.vala
 *
 *  Created on: 2010-1-3
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
 
using ruijie;
 
 

enum State {
	Unknow,
	Authing_Search_Server,
	Authing_Sending_UserName,
	Authing_Sending_Password,
	Authing_done,
	AuthSuccessed,
	AuthFailed,
	Canceling
}



 
public class Connection : Object {
	public int pre_state {get ; private set ; default = State.Unknow ;}
	public int state {get ; set ; default = State.Unknow ;}

	public Connection(){
		this.load_echo();
	}

	public static int auth_callbcak( int reason, string? current_packet ,void * userptr ){
		Connection con = (Connection) userptr ;
		if (con.state == State.Canceling ) { 
			if ( reason == auth_callback_reason.RUIJIE_AUTH_SUCCESS ){
				stop_auth();
			}
			return -1 ;
		}
		switch ( reason ){
			//i do think ruijie_auth_callback_reason is ugly designed.
			case auth_callback_reason.RUIJIE_AUTH_FINDSERVER : con.state = State.Authing_Search_Server ; break;
			case auth_callback_reason.RUIJIE_AUTH_NEEDNAME :  con.state = State.Authing_Sending_UserName ; break;
			case auth_callback_reason.RUIJIE_AUTH_NEEDPASSWD : con.state = State.Authing_Sending_Password ; break;
			case auth_callback_reason.RUIJIE_AUTH_SUCCESS : con.state = State.AuthSuccessed ; break;
			case auth_callback_reason.RUIJIE_AUTH_FAILED : con.state = State.AuthFailed ; break;
			default: message("auth callback returned an unexpected state!") ; break ;
		}
		return 0;
	}
	public void auth(){
		//lock(this.state){
			Thread.create(_auth, false);
		//}
	}
	public void* _auth() {
			start_auth( (char[])(conf.user_name) , (char[])(conf.user_password) , (char[])(conf.NIC) 
					, conf.auth_mode ,auth_callbcak,this);
			return null;
	}
	public void logoff(){
		//caicai didn't offer a good library,so we need some hack here
		if ( this.state == State.AuthSuccessed){
			stop_auth();
		}
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


//some code for binding.
enum auth_callback_reason{
		RUIJIE_AUTH_FINDSERVER = 1,
		RUIJIE_AUTH_NEEDNAME = 2,
		RUIJIE_AUTH_NEEDPASSWD = 3,
		RUIJIE_AUTH_SUCCESS = 0,
		RUIJIE_AUTH_FAILED = 4,
}
enum authmode{
		/*
	 	* dhcp 打开 (DHCP模式)
	 	*/
		RUIJIE_AUTHMODE_DHCP = 0x00000001,
		
		/*
		* 不使用本机获得的 ip 地址和其他信息 (进行dhcp认证的时候用)
	 	*/
		RUIJIE_AUTHMODE_NOIP = 0x00000002 ,

		/*
	 	* 不要初始化 计数 :) (dhcp二次认证的时候用到)
	 	*/
		RUIJIE_AUTHMODE_NOECHOKEY = 0x00000004 ,

		/*
		* 使用锐捷私有广播地址
		*/
		RUIJIE_AUTHMODE_PRIVATEBROADCAST =  0x00010000
}
