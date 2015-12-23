#OverDrive Motor Controller User Guide
Created and Maintained by Ward Prescott.  
Updated December 23, 2015 1:01 PM

##Introduction
The OverDrive board is a comprehensive motor control solution designed for use with 6 large DC motors.  It features encoder counting and analog inputs that can be used in your application.


##Quickstart

1. Follow the quickstart instructions [here.](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge).
2. Open the E01_... Example from the installed Arduino library.
3. Wire your DC motor to output A and wire the encoder into the Encoder A header.  See the connectivity section below for help with this.
4. Upload code and verify that the motor ramps up then down and that encoder count readings are displayed via the serial terminal.

##Caution
- Don't connect this board to an Arduino MEGA, it is 5V


##Connectivity
![TurboCharger System Diagram](https://raw.githubusercontent.com/kitekid250/Project-Overdrive-and-TurboCharge/master/Documentation/OverDrive%20Diagram.png)

#### Encoder Inputs
This is where quadrature encoders can be connected.  Note that many ecoders will not nescesarily have this pinout, potentially requiring the addition of a new connector or use of jumper wires.  The wiring scheme is as follows:
V: +5V
A: Quadrature A input
B: Quadrature B input
I: Encoder Index input (not used on all encoders)
G: Ground


#### Potentiometers In (Or GPIO)
To connect a servo, potentiometer, or digital input, use these ports.  They are compatiable with all servos as the pin order is Ground, +5VDC, Signal.  The corresponding pin numbers, A0, A1...., are labeled to the right of the headers.

####Motor Outputs
These three polarized connections are where the external motor drivers are connected.  The picture displays motors 5 and 6 connected, but up to 3 drivers can be installed at any time.


##Motor Control
Create a motor object with the following code:
```
OverDriveBasic myMotor(1);  //The argument is 1-6, corresponding to the motor number on the OverDrive board
```
Create an encoder object with the following code:
```
#define MDR0 X4_QUAD | FREE_RUN | CLK_DIV_1
#define MDR1 TWO_BYTE | FLAG_IDX | FLAG_BW | FLAG_CY
LS7366R myEncoder(ENC1, CNT_EN, MDR0, MDR1); //ENC[x] corresponds to the encoder number, the rest of the arguments are constants
```
Control a motor with the following functions:
```
myMotor.enable(1);  //Enables the motor
myMotor.enable(0);  //Disables the motor
myMotor.setDirection(1);  //Sets the motor to go in one direction
myMotor.setDirection(0);  //Sets the motor in the other direction
myMotor.setPWM(120);  //The sole argument is a value from 0-255 that scales from 0 to 100 of possible motor power
```
Read from an encoder with the following functions:
```
myEncoder.enableEncoder();  //Starts the encoder
unsigned long counter = myEncoder.readEncoder();  //Reads the count
myEncoder.clearEncoder();  //Zeros the encoder
byte status = myEncoder.readSTR();  //Reads and clears the status register
myEncoder.setCounter(x);  //Sets the counter to a particular value
```