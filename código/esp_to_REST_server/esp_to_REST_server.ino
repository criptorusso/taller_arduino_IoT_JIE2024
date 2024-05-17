
#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SERVER_IP "192.168.0.10:5000" //localhost

#ifndef STASSID
#define STASSID "AccessPoint"
#define STAPSK  "Password"
#endif
WiFiClient client;

String data;

void setup() {
  Serial.begin(9600);
  while(!Serial.available()){};
  Serial.println('ok');
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  //  Serial.println("");
  //  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //delay(1000);
  Serial.flush();
}

void loop() {
  data = "";
  Serial.println('ok');
  delay(500);
  data = Serial.readString();
  postAlarma(data);
  return;
}

///////////////////// FUNCIONES /////////////////////////
String postAlarma(String data) {

  String payload;
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    //Serial.print("[HTTP] begin...\n");
    // configure targed server and url
    http.begin(client, "http://" SERVER_IP "/alarma?temp=" + data); //HTTP
    http.addHeader("Content-Type", "text/plain");
    //Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    //Serial.println(postData);
    int httpCode = http.POST("enviar alarma");
    // httpCode will be negative on error
    payload = http.getString();     
    http.end();
  
  }
  return payload;  
}