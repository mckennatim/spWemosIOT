#ifndef config_h
#define config_h

	#include <ESP8266WebServer.h>
	extern ESP8266WebServer server;
	extern char devid[9];
	extern char devpwd[24];
	extern char mqtt_server[40];
	extern char mqtt_port[6];
	extern char node_port[6];
	extern bool NEEDS_RESET;
	void getOnline();

#endif