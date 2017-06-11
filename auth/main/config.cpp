#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "config.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//{"devid":"CYURD009","devpwd":"mabibi","mqtt_server":"sitebuilt.net","mqtt_port":"1883","node_port":"3332"}

char devid[9] ="CYURD0xx";
char owner[254] ="fred@sitebuilt.net";
char pwd[24]= "mabibi";
char mqtt_server[60];
char mqtt_port[6];
char sensor_type[24];

void reconfig(char pload[]){
  Serial.println(pload);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(pload);
  json.printTo(Serial);
  if (json.success()) {
    Serial.println("\nparsed json");

    strcpy(devid, json["devid"]);
    strcpy(owner, json["owner"]);
    strcpy(pwd, json["pwd"]);
    strcpy(mqtt_server, json["mqtt_server"]);
    strcpy(mqtt_port, json["mqtt_port"]);
    strcpy(sensor_type, json["sensor_type"]);
  } else {
    Serial.println("failed to load json config");
  }  
}

void saveConfig(){
  Serial.println("saving config");
  //DynamicJsonBuffer jsonBuffer;
  StaticJsonBuffer<196> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["devid"] = devid;
  json["owner"] = owner;
  json["pwd"] = pwd;
  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;
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
        reconfig(buf.get());
        // //DynamicJsonBuffer jsonBuffer;
        // StaticJsonBuffer<500> jsonBuffer;
        // JsonObject& json = jsonBuffer.parseObject(buf.get());
        // json.printTo(Serial);
        // if (json.success()) {
        //   Serial.println("\nparsed json");

        //   strcpy(devid, json["devid"]);
        //   strcpy(owner, json["owner"]);
        //   strcpy(pwd, json["pwd"]);
        //   strcpy(mqtt_server, json["mqtt_server"]);
        //   strcpy(mqtt_port, json["mqtt_port"]);
        //   strcpy(sensor_type, json["sensor_type"]);
        // } else {
        //   Serial.println("failed to load json config");
        // }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }  
}

void getOnline(){
  readConfig();
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("connectEspAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println(devid);
  Serial.println(owner);
  Serial.println(pwd);
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  //Serial.println(sensor_type);
}