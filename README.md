# Project Overdrive and TurboCharge

OverDrive and TurboCharge are simple motor control modules designed for use by the Bucknell Department of Mechanical Engineering.  The boards are designed to be a simple interface with motors and encoders.  ProjectOverdrive is the big brother of Turbocharger and was develped to handle 6 high power motors (24V @ 20A ea) and have 6 encoder counters designed for an Arduino Due. Project TurboCharge is the little brother, powered from a L298P dual H-bridge, and fits nicely on top of an Arduino Uno.  It has two channels of encoder counting using the same chip as its big brother.  Its signficantly cheaper than the TurboCharge, making it the ideal candidate for projects requiring smaller motors.

###Documentation
You should read the docs corresponding to your board then follow the Installation/QuickStart instructions below

The documentation for the OverDrive Board is [here](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge/blob/master/Documentation/OverDrive%20Documentation.md).

The documentation for the TurboCharge Board is [here](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge/blob/master/Documentation/TurboCharge%20Documentation.md).

The (advanced) documentation for the encoder counter IC's is [here](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge/blob/master/Documentation/LS7366R%20Encoder%20Counter%20Documentation.md).

###Installation/QuickStart
Here are the steps to get going, starting from square one.

1. Install Arduino 1.6.6 from [here](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous).  There is a good chance that other Arduino versions will work, but this library is confirmed to work on 1.6.6.
2. Download the [OverDrive library](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge/raw/master/OverDrive/OverDrive_Library.zip) if you have an OverDrive board or the [TurboCharge library](https://github.com/kitekid250/Project-Overdrive-and-TurboCharge/raw/master/TurboCharge/TurboCharge_Library.zip) if you have a TurboCharge board.  Keep them as .ZIP files so Arduino can easily import them.
3. Install the library you just downloaded by navigating to Sketch->Include Library-> Add .ZIP Library
4. In the Arduino IDE navigate to Tools->Board->Boards Manager
5. Search for "due" and install version 1.6.6 of the one board that comes up.
6. Navigate to File->Examples->[OverDrive/TurboCharge]_Library->Example->E01...
7. Connect the Arduino Due or Arduino Uno that is connected to your OverDrive or TurboCharge board respectively.  If it is a Due, make sure that you plug into the Due's Programming Port (it says it on the bottom of the board).
8. Select the correct COM port from Tools->Port->[Whichever indicates "(Arduino Due/Uno)"]
9. Select Tools->Board->[Arduino Due (Programming Port) / Arduino Uno]
10. Press the Upload (right arrow) button in the toolbar to run the example on the board.

###About
This project is currently maintained by Ward Prescott (erp006@bucknell.edu), however Phil Diefenderfer is responsible for the current hardware design and the first draft of the documentation and libraries.
