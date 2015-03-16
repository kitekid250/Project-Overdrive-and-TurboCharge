#include "basicUno.h"
#include "LS7366R.h"
#include <SPI.h>

#define RUN 0 // flag for run motor or just report encoder count
#define DELAY 10 

LS7366R enc(EN1_SS);

void setup() {
  Serial.begin(115200);
  Serial.println("Reset");

  pinMode(M1_INA, OUTPUT);
  pinMode(M1_INB, OUTPUT);
  pinMode(M1_EN, OUTPUT); // Solder jumper set for both M1 & M2
  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_CS, INPUT);

  digitalWrite(M1_EN, LOW);
  
  enc.clearEncoder();
}

void loop() {
#ifndef RUN
  Serial.println(enc.readEncoder());
  delay(100);
#endif
  
  
#ifdef RUN
  // forward
  digitalWrite(M1_INA, LOW);
  digitalWrite(M1_INB, HIGH);

  // ramp up
  digitalWrite(M1_EN, HIGH);
  for (int i = 0; i < 255; i++) {
    analogWrite(M1_PWM, i);
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // run
  for (int i = 0; i < 100; i++) {
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // ramp down
  for (int i = 255; i >= 0; i--) {
    analogWrite(M1_PWM, i);
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // halt
  digitalWrite(M1_EN, LOW);
  for (int i = 0; i < 100; i++) {
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // reverse
  digitalWrite(M1_INA, HIGH);
  digitalWrite(M1_INB, LOW);

  // ramp up
  digitalWrite(M1_EN, HIGH);
  for (int i = 0; i < 255; i++) {
    analogWrite(M1_PWM, i);
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // run
  for (int i = 0; i < 100; i++) {
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // ramp down
  for (int i = 255; i >= 0; i--) {
    analogWrite(M1_PWM, i);
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }

  // halt
  digitalWrite(M1_EN, LOW);
  for (int i = 0; i < 100; i++) {
    delay(DELAY);
  Serial.println(enc.readEncoder());
  }
#endif
}

