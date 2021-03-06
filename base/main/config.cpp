#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "config.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//{"devid":"CYURD009","devpwd":"mabibi","mqtt_server":"sitebuilt.net","mqtt_port":"1883","node_port":"3332"}

char devid[9] ="CYURD0xx";
char devpwd[24]= "mabibi";
char mqtt_server[40];
char mqtt_port[6];
char node_port[6];
char sensor_type[24];

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void readConfig(){
  //clean FS, for testing
  //SPIFFS.format();  
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
        //DynamicJsonBuffer jsonBuffer;
        StaticJsonBuffer<196> jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(devid, json["devid"]);
          strcpy(devpwd, json["devpwd"]);
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(node_port, json["node_port"]);
          strcpy(sensor_type, json["sensor_type"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }  
}

void saveConfig(){
  Serial.println("saving config");
  //DynamicJsonBuffer jsonBuffer;
  StaticJsonBuffer<196> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["devid"] = devid;
  json["devpwd"] = devpwd;
  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;
  json["node_port"] = node_port;
  json["sensor_type"] = sensor_type;

  Serial.println(jsonBuffer.size());
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }
  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();  
}

void getOnline(){
  readConfig();
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_devid("devid", "device id", devid, 9);
  WiFiManagerParameter custom_devpwd("devpwd", "device pwd", devpwd, 24);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("mport", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_node_port("nport", "node port", node_port, 6);
  WiFiManagerParameter custom_sensor_type("sensortype", "ds18b20|dht|hrc", sensor_type, 24);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&custom_devid);
  wifiManager.addParameter(&custom_devpwd);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_node_port);
  wifiManager.addParameter(&custom_sensor_type);

  //reset settings - for testing
  //wifiManager.resetSettings();

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

  Serial.println("connected...yeey :)");

  strcpy(devid, custom_devid.getValue());
  strcpy(devpwd, custom_devpwd.getValue());
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(node_port, custom_node_port.getValue());
  strcpy(sensor_type, custom_sensor_type.getValue());

  if (shouldSaveConfig) {
    saveConfig();
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println(devid);
  Serial.println(devpwd);
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  Serial.println(node_port);
  Serial.println(sensor_type);
}