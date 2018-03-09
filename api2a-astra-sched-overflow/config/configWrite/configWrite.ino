#include <FS.h> 
#include <ArduinoJson.h> 

char devid[9] ="CYURD007";
char devpwd[24]= "mabibi";
char mqtt_server[40] = "sitebuilt.net";
char mqtt_port[6] = "1883" ;
char node_port[6] = "3332" ;
char sensor_type[24]= "ds18b20";

//flag for saving data
bool shouldSaveConfig = true;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void saveConfig(){
  if (SPIFFS.begin()) {
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

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
  } else {
    Serial.println("failed to mount FS");
  } 
}

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 clean");
  Serial.println("--------------------------");
  saveConfig();
}

void loop(){

}