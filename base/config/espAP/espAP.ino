#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "ESPap";
char ssids[500];

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/html", "<h1>You are yok connected</h1>");
}

void getSSIDs()
{
  char str[50];
  WiFi.mode(WIFI_STA);
  // WiFi.disconnect();
  delay(200);  
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found"); 
  strcpy(ssids,"{\"ssids\": [");
  // char tr[n][25];
  // int tr2[n][2];
  // for (int i = 0; i < n; ++i){
  //   strcpy(tr[i],WiFi.SSID(i).c_str());
  //   tr2[i][0]=WiFi.RSSI(i);
  //   tr2[i][1]=WiFi.encryptionType(i);
  // }
  // for (int i = 0; i < n; ++i){
  //   sprintf(str,"{\"ssid\": \"%s\", \"rssi\": %d, \"enc\": %d }", tr[i], tr2[i][0], tr2[i][1]);
  //   //sprintf(str,"[\"ssid\": \"%s\"]",  tr[i]);
  //   strcat(ssids,str);
  //   if(i<n-1){strcat(ssids, ", ");}
  // }
  strcat(ssids,"]}");
}

void setupAP(){
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  getSSIDs();
  setupAP();
}

void loop() {
  server.handleClient();
}

// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>

// char *espssid = "espAPsb";
// char ssid[40];
// char pwd[24];
// char devid[9];
// char ip[16];
// char port[5];
// char ssids[500];
// uint8_t idx[5];
// bool NEEDS_RESET = 0;

// ESP8266WebServer server(80);

// void handleRoot(){
//   server.send(200, "text/html", "<h1>You are connected</h1>");
// }
// //http://192.168.4.1/config?ssid=street_no_vale2&pwd=jjjjjjjj&devid=CYURD001&ip=10.0.1.104&port=3332
// //http://192.168.4.1/config?ssid=Uli Fence&pwd=jjjjjjjj&devid=CYURD001&ip=10.0.1.100&port=3332
// //http://192.168.4.1/config?ssid=Uli Fence&pwd=jjjjjjjj&devid=CYURD002&ip=162.217.250.109&port=3332
// //http://192.168.4.1/config?ssid=CenturyLink6185&pwd=jamesett&devid=CYURD001&ip=162.217.250.109&port=3332
// void handleConfig(){
//   Serial.println(server.arg("ssid").c_str());
//   Serial.println(server.arg("pwd").c_str());
//   Serial.println(server.arg("devid").c_str());
//   Serial.println(server.arg("ip").c_str());
//   Serial.println(server.arg("port").c_str());
//   server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
// }

// void getSSIDs()
// {
//   char str[50];
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(200);  
//   int n = WiFi.scanNetworks();
//   Serial.print(n);
//   Serial.println(" networks found"); 
//   strcpy(ssids,"{\"ssids\": [");
//   char tr[n][25];
//   int tr2[n][2];
//   for (int i = 0; i < n; ++i){
//     strcpy(tr[i],WiFi.SSID(i).c_str());
//     tr2[i][0]=WiFi.RSSI(i);
//     tr2[i][1]=WiFi.encryptionType(i);
//   }
//   for (int i = 0; i < n; ++i){
//     sprintf(str,"{\"ssid\": \"%s\", \"rssi\": %d, \"enc\": %d }", tr[i], tr2[i][0], tr2[i][1]);
//     //sprintf(str,"[\"ssid\": \"%s\"]",  tr[i]);
//     strcat(ssids,str);
//     if(i<n-1){strcat(ssids, ", ");}
//   }
//   strcat(ssids,"]}");
// }

// void setupAP(){
//   WiFi.softAP(espssid);
//   server.on("/", handleRoot);
//   server.on("/config", handleConfig);
//   server.begin();
//   Serial.println();
//   Serial.print("connected as AP ");
//   Serial.println(espssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.softAPIP());  
// }

// void getOnline(){
//   WiFi.begin("street_novale2", "jjjjjjjj");
//   int tries =0;
//   int success=1;
//   while (WiFi.status() != WL_CONNECTED ) {
//     delay(500);
//     Serial.print(".");
//     tries++;
//     if (tries==15){
//       success=0;
//       Serial.println("WiFi not connected");
//       //getSSIDs();
//       //Serial.println(ssids);
//       //WiFi.disconnect();
//       //delay(2000);
//       setupAP();
//       break;
//     }
//   }
//   if (success){
//     Serial.println("");
//     Serial.println("WiFi connected");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP()); 
//     Serial.println(ssid); 
//   } 
// }

// void setup(){
//   Serial.begin(115200);
//   Serial.println();
//   Serial.println("--------------------------");
//   Serial.println("ESP8266 incrbuild");
//   Serial.println("--------------------------");
//   //getOnline();//config.cpp
//   setupAP();
// }

// void loop(){
//   server.handleClient();
// }