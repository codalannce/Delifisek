#include "LoRa_E22.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
LoRa_E22 E22(&mySerial);


struct veriler {
  byte  test[4];
  byte  testid[4];
} data;

void setup() {

   Serial.begin(9600);
   E22.begin();

}

void loop() {
  while (E22.available()  > 1) {
 
    ResponseStructContainer rsc = E22.receiveMessage(sizeof(veriler));
    struct veriler data = *(veriler*) rsc.data;
    float testidvalue = *(float*)(data.testid);
    Serial.println(testidvalue);
    if (testidvalue == 5.00)
    {
      Serial.println("1.lora: ");
      Serial.print(*(float*)(data.test));
      Serial.print(",");
      
    }
   
    
    if (testidvalue == 10.00)
    {
      Serial.println("2.lora: ");
      Serial.print(*(float*)(data.test));
      Serial.print(",");
      
    }

    rsc.close();
  }
  delay(400);
}
