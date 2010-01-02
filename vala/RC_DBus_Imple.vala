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
	public int32	SetPassword(string passwd) throws DBus.Error{
		this.UserPassword = passwd ;
		return 0;
	}
	//public signal void StateChanged();
	
	public int32	State 		{get ; protected set ; default= 0;}
	
	public string Message 	{owned get ;protected set ; default= "#!None.";}
	
	public string NetInterface	{owned get ; private set ; default= "eth0";}
	
	public string UserName 	{owned get ; set ; default = "#!None.";}
	
	private string UserPassword	{owned get ; set ; default = "#!None.";}
	
	public int32	AuthenticationMode {get;set;default = 1 ;}
	
	public int32	EchoInterval	{get; set ; default = 25 ;}
	
	public bool	IntelligentReconnect {get; set ; default = true ;}
	
	public bool	AutoConnect	{get; set ; default = true ;}
	
	public string FakeVersion	{owned get; set ; default = "3.33" ;}
	
	public int32	DHCPmode	{get;set;default = 0 ;}
					
	public string PingHost	{owned get; set ; default = "4.3.2.1" ;}
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
