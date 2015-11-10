#Project TurboCharge User Guide
Created and Maintained by Ward Prescott.  Updated Nov 10, 2015 1:13 AM.

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
- Rugged Circuits Ruggedduino
- Arduino UNO R????

More boards will be added as they are confirmed operational.

##Quickstart
1. Install the ????.zip Arduino library found at LINK.
2. Open the 01_TurboCharge_Quickstart Example from the installed Arduino library.
3. Wire the board as the picture shows.
4. Upload code and verify that the motor ramps up then down.

##Wiring Diagram

##Advanced Motor Control

##Extra Peripherals


TODO:
1. Testing sketch
2. Quickstart sketch
3. Annotated pics
