#include "LS7366R.h"

LS7366R::LS7366R(int SS_pin, int EN_pin){
  _SS = SS_pin;
  _EN = EN_pin;
  
  pinMode(_SS, OUTPUT);
  pinMode(_EN, OUTPUT);

  digitalWrite(_SS, HIGH);
  digitalWrite(_EN, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);

  // Init MDR0
  digitalWrite(_SS, LOW);
  byte cmd = WR | MDR0;
  SPI.transfer(cmd);
  SPI.transfer(MDR0_INIT);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);

  // Init MDR1, default 0
  digitalWrite(_SS, LOW);
  cmd = WR | MDR1;
  SPI.transfer(cmd);
  SPI.transfer(MDR1_INIT);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);

  // Clear (Interrupt) Status Register
  digitalWrite(_SS, LOW);
  cmd = CLR | STR;
  SPI.transfer(cmd);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);
}

LS7366R::LS7366R(int SS_pin){
  _SS = SS_pin;
  
  pinMode(_SS, OUTPUT);

  digitalWrite(_SS, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);

  // Init MDR0
  digitalWrite(_SS, LOW);
  byte cmd = WR | MDR0;
  SPI.transfer(cmd);
  SPI.transfer(MDR0_INIT);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);

  // Init MDR1, default 0
  digitalWrite(_SS, LOW);
  cmd = WR | MDR1;
  SPI.transfer(cmd);
  SPI.transfer(MDR1_INIT);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);

  // Clear (Interrupt) Status Register
  digitalWrite(_SS, LOW);
  cmd = CLR | STR;
  SPI.transfer(cmd);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);
}

void LS7366R::clearEncoder(){
  digitalWrite(_SS, LOW);
  byte cmd = WR | DTR;
  SPI.transfer(cmd);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite(_SS, HIGH);
  delayMicroseconds(100);

  digitalWrite(_SS, LOW);
  cmd = LOAD | CNTR;
  SPI.transfer(cmd);
  digitalWrite(_SS, HIGH);
}

long LS7366R::readEncoder(){
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;
  
  digitalWrite(_SS, LOW);
  byte cmd = RD | CNTR;
  SPI.transfer(cmd);
  delay(1);
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);
  count_3 = SPI.transfer(0x00);
  count_4 = SPI.transfer(0x00);           // Read lowest order byte
  digitalWrite(_SS, HIGH);
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;

  return count_value;
}

/*
void initEncCounter() {
  pinMode(EN1_SS, OUTPUT);
  pinMode(EN2_SS, OUTPUT);
  pinMode(CNT_EN, OUTPUT);

  digitalWrite(EN1_SS, HIGH);
  digitalWrite(EN2_SS, HIGH);
  digitalWrite(CNT_EN, HIGH);

  SPI.begin();
  // SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);

  // Init MDR0
  digitalWrite(EN1_SS, LOW);
  byte cmd = WR | MDR0;
  // Serial.print("MDR0: ");
  // Serial.println(cmd);
  SPI.transfer(cmd);
  SPI.transfer(MDR0_INIT);
  digitalWrite(EN1_SS, HIGH);
  delay(1);

  // Init MDR1, default 0
  digitalWrite(EN1_SS, LOW);
  cmd = WR | MDR1;
  // Serial.print("MDR1: ");
  // Serial.println(cmd);
  SPI.transfer(cmd);
  SPI.transfer(MDR1_INIT);
  digitalWrite(EN1_SS, HIGH);
  delay(1);

  // Clear (Interrupt) Status Register
  digitalWrite(EN1_SS, LOW);
  cmd = CLR | STR;
  // Serial.print("STR: ");
  // Serial.println(cmd);
  SPI.transfer(cmd);
  digitalWrite(EN1_SS, HIGH);
  delay(1);
}

long readEncoder() {
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;
  
  digitalWrite(EN1_SS, LOW);
  byte cmd = RD | CNTR;
  SPI.transfer(cmd);
  delay(1);
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);
  count_3 = SPI.transfer(0x00);
  count_4 = SPI.transfer(0x00);           // Read lowest order byte
  digitalWrite(EN1_SS, HIGH);
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;

  return count_value;
}

void clearEncoder() {
  digitalWrite(EN1_SS, LOW);
  byte cmd = WR | DTR;
  SPI.transfer(cmd);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite(EN1_SS, HIGH);
  delay(1);

  digitalWrite(EN1_SS, LOW);
  cmd = LOAD | CNTR;
  SPI.transfer(cmd);
  digitalWrite(EN1_SS, HIGH);
}
*/
