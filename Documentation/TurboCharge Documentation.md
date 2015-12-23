#Project TurboCharge User Guide
Created and Maintained by Ward Prescott.  
Updated Nov 10, 2015 1:13 AM.
Nov 18, 2015 12:02 AM

##Introduction
The TurboCharge board is a comprehensive motor control solution designed for use with 2 small to medium sized DC motors.  It features encoder counting and analog inputs that can be used in your application.

##Maxiumum Operating Parameters
This table should be used as an initial guide to see if TurboCharge is right for your project.

|  Parameter | Max Rating | Notes|
|--------|--------|------| 
|DC Motor Current        |    2 Amps    |Small transient spikes of up to 3A are allowed.|
|DC Motor Voltage| 50V | The Arduino must be powered seperately for voltages in excess of 20V|
|Free Digital Pins| 8 | However, these represent the pins for I2C (2 pins), SPI (4 pins), and UART (2 pins) connectivity|
|Free Analog Pins| 2 | These are configured for potentiometers|

The TurboCharge has been tested on:
- Rugged Circuits Ruggeduino

More boards will be added as they are confirmed operational.

##Quickstart
1. Install the TurboChargerOverdrive.zip Arduino library found [here.](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge).
2. Open the E01_TurboCharge_Quickstart Example from the installed Arduino library.
3. Wire your DC motor to output A and wire the encoder into the Encoder A header.  See the connectivity section below for help with this.
4. Upload code and verify that the motor ramps up then down and that encoder count readings are displayed via the serial terminal.

##Caution



##Connectivity
![TurboCharger System Diagram](https://raw.githubusercontent.com/kitekid250/Project-Overdrive-and-TurboCharge/master/Documentation/TurboCharge%20Diagram.png)
#### Power in and Motors Out
Motors and the motor's power supply are connected here.  Your motors may require a specific voltage for best operation, but never exceed 24VDC.  Wiring is:
Vin: Voltage Input
Gnd: Ground/negative input
B2 & B1: Hookups for the B motor
A2 and A1: Hookups for the A motor

#### Analog and PWM
To connect a servo, potentiometer, or digital input, use these ports.  They are compatiable with all servos as the pin order is Ground, +5VDC, Signal.  The corresponding pin numbers, A0 and A1, are labeled underneath the headers.

#### I2C
PLEASE NOTE: SDA and SCL on the TWI header are labelled incorrectly and should be swapped.
The TWI has been made available for users who need the I2C communication protocol. On R3 Arduino Unos these pins are available on the digital side, but on R2 Arduino Unos these pins are only available with the analog input pins. In the interests of keeping this board inclusive, the TWI header is connected to Analog Pins A4 and A5. With this, if additional analog pins past A0 and A1 are needed and no I2C is needed, these pins can be directly used as analog inputs. Also, since all analog input pins are digital enabled as well, these pins can serve digital functions too.


This is a standard two wire interface, also known as I2C, that can be used to connect to various sensors and interfaces.  Take a look at [the Arduino Organizations reference page](https://www.arduino.cc/en/Reference/Wire) for more info on using it.
#### Reset Button
Since the Arduino's reset button may be covered by the shield, there is an identical button mounted on the shield.  Either button can be used to reset the Arduino mid sketch.
#### Vin Enable
For the Arduino and the motor shield to be powered from the same power supply, solder this jumper closed.  Be careful to note that this voltage must be between 5.5 and 16V to power the Arduino from it 

#### Encoder 1 and 2
This is where quadrature encoders can be connected.  Note that many ecoders will not nescesarily have this pinout, potentially requiring the addition of a new connector or use of jumper wires.  The wiring scheme is as follows:
V: +5V
A: Quadrature A input
B: Quadrature B input
I: Encoder Index input (not used on all encoders)
G: Ground

#### UART
Standard serial port for the Arduino.  See [the Arduino reference page for hardware serial](https://www.arduino.cc/en/Reference/Serial).

#### SPI
A standard serial peripheral interface with a chip select pin (Pin 3).  See [the Arduino reference page for SPI](https://www.arduino.cc/en/Reference/SPI)

#### Current Sense (Not Pictured)
Current sense is useful for detecting if the motor is stalled, disconnected, but practically can be used for anything that require the current measurement of the motor.  The Motor A current sense is on pin A3 and the Motor B current sense is on pin A2.  There are jumpers that need to be soldered together on the bottom of the board to use this functionality, but they may be presoldered so be sure to check if you're gettting a reading of 0.  To calculate the current use the conversion 2.25V/1A or 460.35/1A for 10-bit ADC with 5V Aref.

##Motor Control
Include the following at the top of your Arduino sketch:
```
#define DIRA 7
#define DIRB 8
#define PWMA 9
#define PWMB 10
#define CSA A3
#define CSB A2
```
From there Motor A can be made to go from fully on to fully off with:
```
analogWrite(PWMA, 0) //Change to PWMB for Motor B.  0 is full stop, 255 is full on, anything in between is a fraction.
```
From there Motor A can be made to go in a different direction with:
```
digitalWrite(DIRA, 0) //Change to DIRB for Motor B.  0 is one direction, 1 is the other.
```

TODO:
1. Testing sketch
2. Quickstart sketch