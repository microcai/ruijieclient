using GLib;

public class RuijieDbusObject : Object , RuijieDbusInterface {

	public void	tell_State_Changed(int state){
		this.State = state ;
		StateChanged();
	}
	public void	tell_Message(string msg){
		this.Message = msg ;
	}

	//implement the dbus interface
	
	public int32	Auth() throws DBus.Error{
		warning("The Auth Method will do nothing till now!");
		return 1;
	}
	public int32	DisAuth() throws DBus.Error{
		warning("The DisAuth Method will do nothing till now!");
		return 1;
	}
	public signal void StateChanged();
	
	
	//------------------------------------------------------
	private int32 _state;
	public int32	get_State()throws DBus.Error{
		return this._state ;
	}
	//this will not be part of dbus interface
	public int32	set_State(int32 v)throws DBus.Error;
	
	
	//------------------------------------------------------
	private string _message;
	public string get_Message() throws DBus.Error{
		return this._message;
	}
	//this will not be part of dbus interface
	public string set_Message(string v) throws DBus.Error;
	
	
	//------------------------------------------------------
	private string _nic;
	public string get_NetInterface() throws DBus.Error{
		return this._nic ;
	}
	public string set_NetInterface(string v) throws DBus.Error{
		//TODO do we need to check it here?
		this._nic = v ;
		return this._nic;
	}
	
	
	//------------------------------------------------------
	private string _user_name;
	public string get_UserName() throws DBus.Error{
		return this._user_name;
	}
	public string set_UserName(string v) throws DBus.Error{
		this._user_name = v;
		return  this._user_name;
	}
	
	
	//------------------------------------------------------
	private string _user_pwd;
	public string set_UserPassword(string v) throws DBus.Error{
		this._user_pwd = v;
	}
	
	
	//------------------------------------------------------
	private int32 _auth_mode;
	public int32	get_AuthenticationMode() throws DBus.Error{
		return this._auth_mode;
	}
	public int32	set_AuthenticationMode(int32 v) throws DBus.Error{
		//TODO check it
		this._auth_mode = v;
		return this._auth_mode ;
	}
	
	
	//------------------------------------------------------
	public int32	get_EchoInterval() throws DBus.Error;
	public int32	set_EchoInterval(int32 v) throws DBus.Error;
	
	
	//------------------------------------------------------
	public bool	get_IntelligentReconnect() throws DBus.Error;
	public bool	set_IntelligentReconnect(bool v) throws DBus.Error;
	
	
	//------------------------------------------------------
	public bool	get_AutoConnect() throws DBus.Error;
	public bool	set_AutoConnect(bool v) throws DBus.Error;
	
	
	//------------------------------------------------------
	public string get_FakeVersion() throws DBus.Error;
	public string set_FakeVersion(string v) throws DBus.Error;
	
	
	//------------------------------------------------------
	public int32	get_DHCPmode() throws DBus.Error;
	public int32	set_DHCPmode(int32 v) throws DBus.Error;
	
	
	//------------------------------------------------------
	public string get_PingHost() throws DBus.Error;
	public string set_PingHost(string v) throws DBus.Error;
	
	
	
}

public class RuijieDbusServer : Object {

	MainLoop loop = new MainLoop (null ,false);
	private static dynamic DBus.Object bus ;
	private static DBus.Connection con ;
	private static RuijieDbusObject dbus_obj ;
	public int init_dbus_server(){
		if (dbus_obj != null ){
			return 2;
		}
		try {
			con = DBus.Bus.get (DBus.BusType.SESSION);
			bus = con.get_object (	"org.freedesktop.DBus"	,
						"/org/freedesktop/DBus"	,
						"org.freedesktop.DBus"	);
		        uint request_name_result = bus.request_name (	"com.googlecode.ruijieclient", (uint) 0);
		        if (request_name_result == DBus.RequestNameReply.PRIMARY_OWNER) {
		        	dbus_obj = new RuijieDbusObject ();
				con.register_object ("/com/googlecode/ruijieclient", dbus_obj);
				return 0;
		        }else{
		        	warning("Find Ruijie DBus Server is already running. ");
		        	return 1;
		        }
                }catch (Error e) {
			stderr.printf ("Got error when init the dbus server: %s\n", e.message);
		}
		return 0;
	}
	public int loop_run(){
		loop.run ();
		return 0;
	}
}
