#include "OverDrive_Library.h"
#include <Arduino.h>
#include "LS7366R.h"

OverDriveBasic::OverDriveBasic(int motorNum) {
  _MOTOR = motorNum;
  switch (_MOTOR) {
    case 1:
      _EN = M1EN;
      _INA = M1INA;
      _INB = M1INB;
      _PWM = M1PWM;
      _CS = M1CS;
      break;
    case 2:
      _EN = M2EN;
      _INA = M2INA;
      _INB = M2INB;
      _PWM = M2PWM;
      _CS = M2CS;
      break;
    case 3:
      _EN = M3EN;
      _INA = M3INA;
      _INB = M3INB;
      _PWM = M3PWM;
      _CS = M3CS;
      break;
    case 4:
      _EN = M4EN;
      _INA = M4INA;
      _INB = M4INB;
      _PWM = M4PWM;
      _CS = M4CS;
      break;
    case 5:
      _EN = M5EN;
      _INA = M5INA;
      _INB = M5INB;
      _PWM = M5PWM;
      _CS = M5CS;
      break;
    default:
      _EN = M6EN;
      _INA = M6INA;
      _INB = M6INB;
      _PWM = M6PWM;
      _CS = M6CS;
      break;
  }
  pinMode(_INA, OUTPUT);
  pinMode(_INB, OUTPUT);
  pinMode(_EN, OUTPUT);
  pinMode(_PWM, OUTPUT);
  pinMode(_CS, INPUT);

  digitalWrite(_INA, LOW);
  digitalWrite(_INB, LOW);
  digitalWrite(_EN, LOW);
  digitalWrite(_PWM, LOW);

}

void OverDriveBasic::setDirection(int dir) {
  if (dir) {
    digitalWrite(_INA, HIGH);
    digitalWrite(_INB, LOW);
  }
  else {
    digitalWrite(_INB, HIGH);
    digitalWrite(_INA, LOW);
  }
}

void OverDriveBasic::setEnable(int en) {
  if (en) {
    digitalWrite(_EN, HIGH);
  }
  else {
    digitalWrite(_EN, LOW);
  }
}

void OverDriveBasic::setPWM(int spd) {
  analogWrite(_PWM, spd);
}

int OverDriveBasic::getCurrent() {
  return analogRead(_CS);
}


