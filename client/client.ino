/* Creado por : Deimer Andres Morales
cambiar las credenciales en certificates.h 
por las propias */
#include "certificates.h"
#include <ESP8266WiFi.h>

#define room "1"

int inPinCall = D4;
int inPinEmer = D2;
int inPinNorm = D1;
String url1;
String url2;
String url3;
int Call, Emer, Norm;
boolean statCall, statNorm, statEmer;
IPAddress host(192, 168, 1, 70);
const uint16_t port = 80;

void setup() {
  
  url1 = "GET /?r="+String(room)+"&s=0 HTTP/1.1";
  url2 = "GET /?r="+String(room)+"&s=1 HTTP/1.1";
  url3 = "GET /?r="+String(room)+"&s=2 HTTP/1.1";
  
  const char* ssid = ssd;
  const char* passw = pass;
  
  Serial.begin(115200);
 
  pinMode(inPinCall, INPUT);
  pinMode(inPinEmer, INPUT);
  pinMode(inPinNorm, INPUT);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passw);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  
  Call = digitalRead(inPinCall);
  Norm = digitalRead(inPinNorm);
  Emer = digitalRead(inPinEmer);
  if(Norm)
  {
    statNorm = true;
  }
  if(Call)
  {
    statCall = true;
  }
  if(Emer)
  {
    statEmer = true;
  }
  
  if((!Norm) && statNorm)
  {
     
      SendMessage(url1);
      statNorm = false;
  }
  if((!Call)  && statCall)
  {    
      SendMessage(url2);
      statCall = false;      
  }
  if((!Emer) && statEmer)
  {
    
     SendMessage(url3);
     statEmer = false;
  }
  delay(10);
}

void SendMessage(String url)
  {
       WiFiClient client;  
       if (!client.connect(host, port)) {
          Serial.println("connection failed");
          return;
      }
      if (client.connected()) {
        client.println(url);
        client.println("Connection: close");
        client.println();
        Serial.println(url);
      }

    client.stop();
  }
