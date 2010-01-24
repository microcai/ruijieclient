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
		/*
		entry_nic.hide();
 			NICBox box ;
		try{ 
			box = new NICBox();
 		}catch (GLib.Error e){
 			message("Cann't connect to NetworkManager.");
 			entry_nic.show_all();
 		}
		box.get_box().show_all();*/
		Gtk.main ();
 	}
 	
 	Entry entry_username	;
 	Entry entry_password	;
 	NIC_Chooser nic_chooser	;
 	private void init_content(Builder builder){
 		this.entry_username = builder.get_object ("entry_username") as Entry ;
 		this.entry_password = builder.get_object ("entry_password") as Entry ;
 		this.nic_chooser	= new_NIC_Chooser();
 		
 		this.entry_username.text = conf.user_name ;
 		this.entry_password.text = conf.user_password ;
 		this.nic_chooser.set_nic	(conf.NIC) ;
 		
		Table table = builder.get_object ("table_set") as Table;
		table.attach(nic_chooser.get_widget(),1,2,2,3,AttachOptions.FILL,AttachOptions.FILL,10,0);
 		
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

private interface NIC_Chooser : GLib.Object{
	//public void show();
	//public void hide();
	
	public signal void nic_change ();
	
	public abstract string get_nic();
	
	public abstract bool set_nic(string nic);
	
	public abstract Widget get_widget();
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


}


private class NIC_Box : GLib.Object , NIC_Chooser {
	private ComboBox box ;
	
	public NIC_Box() throws DBus.Error, GLib.Error{
		this.box = new ComboBox.text() ;
		this.init_all_nic();
		this.box.changed.connect(()=>{this.nic_change();});
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
					return true;
				}
		}
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
	
	public void init_all_nic() throws DBus.Error, GLib.Error{
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

