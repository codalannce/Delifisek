#include "LoRa_E22.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
LoRa_E22 E22(&mySerial);
 

struct gveriler {
  
  byte   glat[6];
  byte   glng[6];
  byte   galt[6];
  

} data;



void setup() {
 
  Serial.begin(9600);
  E22.begin();


}

void loop() {
  while (E22.available()  > 1) {
    // Gelen mesaj okunuyor
    ResponseStructContainer rsc = E22.receiveMessage(sizeof(gveriler));
    struct gveriler data = *(gveriler*) rsc.data;
    Serial.print(*(float*)(data.glat), 6); // Görev yükü enlem  değeri
    Serial.print(";");
    Serial.print(*(float*)(data.glng), 6); // Görev yükü boylam değeri
    Serial.print(";");
    Serial.println(*(float*)(data.galt)); // Görev yükü irtifa değeri
    rsc.close();
    delay(400);
  }
}
