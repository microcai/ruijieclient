/*
 * RC_Config.vala
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

using Config;

 
public class RC_Config : Object {

	public string user_name = "" ;
	public string user_password = "";
	public int	auth_mode = 1 ;
	public string NIC = "eth0" ;
	public int	echo_interval {get;set;default = 20 ;}
	public bool	auto_reconnect = true ;
	public string fake_version = "3.33" ;
	public int	DHCP_mode = 0 ;
	public string ping_host = "1.2.3.4" ;
	
	public bool	daemon_mode = false ;
	public bool	gui_mode = false ;
	public string conf_file_path = "/etc/ruijie.conf" ;

	public RC_Config () {
		this.load_from_file(Config.CONF_PATH );
		this.notify["echo-interval"].connect((s, p) => {connection.load_echo();});
		this.notify["user_name"].connect((s, p)=>{keyfile.set_string(group,"UserName",this.user_name);});
		this.notify["user_password"].connect((s, p)=>{keyfile.set_string(group,"UserPassword",this.user_password);});
		this.notify["NIC"].connect((s, p)=>{message("csd");keyfile.set_string(group,"NIC",this.NIC);});
	}
	
	File file ;
	KeyFile keyfile ;
	string group = "ruijieclient";
	public void load_from_file(string path){
		try{
			this.file = File.new_for_path (path);
			
			this.keyfile = new GLib.KeyFile();
			this.keyfile.load_from_file(path,KeyFileFlags.NONE);
			
			this.user_name = keyfile.get_string(group,"UserName");
			this.user_password = keyfile.get_string(group,"UserPassword");
			this.NIC= keyfile.get_string(group,"NIC");
		} catch (GLib.Error e){
			warning("Config file doesn't fit.");
			this.gen_new_config_file(path);
			//some work
		}
	}
	public void save_to_file(){
		string config = keyfile.to_data();
		message(config);
		return ;
		/*file.delete(null);
		var file_stream = this.file.create(GLib.FileCreateFlags.REPLACE_DESTINATION, null);
		var data_stream = new DataOutputStream (file_stream);
		data_stream.put_string (config, null);
		message("still some work");*/
	}
	public void gen_new_config_file(string path){
		message("gen_new_config_file still not work now");
	}
}

