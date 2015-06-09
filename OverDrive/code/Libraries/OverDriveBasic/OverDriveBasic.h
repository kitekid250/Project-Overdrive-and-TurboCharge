#ifndef _OVERDRIVE_BASIC
#define _OVERDRIVE_BASIC

// Encoder Interface Pins
#define ENC1 50
#define ENC2 49
#define ENC3 48
#define ENC4 27
#define ENC5 29
#define ENC6 28
#define CNT_EN 8
#define FLAG 51
// +SPI on ICSP header

// Driver Interface Pins
#define M1CS A6
#define M2CS A7
#define M3CS A8
#define M4CS A9
#define M5CS A10
#define M6CS A11

#define M1PWM 6
#define M2PWM 7
#define M3PWM 4
#define M4PWM 5
#define M5PWM 2
#define M6PWM 3

#define M1EN 47
#define M1INA 42
#define M1INB 46

#define M2EN 43
#define M2INA 45
#define M2INB 44

#define M3EN 33
#define M3INA 32
#define M3INB 30

#define M4EN 35
#define M4INA 31
#define M4INB 34

#define M5EN 39
#define M5INA 38
#define M5INB 36

#define M6EN 40
#define M6INA 37
#define M6INB 41

// Constants
#define ENABLED 1
#define DISABLED 0
#define FORWARD 0
#define BACKWARD 1

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3
#define MOTOR4 4
#define MOTOR5 5
#define MOTOR6 6

class OverDriveBasic
{
  public:
    OverDriveBasic(int motorNum); 
    void setDirection(int dir);
    void setEnable(int en);
    void setPWM(int spd);
    int getCurrent();
   private:
     int _MOTOR;
     int _EN;
     int _INA;
     int _INB;
     int _PWM;
     int _CS;
};

#endif /* _OVERDRIVE_BASIC */
