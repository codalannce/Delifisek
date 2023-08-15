#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define RX_PIN 11
#define TX_PIN 10

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
TinyGPSPlus gps;

void setup() {

  Serial.begin(9600);
  gpsSerial.begin(9600);

  delay(2000);
  Serial.println("testing");
}

void loop() {
  if(gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(",");
        Serial.print(gps.location.lng(), 6);
        Serial.print(",");
        Serial.println(gps.altitude.meters());
    }
  }
}
