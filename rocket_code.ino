#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <MPU6050_tockn.h>
#include "LoRa_E22.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Kalman.h>

#define RX_PIN 3
#define TX_PIN 2

SoftwareSerial mySerial(11, 10);
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

LoRa_E22 E22(&mySerial);

Kalman kalmanX;
Kalman kalmanY;

#define M0 7
#define M1 6

struct veriler {

  byte   paalt[4];

  byte   pitchdeger[4];
  byte   rolldeger[4];

  byte   lat[6];
  byte   lng[6];
  byte   alt[6];

  byte   glat[6];
  byte   glng[6];
  byte   galt[6];
  

} data;


#define funye 4 

bool k1 = false;
bool k2 = false;

Adafruit_BMP085 sensor;

TinyGPSPlus gps;
MPU6050 mpu(Wire);




void setup() {
pinMode(funye, OUTPUT);
pinMode(M0, OUTPUT);
pinMode(M1, OUTPUT);
digitalWrite(M0, LOW);
digitalWrite(M1, LOW);

Serial.begin(9600);
gpsSerial.begin(9600);
Wire.begin();
E22.begin();
mpu.begin();
mpu.calcGyroOffsets();
sensor.begin();
kalmanX.setAngle(0);
kalmanY.setAngle(0);

}


void loop() {

digitalWrite(funye, LOW);

static uint32_t timer = millis();
float dt = (millis() - timer) / 1000.0;
timer = millis();

mpu.update();
float gyroX = mpu.getGyroAngleX();
float gyroY = mpu.getGyroAngleY();

float accelX = mpu.getAccAngleX();
float accelY = mpu.getAccAngleY();

float p = kalmanX.getAngle(accelX, gyroX, dt);
float r = kalmanY.getAngle(accelY, gyroY, dt);

float gla = *(float*)(data.glat);
float gln = *(float*)(data.glng);
float gA =  *(float*)(data.galt);

int a = sensor.readAltitude();
int alt_status = a - sensor.readAltitude(101325);
int min_apogee = 1400;

while (E22.available()  > 1) {
  ResponseStructContainer rsc = E22.receiveMessage(sizeof(veriler));
  struct veriler data = *(veriler*) rsc.data;
  float gla = *(float*)(data.glat);
  float gln = *(float*)(data.glng);
  float gA =  *(float*)(data.galt);
  rsc.close();

}
float la = gps.location.lat();
float ln = gps.location.lng();
float A =  gps.altitude.meters();

  while (!(k1 && k2)) {
    *(float*)(data.rolldeger) = r;
    *(float*)(data.pitchdeger) = p;
    *(int*)(data.paalt) = alt_status;
    *(float*)(data.glat) = gla;
    *(float*)(data.glng) = gln;
    *(float*)(data.galt) = gA; 
 

    while (gpsSerial.available() > 0) {
      if (gps.encode(gpsSerial.read())) {
        *(float*)(data.lat) = la;
        *(float*)(data.lng) = ln;
        *(float*)(data.alt) = A;
      }
    }
      
    if (alt_status > 500){
      if (a >= min_apogee) {
        k1 = true;
        if(a >= 1000 && a < 1300){
          if(p <= 1){
            digitalWrite(funye, HIGH);
            k2 = true;
          
          }
        }
      }  
    }  
  }
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      *(float*)(data.lat) = la;
      *(float*)(data.lng) = ln;
      *(float*)(data.alt) = A;
    }
  }

  *(float*)(data.rolldeger) = r;
  *(float*)(data.pitchdeger) = p;
  *(int*)(data.paalt) = alt_status;
  *(float*)(data.glat) = gla;
  *(float*)(data.glng) = gln;
  *(float*)(data.galt) = gA; 

  delay(500);
  ResponseStatus rs = E22.sendFixedMessage(0, 2, 19, &data, sizeof(veriler));
  //Serial.println(rs.getResponseDescription());
    
}
