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
			this.init_content(builder);
			main_win.show_all();
		}catch (GLib.Error e){
			//TODO
			return ;
		}
		
		connection.notify["state"].connect(gtk_state_change);
		Table table = builder.get_object ("table_set") as Table;
		entry_nic.hide();
 			NICBox box ;
		try{ 
			box = new NICBox();
 		}catch (GLib.Error e){
 			message("Cann't connect to NetworkManager.");
 			entry_nic.show_all();
 		}
		table.attach(box.get_box(),1,2,2,3,AttachOptions.FILL,AttachOptions.FILL,10,0);
		box.get_box().show_all();
		Gtk.main ();
 	}
 	
 	Entry entry_username	;
 	Entry entry_password	;
 	Entry entry_nic	;
 	private void init_content(Builder builder){
 		this.entry_username = builder.get_object ("entry_username") as Entry ;
 		this.entry_password = builder.get_object ("entry_password") as Entry ;
 		this.entry_nic	= builder.get_object ("entry_nic") as Entry ;
 		
 		this.entry_username.text = conf.user_name ;
 		this.entry_password.text = conf.user_password ;
 		this.entry_nic.text	= conf.NIC ;
 	}
 	
 	public void gtk_state_change(){
 		stdout.printf("state:%i\n",connection.state);
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


public class NICBox  {
	
	private ComboBox box = new ComboBox.text();
	struct NIC {
		public string nic;
		public string detail;
	}
	private NIC[] nics ;
	
	public ComboBox get_box() {
		return this.box ;
	}
	
	public NICBox(){
		this.get_all_nic();
	}
	
	public void get_all_nic() throws DBus.Error, GLib.Error{
		DBus.Connection dbus_con =  DBus.Bus.get(DBus.BusType.SYSTEM);
		dynamic DBus.Object nm = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 "/org/freedesktop/NetworkManager", 
				 "org.freedesktop.NetworkManager");
		ObjectPath[unowned] devices_path = nm.GetDevices();
		this.nics = new NIC[devices_path.length];
		for (int i = 0 ; i < devices_path.length ; i++ ){
			string path = devices_path[i];
			dynamic DBus.Object device = dbus_con.get_object 
				("org.freedesktop.NetworkManager",
				 path, 
				 "org.freedesktop.NetworkManager.Device");
			nics[i].nic = device.Interface ;
			nics[i].detail = nics[i].nic + get_nic_type(device) ;
			message("find nic : %s/%s " , nics[i].nic ,nics[i].detail );
			this.box.append_text(nics[i].detail);
		}
			
	}
	private string get_nic_type(dynamic DBus.Object device){
		uint32 type_id = device.DeviceType ;
		switch ( type_id ){
			case 1: return " 有线网络接口" ; 
			case 2: return " 无线网络接口" ; 
			default: return " 网络接口" ; 	
		}
	}

} 
