#include "LoRa_E22.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
LoRa_E22 E22(&mySerial);
 
#define M0 7
#define M1 6

struct veriler {
  byte  testid[4];
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



void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.begin(9600);
  E22.begin();
  delay(500);

}

void loop() {
  while (E22.available()  > 1) {
    // Gelen mesaj okunuyor
    ResponseStructContainer rsc = E22.receiveMessage(sizeof(veriler));
    struct veriler data = *(veriler*) rsc.data;
    float testidvalue = *(float*)(data.testid);

    if(testidvalue == 10) {
      Serial.print(*(int*)(data.paalt));//Basınç irtifa değeri
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
    }
    
    
    if (testidvalue == 5) {
      Serial.print(*(float*)(data.glat)); // Görev yükü enlem değeri
      Serial.print(";");
      Serial.print(*(float*)(data.glng)); // Görev yükü boylam değeri
      Serial.print(";");
      Serial.println(*(float*)(data.galt)); // Görev yükü irtifa değeri

    }
    delay(300);
    rsc.close();

  }
}
