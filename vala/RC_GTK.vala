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
 
 
public class Gui : GLib.Object {
 	
 	Window main_win ;
 	public Gui(){
 		string[] ab_args = {""};
 		Gtk.init (ref ab_args);
 		try {
			var builder = new Builder ();
			builder.add_from_file ("./gtkbuilder.glade");
			builder.connect_signals (this);
			main_win = builder.get_object ("window") as Window;
			this.init_content(builder);
			
		}catch (Error e){
			//TODO
			return ;
		}
		
		connection.notify["state"].connect(gtk_state_change);
		main_win.show_all();
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

 
