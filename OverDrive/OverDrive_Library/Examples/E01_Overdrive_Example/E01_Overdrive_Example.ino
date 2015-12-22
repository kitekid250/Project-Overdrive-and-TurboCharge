//This is the barebones example, designed to show the basics.  Example 2 talks more about how to control motors to you bidding and Example 3 discusses each of these many settings in detail.  Please note that this example doesn't handle encoder overflow, though Examples 2 and 3 do.

#include <SPI.h>
#include <OverDrive_Library.h>
#include <LS7366R.h>

#define MDR0 X4_QUAD | FREE_RUN | CLK_DIV_1
#define MDR1 TWO_BYTE | FLAG_IDX | FLAG_BW | FLAG_CY

LS7366R enc1(ENC1, CNT_EN, MDR0, MDR1);
OverDriveBasic motor1(1);

void setup(){
  Serial.begin(115200);
  Serial.println("Begin");
  enc1.init();
  enc1.readSTR(); //Clear interrupt register
  enc1.enableEncoder();
  enc1.clearEncoder();
    
  motor1.setDirection(0);
  motor1.setEnable(1);
  motor1.setPWM(200);
}

void loop(){
  unsigned long count = enc1.readEncoder();  //Read the encoder
  
  Serial.print(" Current Encoder Count: ");
  Serial.println(count);  //Print it!

  delay(100);   // delay so we don't flood the serial monitor
}