#ifndef _LS7366R
#define _LS7366R

#include <SPI.h>

// LS7366R commands
// Selection
#define NONE 0x00 // 0xXX00 0XXX 
#define MDR0 0x08 // 0xXX00 1XXX 
#define MDR1 0x10 // 0xXX01 0XXX 
#define DTR  0x18 // 0xXX01 1XXX 
#define CNTR 0x20 // 0xXX10 0XXX 
#define OTR  0x28 // 0xXX10 1XXX 
#define STR  0x30 // 0xXX11 0XXX 

// Command
#define CLR  0x00 // 0x00XX XXXX
#define RD   0x40 // 0x01XX XXXX
#define WR   0x80 // 0x10XX XXXX
#define LOAD 0xC0 // 0x11XX XXXX

#define MDR0_INIT 0x03 // 0x1000 0011
#define MDR1_INIT 0x00 // 0x0000 0000

/* This class allows for easy interface between the LS7366R encoder counter
from US Digital (LSCSI) and an Arduino */
class LS7366R
{
  public:
    /* constructor if the count enable pin is left floating or if the user
    wants to manually control the counter. */
    LS7366R(int SS_pin); 
    
    /* Constructor if the user wants to hand control of the enable pin over
    to the library */
    LS7366R(int SS_pin, int EN_pin);
    
    /* Reset the count on the encoder to zero */
    void clearEncoder();
    
    /* Read the current value of the encoder (32-bit) */
    long readEncoder();
   private:
     /* The slave select pin that corresponds to this encoder */
     int _SS;
     
     /* The enable pin that corresponds to this encoder */
     int _EN;
};

#endif /* _LS7366R */
