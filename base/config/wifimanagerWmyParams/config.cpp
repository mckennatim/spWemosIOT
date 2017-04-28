#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "config.h"
#include <Arduino.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

char devid[9] ="CYURD0xx";
char devpwd[24]= "mabibi";
char mqtt_server[40];
char mqtt_port[6];
char node_port[6];

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void getOnline(){
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(devid, json["devid"]);
          strcpy(devpwd, json["devpwd"]);
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(node_port, json["node_port"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_devid("devid", "device id", devid, 9);
  WiFiManagerParameter custom_devpwd("devpwd", "device pwd", devpwd, 24);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("mport", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_node_port("nport", "node port", node_port, 6);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
 // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_devid);
  wifiManager.addParameter(&custom_devpwd);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_node_port);

  //reset settings - for testing
  wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("connectEspAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(devid, custom_devid.getValue());
  strcpy(devpwd, custom_devpwd.getValue());
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(node_port, custom_node_port.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    //DynamicJsonBuffer jsonBuffer;
    StaticJsonBuffer<196> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["devid"] = devid;
    json["devpwd"] = devpwd;
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["node_port"] = node_port;

    Serial.println(jsonBuffer.size());
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    // end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println(devid);
  Serial.println(devpwd);
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  Serial.println(node_port);
}