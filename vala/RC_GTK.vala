/*
 * RC_GTK.vala
 *
 *  Created on: 2010-1-12
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
 
using Gtk;
using DBus;
 
 
public class Gui : GLib.Object {


 	
 	Window main_win ;
 	public Gui(){
 		string[] ab_args = {""};
 		Gtk.init (ref ab_args);
		Builder builder ;
 		try {
 			builder = new Builder ();
			builder.add_from_file ("./gtkbuilder.glade");
			builder.connect_signals (this);
			main_win = builder.get_object ("window") as Window;
			this.init_widget(builder);
			main_win.show_all();
		}catch (GLib.Error e){
			//TODO
			return ;
		}
		init_signal();
		init_content();
		Gtk.main ();
 	}
 	
 	Entry entry_username	;
 	Entry entry_password	;
 	NIC_Chooser nic_chooser	;
 	Label label_state		;
 	Label label_ip		;
 	Label label_netmask	;
 	private void init_widget(Builder builder){
 		this.entry_username = builder.get_object ("entry_username") as Entry ;
 		this.entry_password = builder.get_object ("entry_password") as Entry ;
 		this.nic_chooser	= new_NIC_Chooser();
 		this.label_state	= builder.get_object ("label_state") as Label ;
 		this.label_ip	= builder.get_object ("label_ip") as Label ;
 		this.label_netmask	= builder.get_object ("label_netmask") as Label ;
 		
		Table table = builder.get_object ("table_set") as Table;
		table.attach(nic_chooser.get_widget(),1,2,2,3,AttachOptions.FILL,AttachOptions.FILL,10,0);
 		
 	}
 	private void init_content(){
 		this.entry_username.text = conf.user_name ;
 		this.entry_password.text = conf.user_password ;
 		this.nic_chooser.set_nic	(conf.NIC) ;
 	}
 	private void init_signal(){
		connection.notify["state"].connect(state_info_update);
		this.nic_chooser.nic_change.connect(()=>{this.label_ip.label = this.nic_chooser.get_ip();
								this.label_netmask.label = this.nic_chooser.get_netmask();});
 	}
 	public void state_info_update(){
 		stdout.printf("state:%i\n",connection.state);
 		switch (connection.state){
			case State.Authing_Search_Server :
				stdout.printf("Searching for server ...\n "); 
				this.label_state.label = "搜索服务器... ";
				break;
			case State.Authing_Sending_UserName :
				stdout.printf("Server Found . Now sending user name ... \n")  ; 
				this.label_state.label = "发送认证信息... ";
				break;
			case State.Authing_Sending_Password :
				stdout.printf("Sending password ... \n")  ; 
				this.label_state.label = "发送认证信息... ";;
				break;
			case State.AuthSuccessed :
				stdout.printf("Authenticate success.\n")  ; 
				this.label_state.label = "认证成功 ";
				break;
			case State.AuthFailed :
				stdout.printf("Authenticate FAILED \n")  ; 
				this.label_state.label = "认证失败 ";
				break;
			default: 
				this.label_state.label = "Unknow";
				break;
		}
 	}
 	
 	
	[CCode (instance_pos = -1)]
	public void quit(Button source){
		//connection.logoff();
		Gtk.main_quit();
	}
	
	[CCode (instance_pos = -1)]
	public void auth(Button sourve){
		connection.auth() ;
	}
	
	[CCode (instance_pos = -1)]
	public void logoff(Button source){
		connection.logoff();
	}
}

private interface NIC_Chooser : GLib.Object{
	//public void show();
	//public void hide();
	
	public signal void nic_change ();
	
	public abstract string get_nic();
	
	public abstract bool set_nic(string nic);
	
	public abstract Widget get_widget();
	
	public abstract string get_ip();
	public abstract string get_netmask();
}

//caicai , do you know this is factory mode ?
private static NIC_Chooser new_NIC_Chooser(){
	try {
		return new NIC_Box() ;
	}catch (GLib.Error e){
		return new NIC_Entry() ;
	}
}

private class NIC_Entry : GLib.Object , NIC_Chooser {
	private Entry entry_box ;
	
	public NIC_Entry() {
		this.entry_box = new Entry();
		this.entry_box.changed.connect(()=>{this.nic_change();});
	}
	
	public string get_nic(){
		return this.entry_box.text ;
	}
	
	public bool set_nic(string nic){
		this.entry_box.text = nic ;
		return true ;
	}
	
	public Widget get_widget(){
		return this.entry_box ;
	}

	public string get_ip(){
		return "不可用";
	}
	public string get_netmask(){
		return "不可用";
	}
}


private class NIC_Box : GLib.Object , NIC_Chooser {
	private ComboBox box ;
	
	public NIC_Box() throws DBus.Error, GLib.Error{
		this.box = new ComboBox.text() ;
		this.init_all_nic();
		this.box.changed.connect(()=>{this.update_device();this.nic_change();});
	}
	
	public string get_nic(){
		for ( int i = 0 ; i < nics.length ; i++ ){
				if ( i == box.get_active() ) {
					return nics[i].nic ;
				}
		}
		warning("This shoud not happen... will use eth0");
		return "eth0" ;
	}
	
	public bool set_nic(string nic){
		for ( int i = 0 ; i < nics.length ; i++ ){
				if ( nics[i].nic == nic ) {
					box.set_active(i) ;
					this.nic_change();
					return true;
				}
		}
		this.nic_change();
		return false ;
	}
	
	public Widget get_widget(){
		return this.box ;
	}
	
	struct NIC {
		public string nic;
		public string detail;
	}
	private NIC[] nics ;
	
	
	DBus.Connection dbus_con ;
	dynamic DBus.Object device ;
	dynamic DBus.Object nm 	;
	public void init_all_nic() throws DBus.Error, GLib.Error{
		this.dbus_con =  DBus.Bus.get(DBus.BusType.SYSTEM);
		this.nm = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 "/org/freedesktop/NetworkManager", 
				 "org.freedesktop.NetworkManager");
		ObjectPath[unowned] devices_path = nm.GetDevices();
		this.nics = new NIC[devices_path.length];
		for (int i = 0 ; i < devices_path.length ; i++ ){
			string path = devices_path[i];
			dynamic DBus.Object device_c = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 path, 
				 "org.freedesktop.NetworkManager.Device");
			nics[i].nic = device_c.Interface ;
			nics[i].detail = nics[i].nic + get_nic_type(device_c) ;
			message("find nic : %s/%s " , nics[i].nic ,nics[i].detail );
			this.box.append_text(nics[i].detail);
		}
			
	}
	private string get_nic_type(dynamic DBus.Object device_c){
		uint32 type_id = device_c.DeviceType ;
		switch ( type_id ){
			case 1: return " 有线网络接口" ; 
			case 2: return " 无线网络接口" ; 
			default: return " 网络接口" ; 	
		}
	}
	private void update_device(){
		string nic_goal = this.get_nic();
		ObjectPath[unowned] devices_path = nm.GetDevices();
		for (int i = 0 ; i < devices_path.length ; i++ ){
			string path = devices_path[i];
			dynamic DBus.Object device_c = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 path, 
				 "org.freedesktop.NetworkManager.Device");
			string s = device_c.Interface ;
			if ( nic_goal ==  s){
				this.device = device_c ;
				return ;
			}
		}
	}

	public string get_ip(){
		if ( this.device == null ){
			return "不可用";
		}
		/* seems to be a vala bug?
		//try {
			string ip4conf_path = (owned) this.device.Ip4Config ;
			if ( ip4conf_path == "/" ){
				return "尚未配置" ;
			}
			dynamic DBus.Object ip4conf = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 ip4conf_path, 
				 "org.freedesktop.NetworkManager.IP4Config");
			//uint32[][] add = ip4conf.Addresses ;
			//uint32 ip_i = add[0][1];
			//message("IP_i:%u",ip_i);
		//}catch (DBus.Error e){
		
		//}
		*/
		return "不可用";
	}
	public string get_netmask(){
		return "不可用";
	}
}


