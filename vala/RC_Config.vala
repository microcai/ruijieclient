/*
 * RC_Config.vala
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
 
using Gee;
 
public class RC_Config :  HashMap<string,string> {

	public RC_Config () {
		//default values
		this.set("userName",		"");
		this.set("userPassword",	"");
		this.set("authMode",		"1");
		this.set("NIC",		"eth0");
		this.set("echoInterval",	"20");
		this.set("autoReconnect",	"1");
		this.set("fakeVersion",	"3.33");
		this.set("DHCPmode",		"0");
		this.set("pingHost",		"1.2.3.4");
	}
	
	public void load_from_file(string path){
	
	}
}
