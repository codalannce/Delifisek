#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Kalman.h>

Kalman kalmanX;
Kalman kalmanY;

#define RX_PIN 11
#define TX_PIN 10

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
Adafruit_BMP085 sensor;
TinyGPSPlus gps;


bool k1 = false;
bool k2 = false;
bool k3 = false;

#define buzzer 13

MPU6050 mpu(Wire);
void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();
  mpu.begin();
  sensor.begin();
  mpu.calcGyroOffsets(true);
  kalmanX.setAngle(0);
  kalmanY.setAngle(0);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  


  while (!(k1 && k2 && k3)) {
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

  float pa = sensor.readPressure();
  float a = sensor.readAltitude();
  float alt_status = a - sensor.readAltitude(101325);

   while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      Serial.println(p);
      Serial.println(";");
      Serial.println(r);          
      Serial.print(";");
      Serial.print(alt_status);
      Serial.print(";");
      Serial.print(gps.location.lat());
      Serial.print(";");
      Serial.print(gps.location.lng());
      Serial.print(";");
      Serial.print(gps.altitude.meters());
      Serial.println(";");
      
    }
  }
  if (r > 90){
    if (r > 100) {
      k1 = true;
      if(r > 120){
        k2 = true;
        if(r > 160){
          k3 = true;          
        }
      }
    }  
  }
 }tone(buzzer, 4000);

 while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      Serial.println(p);
      Serial.println(";");
      Serial.println(r);          
      Serial.print(";");
      Serial.print(alt_status);
      Serial.print(";");
      Serial.print(gps.location.lat());
      Serial.print(";");
      Serial.print(gps.location.lng());
      Serial.print(";");
      Serial.print(gps.altitude.meters());
      Serial.println(";");
       
    }
  }
  
}
