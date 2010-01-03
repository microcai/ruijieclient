using GLib;


[DBus (name="com.googlecode.ruijieclient")]
public interface RuijieDbusInterface  {

	//This is a draft of Ruijieclient DBus Server API.
	//Still not useable.

	//==================signals
	
	public signal void StateChanged();
	
	//==================property
	/*
	public abstract int32	State 		{ get ; protected set ; default= 0;}
	
	public abstract string 	Message 	{owned get ;protected set ; default= "#!None.";}
	
	public abstract string 	NetInterface	{owned get ; private set ; default= "eth0";}
	
	public abstract string 	UserName 	{owned get ; set ; default = "#!None.";}
	
	//public abstract string 	UserPassword	{owned get ; set ; default = "#!None.";}
	
	public abstract int32	AuthenticationMode	{get;set;default =1  ;}
	
	public abstract int32	EchoInterval	{get; set ; default = 25 ;}
	
	public abstract bool	IntelligentReconnect {get; set ; default = true ;}
	
	public abstract bool	AutoConnect	{get; set ; default = true ;}
	
	public abstract string 	FakeVersion	{owned get; set ; default = "3.33" ;}
	
	public abstract int32	DHCPmode	{get;set;default =0  ;}
	
	public abstract string 	PingHost	{owned get; set ; default = "1.2.3.4" ;}
	*/
	
	
	//====================methods
	public abstract int32	Auth() throws DBus.Error;
	
	public abstract int32	DisAuth() throws DBus.Error;
	
	public abstract int32	get_State()throws DBus.Error;
	//public abstract int32	set_State(int32 v)throws DBus.Error;
	
	public abstract string 	get_Message() throws DBus.Error;
	//public abstract string 	set_Message(string v) throws DBus.Error;
	
	public abstract string 	get_NetInterface() throws DBus.Error;
	public abstract string 	set_NetInterface(string v) throws DBus.Error;
	
	public abstract string 	get_UserName() throws DBus.Error;
	public abstract string 	set_UserName(string v) throws DBus.Error;
	
	public abstract string 	set_UserPassword(string v) throws DBus.Error;
	
	public abstract int32	get_AuthenticationMode() throws DBus.Error;
	public abstract int32	set_AuthenticationMode(int32 v) throws DBus.Error;
	
	public abstract int32	get_EchoInterval() throws DBus.Error;
	public abstract int32	set_EchoInterval(int32 v) throws DBus.Error;
	
	public abstract bool	get_IntelligentReconnect() throws DBus.Error;
	public abstract bool	set_IntelligentReconnect(bool v) throws DBus.Error;
	
	public abstract bool	get_AutoConnect() throws DBus.Error;
	public abstract bool	set_AutoConnect(bool v) throws DBus.Error;
	
	public abstract string 	get_FakeVersion() throws DBus.Error;
	public abstract string 	set_FakeVersion(string v) throws DBus.Error;
	
	public abstract int32	get_DHCPmode() throws DBus.Error;
	public abstract int32	set_DHCPmode(int32 v) throws DBus.Error;
	
	public abstract string 	get_PingHost() throws DBus.Error;
	public abstract string 	set_PingHost(string v) throws DBus.Error;
	
	
	

}
