#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Kalman.h>
#include <Adafruit_BMP085.h>

MPU6050 mpu6050(Wire);
Kalman kalmanX;
Kalman kalmanY;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets();

  kalmanX.setAngle(0);
  kalmanY.setAngle(0);

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

}

void loop() {
  static uint32_t timer = millis();

  float dt = (millis() - timer) / 1000.0;
  timer = millis();

  float a = bmp.readAltitude();
  float alt_status = a - bmp.readAltitude(101325);

  mpu6050.update();
  float gyroX = mpu6050.getGyroAngleX();
  float gyroY = mpu6050.getGyroAngleY();

  float accelX = mpu6050.getAccAngleX();
  float accelY = mpu6050.getAccAngleY();

  float pitch = kalmanX.getAngle(accelX, gyroX, dt);
  float roll = kalmanY.getAngle(accelY, gyroY, dt);

  delay(50);
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.println(alt_status);
  
  delay(100);
}
