/*This example shows how to control a motor with a potentiometer,
  in addition to the motor in example 1.  The methodology can be 
  applied to any other control input.
Example 3 discusses each of these many settings in detail.  
Overflow is indeed considered in this example.   */

//Connect a potentiometer into A0 and motors into 1 and 2 for this to work

#include <SPI.h>
#include <OverDrive_Library.h>
#include <LS7366R.h>


#define MDR0 X4_QUAD | FREE_RUN | CLK_DIV_1
#define MDR1 TWO_BYTE | FLAG_IDX | FLAG_BW | FLAG_CY

LS7366R enc1(ENC1, CNT_EN, MDR0, MDR1);
LS7366R enc2(ENC2, CNT_EN, MDR0, MDR1);
OverDriveBasic motor1(1);
OverDriveBasic motor2(2);

//Global variables we need for this example code
volatile byte flag = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("Begin");

  enc1.init();
  enc1.readSTR();    //Clear the interrupt register after init
  enc1.enableEncoder();    // Enable the encoder counter  
  enc1.clearEncoder();  // so reset to 0 when we're ready to start
  
  enc2.init();  //Do it all again for the second encoder
  enc2.readSTR();
  enc2.enableEncoder();
  enc2.clearEncoder();

  motor1.setDirection(0);
  motor1.setEnable(1);
  motor1.setPWM(15);
  
  motor2.setDirection(0);
  motor2.setEnable(1);
  motor2.setPWM(15);

  // Finally attach an interrupt to the LFLAG pin
  attachInterrupt(FLAG, ISR_ENC, FALLING);
  Serial.println("Setup Finished");
}

void loop(){
  if(flag){  // If the ISR detected an interrupt, handle it

    byte stat = enc1.readSTR();  // read (then clear) the status register    
    Serial.print("INT EVENT: ");  // Some interrupt event happened
    
    // Compare the status register to the Carryover bit-mask to
    if(stat & CY_MASK){  // determine if it was a carryover event
      Serial.println("Carryover Event");  //This means it overflowed
    }
    
    // Compare the status register to the Borrow bit-mask to
    if(stat & BW_MASK){  // determine if it was a borrow event
      Serial.println("Borrow Event");  //This means we subtracted from zero
    }
    
    // Compare the status register to the Index bit-mask to
    if(stat & IDX_MASK){  // determine if it was an index pulse
      Serial.println("Index Pulse");  //Most encoders don't have this, but it can be useful
    }

    flag = 0;  // reset the ISR's flag noting that we handled it
  }
  
  // read the encoder's count as an unsigned long
  unsigned long count = enc1.readEncoder();
  unsigned long count2 = enc2.readEncoder();
  
  //Print the counts
  Serial.print("Enc 1 Count: ");
  Serial.print(count);
  Serial.print("   Enc 2 Count: ");
  Serial.print(count2);
  Serial.println();

  //This is where the control majic happens
  int potentiometerReading = analogRead(A0);  //Get this reading once instead of 3+ times
  if (potentiometerReading > 510){
    //This accounts for one half of the potentiometer going in one direction
    motor1.setDirection(1);
    motor1.setPWM(map(potentiometerReading, 510, 1023, 0 , 255));
  }
  else if (potentiometerReading > 490){
    //This is the middle, dead spot of the potetiometer where the motor is stopped
    motor1.setPWM(0);
  }
  else {
    //This is for the other half of the potentiometers range that goes in the opposite direction
    motor1.setDirection(0);
    motor1.setPWM(map(potentiometerReading, 0, 490, 0 , 255));
  }


  // delay so we don't flood the serial monitor
  delay(500);
}


/* Interrupt Service Routine for LFLAG. */
void ISR_ENC(){
  //See example 3 for a better explanation
  flag = 1;
}
