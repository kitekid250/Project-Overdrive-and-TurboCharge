#include "OverDriveBasic.h"
#include "LS7366R.h"
#include <SPI.h>

OverDriveBasic motor1(MOTOR1);
LS7366R enc(ENC1, CNT_EN);

void setup() {
  Serial.begin(115200);
  Serial.println("Reset");
  enc.init();
  enc.enableEncoder(1);
  enc.clearEncoder();

  motor1.setDirection(FORWARD);
  motor1.setPWM(128);
  motor1.setEnable(ENABLED); // Uncomment to run motor
}

void loop() {
  Serial.print("Enc: ");
  Serial.println(enc.readEncoder());
  delay(100);  
}
