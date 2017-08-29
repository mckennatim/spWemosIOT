#include <FS.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 cleaning: wait ...");
  Serial.println("--------------------------");
  SPIFFS.format();

  WiFiManager wifiManager;
  wifiManager.resetSettings();
  WiFi.disconnect();
}

void loop(){

}