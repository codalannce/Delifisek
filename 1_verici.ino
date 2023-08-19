#include "LoRa_E22.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);
LoRa_E22

E22(&mySerial);

struct veriler {
  byte  test[4];
  byte  testid[4];
} data;


void setup() {
  Serial.begin(9600);
  E22.begin();
}

void loop() {
    *(float*)(data.test) = 15;
    *(float*)(data.testid) = 10;
    ResponseStatus rs = E22.sendFixedMessage(0, 101, 76, &data, sizeof(veriler));
    delay(900);
}
