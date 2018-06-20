#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <DHT.h>
#include "config.h" //getOnline() devid owner pwd
#include "STATE.h"
#include "MQclient.h" //globals(extern) NEW_MAIL, itopic, ipayload + Console + handleCallback()
#include "Reqs.h"
#include "Sched.h"

Sched sched;

prgs_t prgs {
    {0,255,1,2,{{0,0,76,73}}},
    {1,255,1,2,{{0,0,74,69}}},
    {2,255,1,1,{{0,0,0}}},
    {3,255,1,1,{{0,0,0}}},
    {4,255,1,1,{{0,0,0}}}
  };
flags_t f {0,0,5,28,0,0,31,0,0,0,{0,0,0,0,0}};
state_t sr {{44,0,80,50,0},{33,0,90,60,0},{0,0},{0,0},{0,0}};
ports_t po {5, 16, 15, 13, 12, 4, 14};
labels_t la; //subsribedTo[], numcmds

#define ONE_WIRE_BUS po.ds18b20 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

#define DHTTYPE DHT22 
#define DHTPIN po.io14d5
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
Console console(devid, client);
Reqs req(devid, client);
MQclient mq(devid, owner, pwd);

void initShit(){
  pinMode(po.temp1, OUTPUT);
  pinMode(po.temp2, OUTPUT);
  pinMode(po.timr1, OUTPUT);
  pinMode(po.timr2, OUTPUT);
  pinMode(po.timr3, OUTPUT);
  pinMode(po.io14d5, INPUT);
  digitalWrite(po.temp1, LOW);
  digitalWrite(po.temp2, LOW);
  digitalWrite(po.timr1, LOW);
  digitalWrite(po.timr2, LOW);
  digitalWrite(po.timr3, LOW);
  digitalWrite(po.timr3, LOW);
}

void readTemps(){
  //Serial.println(sensor_type);
  int temp1;
  int temp2;
  if(strcmp(sensor_type, "dht")==0){
    temp1 = (int)(dht.readTemperature(true));
    temp2 = (int)dht.readHumidity();
  } else if(strcmp(sensor_type, "hrc")==0){
    temp1 =  digitalRead(po.ds18b20); 
    temp2 =  digitalRead(po.io14d5); 
  } else {
    DS18B20.requestTemperatures(); 
    temp1 = (int)DS18B20.getTempFByIndex(0);
    temp2 = (int)DS18B20.getTempFByIndex(1);
  }
  int dif = temp1-sr.temp1.temp;
  if(abs(dif)>1 && temp1 <120 && temp1>-20){
    sr.temp1.temp=temp1;
    sr.temp1.rec=1;
    int id = 0;
    int bit =pow(2,id);
    int mask = 31-bit;
    sched.adjHeat(id, sr.temp1, po.temp1);
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
  }
  int dif2 = temp2-sr.temp2.temp;
  if(abs(dif2)>1 && temp2 <120 && temp2>-20){
    sr.temp2.temp=temp2;
    sr.temp2.rec=1;
    int id = 1;
    int bit =pow(2,id);
    int mask = 31-bit;
    sched.adjHeat(id, sr.temp2, po.temp2);
    f.HAYsTATEcNG=f.HAYsTATEcNG | bit;
  }
}

void setSensorType(){
  Serial.print("sensor_type = ");
  Serial.println(sensor_type);
}

void getTime(){
  char* dd = "the time is being requested";
  Serial.println(dd);
  char time[20];
  strcpy(time,devid);
  strcat(time,"/time");  
  client.publish(time, dd, true);   
}

void dailyAlarm(){
  Serial.println("in daily alarm");
  Serial.println(devid);
  int minu = (10*((int)devid[6]-'0')+(int)devid[7]-'0')%16;
  Serial.print(hour());
  Serial.print(':');
  Serial.println(minute());
  Alarm.alarmRepeat(0,minu,0, getTime);
}

void setup(){
	Serial.begin(115200);
  EEPROM.begin(512);
	Serial.println();
	Serial.println("--------------------------");
  Serial.println("ESP8266 incrbuild");
  Serial.println("--------------------------");
  initShit();
  getOnline();//config.cpp
  setSensorType();
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(handleCallback); //in Req.cpp
  Alarm.timerOnce(10, dailyAlarm);//wait til system time has been requested and set
}

time_t before = 0;
time_t schedcrement = 0;
time_t inow;

void loop(){
  Alarm.delay(100);
  //server.handleClient();
  if(NEW_MAIL){
    req.processInc();
    //Serial.println("hay NEW_MAIL");
    NEW_MAIL=0;
  }  
  if(!client.connected() && !f.fORCErESET){
    Serial.println(owner);
     mq.reconn(client);
  }else{
    client.loop();
  }  
  if (f.CKaLARM>0){
    sched.ckAlarms(); //whatever gets scheduled should publish its update
    req.pubPrg(f.CKaLARM);
    req.pubTimr();
    f.CKaLARM=f.CKaLARM & 0; //11110 turnoff CKaLARM for 1
  }
  inow = millis();
  if(inow-schedcrement > f.cREMENT*1000){
    schedcrement = inow;
    if (f.IStIMERoN >0){
      sched.updTimers();
      req.pubTimr();
    }
    sched.ckRelays();
  }
  if (inow - before > 1000) {
    before = inow;
    if(f.aUTOMA){
      readTemps();
    }
    if(f.HAYsTATEcNG>0){
      // Serial.print("f.HAYsTATEcNG=");
      // Serial.println(f.HAYsTATEcNG);
      //console.log("example console.log entry");
      req.pubState(f.HAYsTATEcNG);
      f.HAYsTATEcNG=0;
    }
  } 
}