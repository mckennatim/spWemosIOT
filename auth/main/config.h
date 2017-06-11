#ifndef config_h
#define config_h

	#include <ESP8266WebServer.h>
	extern ESP8266WebServer server;
	extern char devid[9];
	extern char owner[254];
	extern char pwd[24];
	extern char mqtt_server[60];
	extern char mqtt_port[6];
	extern char sensor_type[24];

	extern bool NEEDS_RESET;
	void getOnline();
	void readConfig();
	void saveConfig();
	void reconfig(char pload[]);

#endif