#include <FS.h>  

void setup(){
	Serial.begin(115200);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 clean");
  Serial.println("--------------------------");
  SPIFFS.format();
}

void loop(){

}