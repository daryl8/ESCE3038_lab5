#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>   
#include "env.h"  

const int light1 = 15;
const int light2 = 2;
const int light3 = 4;



bool states [8] [3] = {
  
  {false,false,false},
  {false,false,true},
  {false,true,false},
  {false,true,true},
  {true,false,false},
  {true,false,true},
  {true,true,false},
  {true,true,true},};

void setup(){

  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);


  //LED array showing states

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID,WIFI_PASS);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(ENDPOINT);

    for (int i =0; i < 8; i++){

    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", keyvalue);

    StaticJsonDocument<1024> doc;
    String httpRequestData;

    doc["light_switch_1"] = states[i][0];
    doc["light_switch_2"] = states[i][1];
    doc["light_switch_3"] = states[i][2];

    digitalWrite(light1, states[i][0]);
    digitalWrite(light2, states[i][1]);
    digitalWrite(light3, states[i][2]);

      serializeJson(doc, httpRequestData);
    

    int httpResponseCode = http.PUT(httpRequestData);
    String http_response;

    if(httpResponseCode>0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);
      

    }
    else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }

    delay(1000);
    }

    // free resource 
    http.end();
      
  }
  else {
    Serial.print("WiFi Disconneted");
    
  }
}