#include <DHT.h>

#define DHTTYPE DHT11 
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE);


void setup(){
  Serial.begin(9600);
  Serial.println("DHT11 test!");
  dht.begin();  
}

void loop(){
	delay(2000);
	//cast to int(rounded)
  int h = (int)dht.readHumidity()+17;
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = (int)(dht.readTemperature(true)+.5);	
  Serial.print("Temperature = ");
  Serial.println(f);
  Serial.print("Humidity = ");
  Serial.println(h);

}
