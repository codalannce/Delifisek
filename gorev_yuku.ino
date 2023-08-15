#include <Wire.h>
#include "LoRa_E22.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define RX_PIN 3
#define TX_PIN 2

SoftwareSerial mySerial(11, 10);
SoftwareSerial gpsSerial(RX_PIN, TX_PIN); //rx, tx

LoRa_E22 E22(&mySerial);

#define M0 7
#define M1 6

struct veriler {
  byte   glat[6];
  byte   glng[6];
  byte   galt[6];
  
} data;

TinyGPSPlus gps;


void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();
  E22.begin();

}

void loop() {
  
  while(E22.available()> 1){
    if(gps.encode(gpsSerial.read())){

      *(float*)(data.glat) = gps.location.lat();
      *(float*)(data.glng) = gps.location.lng();
      *(float*)(data.galt) = gps.altitude.meters();
      ResponseStatus rs = E22.sendFixedMessage(0, 2, 19, &data, sizeof(veriler));
      Serial.println(rs.getResponseDescription());

      delay(100);
    }    
  }
  

  

}
