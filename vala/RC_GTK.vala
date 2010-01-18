/*
 * RC_GTK.vala
 *
 *  Created on: 2010-1-12
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
			main_win.show_all();
		}catch (Error e){
			//TODO
			return ;
		}
		Gtk.main ();
		
 	}
 	
	[CCode (instance_pos = -1)]
	public void quit(Button source){
		//connection.logoff();
		Gtk.main_quit();
	}
	
	[CCode (instance_pos = -1)]
	public void auth(Button sourve){
		try{
			connection.auth() ;
		}catch (AuthFailed e) {
			message("Auth Failed.//TODO gui info");
		}
	}
	
	[CCode (instance_pos = -1)]
	public void logoff(Button source){
		connection.logoff();
	}
}

 
