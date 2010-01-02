using GLib;

int main(){
	RuijieDbusServer dbuss = new RuijieDbusServer();
	dbuss.init_dbus_server();
	dbuss.loop_run();
	return 0;
}
