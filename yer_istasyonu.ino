#include "LoRa_E22.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(16, 17); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
LoRa_E22 E22(&mySerial);
 


struct veriler {
 
  byte   paalt[4];

  byte   pitchdeger[4];
  byte   rolldeger[4];

  byte   lat[6];
  byte   lng[6];
  byte   alt[6];

} data;



void setup() {
  Serial.begin(9600);
  E22.begin();
  delay(500);

}

void loop() {
  while (E22.available()  > 1) {
    // Gelen mesaj okunuyor
    ResponseStructContainer rsc = E22.receiveMessage(sizeof(veriler));
    Serial.print(*(float*)(data.paalt));//Basınç irtifa değeri
    Serial.print(";"); 
    Serial.print(*(float*)(data.pitchdeger)); // Pitch değeri
    Serial.print(";");
    Serial.print(*(float*)(data.rolldeger)); // Roll değeri
    Serial.print(";");
    Serial.print(*(float*)(data.lat)); // GPS Enlem değeri
    Serial.print(";");
    Serial.print(*(float*)(data.lng)); // GPS Boylam Değeri
    Serial.print(";");
    Serial.println(*(float*)(data.alt)); // GPS irtifa değeri
    delay(300);
    rsc.close();

  }
}
