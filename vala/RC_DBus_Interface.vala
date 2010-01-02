using GLib;


[DBus (name="com.googlecode.ruijieclient")]
public interface RuijieDbusInterface  {

	//This is a draft of Ruijieclient DBus Server API.
	//Still not useable.

	//==================signals
	
	public signal void StateChanged();
	
	//==================property
	
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
	
	//====================methods
	public abstract int32	Auth() throws DBus.Error;
	
	public abstract int32	DisAuth() throws DBus.Error;
	
	public abstract int32	SetPassword(string passwd) throws DBus.Error;
	
	

}
