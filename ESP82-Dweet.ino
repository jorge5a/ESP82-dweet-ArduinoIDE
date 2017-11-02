
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#define TRIGGER_PIN 0             //Reset WiFi config button

#define FRECUENCY 55000          //frecuency upload time in microseconds 
const char* host = "dweet.io";


/////////////////////// //
// Dweet configuration //
////////////////////////

const char* THINGNAME ="jorge5a";  //use any name you want
//////////////////////
// WiFi Definitions //
//////////////////////
char *AP="dweet_Sensor";
const char WiFiAPPSK[] = "test";



//////////////////////
// WiFi Manager    //
////////////////////
WiFiManager wifiManager;
WiFiClient client;


char serialdata;
int i;


void setup() {
//delay 10 second. If power down the gateway must be online before this

Serial.begin(115200);
pinMode(TRIGGER_PIN, INPUT);

//sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);
  
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect(AP,WiFiAPPSK)) {
    Serial.print("failed to connect and hit timeout");
    Serial.println(" Push button to reset Wifi configuration and start config menu");
    
    
    delay(5000);
  } 
   attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), configWiFi, RISING);

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
 
}

void loop() {
  
 sendValues(readSensors());
 
 
delay(FRECUENCY);
}

void configWiFi(){
  wifiManager.resetSettings();
    if(!wifiManager.autoConnect(AP,WiFiAPPSK)) {
    Serial.println("Reconfiguration called");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(15000);
  } 

}

bool sendValues(String values){


if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return false;
  }
  
  Serial.print("Requesting URL: ");
  

  String str = "/dweet/for/"+String(THINGNAME)+"?"+ values ;
 Serial.println("----Sending----");
 Serial.println(str);
 client.print(String("GET ") + str + " HTTP/1.1\r\n" +
 "Host: " + host + "\r\n" + 
 "Connection: close\r\n\r\n");
 
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
Serial.println("closing connection"); 
return true;
}

String readSensors(){
String s="";
//Function where read all sensors and create de String s to send
//s format  key=value&key=value....
//my first sensor
s+="temperature=";
s+=34;
//my second sensor
s+="&humidity=";
s+=45;

return s;

Serial.println(s);

}


