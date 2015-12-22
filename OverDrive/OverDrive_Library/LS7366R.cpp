// ALPHA RELEASE
// prd005

//6_25_2015: Added Set Counter - erp006 

#include <SPI.h>
#include "LS7366R.h"
#include "Arduino.h"

LS7366R::LS7366R(int SSP, int CNT_EN){
  _SS = SSP;
  _EN = CNT_EN;
  _bytes = FOUR_BYTE;
  
  //  Default Encoder Settings
  //  Clock division factor: 0
  //  Negative index input
  //  free-running count mode
  //  x4 quatrature count mode (four counts per quadrature cycle)
  //  2-byte counter
  _MDR0 = X4_QUAD;
  _MDR1 = TWO_BYTE;
  
  pinMode(_SS, OUTPUT);
  pinMode(CNT_EN, OUTPUT);
  digitalWrite(_SS, HIGH);
}


LS7366R::LS7366R(int SSP, int CNT_EN, byte MDR0, byte MDR1){
//	  pinMode(9, OUTPUT);
// digitalWrite(9, HIGH);
	_SS = SSP;
	_EN = CNT_EN;
	_bytes = MDR1 & 0x3;
	_MDR0 = MDR0;	  
	_MDR1 = MDR1;	  
	
	pinMode(_SS, OUTPUT);
	pinMode(CNT_EN, OUTPUT);
	digitalWrite(_SS, HIGH);
}


void LS7366R::init(){
    SPI.begin();  
	
	digitalWrite(_SS, LOW);
	SPI.transfer(MDR0_REG);      // Write to MDR0
	SPI.transfer(_MDR0);       
	digitalWrite(_SS, HIGH);
	
	delayMicroseconds(100);
	
	digitalWrite(_SS, LOW);
	SPI.transfer(MDR1_REG);      // Write to MDR1
	SPI.transfer(_MDR1);       
	digitalWrite(_SS, HIGH);
}

unsigned long LS7366R::readEncoder(){
  unsigned long count_value = 0;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x60);  // Request count
	
	// This is not an improper use of switch-case, just clever...
	switch(_bytes){
		case FOUR_BYTE:
			count_value = SPI.transfer(0x00);
			count_value = count_value << 8;
		case THREE_BYTE:
			count_value = count_value + SPI.transfer(0x00);
			count_value = count_value << 8;
		case TWO_BYTE:
			count_value = count_value + SPI.transfer(0x00);
			count_value = count_value << 8;
		default:
			count_value = count_value + SPI.transfer(0x00);		
		}
	digitalWrite(_SS, HIGH);
	
  return count_value;
}

// There's a better way to accomplish this...
void LS7366R::clearEncoder(){
  digitalWrite(_SS, LOW);

  SPI.transfer(0x98);

  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite(_SS, HIGH);

  delayMicroseconds(100);


  digitalWrite(_SS, LOW);     
  SPI.transfer(0xE0);      // Load DTR into CNTR
  digitalWrite(_SS, HIGH);
}

void LS7366R::enableEncoder(){
	digitalWrite(_EN, HIGH);
}

void LS7366R::disableEncoder(){
	digitalWrite(_EN, LOW);
}

// =================================================================================
// Wrong, DTR changes its width with the byte mode. This function needs to be fixed
// to reflect those changes.
void LS7366R::writeDTR(unsigned long set){
  byte b1, b2, b3, b4;
  b1 = set & 0xFF;
  b2 = (set >> 8) & 0xFF;
  b3 = (set >> 16) & 0xFF;
  b4 = (set >> 32) & 0xFF;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x98);  // Write DTR
  SPI.transfer(b1);
  SPI.transfer(b2);
  SPI.transfer(b3);
  SPI.transfer(b4);

  digitalWrite(_SS, HIGH); 
}
// =================================================================================


byte LS7366R::readSTR(){
  byte ret = 0;
  digitalWrite(_SS, LOW);
  SPI.transfer(0x70);  // Request STR
  ret = SPI.transfer(0x00);
  digitalWrite(_SS, HIGH);
  
  delayMicroseconds(100);
  
  digitalWrite(_SS, LOW);
  SPI.transfer(0x30); // Clear STR
  digitalWrite(_SS, HIGH); 
  return ret;
}


void LS7366R::setCounter(unsigned long set){
	//////////////////////////////////////////////////////
	//This should be tested for all bytes modes.  I only tried it with 2 byte - WP
	// Also, this would ideally have a means of ensuring that the input data matches up with the number of bytes that the counter register is using.
	//		Currently, I just truncate it bitwise, but this could lead to users thinking they pass one number when really it is another.
	//////////////////////////////////////////////////////

	digitalWrite(_SS, LOW);
	SPI.transfer(0x98); //Write DTR

	 unsigned char dataOut1 = (set & 0xff000000UL) >> 24;
	 unsigned char dataOut2 = (set & 0x00ff0000UL) >> 16;
	 unsigned char dataOut3 = (set & 0x0000ff00UL) >> 8;
	 unsigned char dataOut4 = (set & 0x000000ffUL);

switch(_bytes){
	case FOUR_BYTE:
		SPI.transfer(dataOut1);
		SPI.transfer(dataOut2);
		SPI.transfer(dataOut3);
		SPI.transfer(dataOut4);
		break;
	case THREE_BYTE:
		SPI.transfer(dataOut2);
		SPI.transfer(dataOut3);
		SPI.transfer(dataOut4);
		break;
	case TWO_BYTE:
		SPI.transfer(dataOut3);
		SPI.transfer(dataOut4);
		break;
		
	default: // ONE_BYTE
		SPI.transfer(dataOut4);
		break;
	}

	digitalWrite(_SS, HIGH);  //End command
	
	delayMicroseconds(100); //Give adequate time between commands

	digitalWrite(_SS, LOW);
	SPI.transfer(0xE0);  //Load Counter
	digitalWrite(_SS, HIGH);

}


/*
unsigned long LS7366R::readOTR(){
	// TODO implement this.
}
*/

// =================================================================================

// DEPRICATED
long LS7366R::s_readEncoder(){
  // TODO, decide on keeping or not.
  long count_value = 0;

  digitalWrite(_SS, LOW);
  
  SPI.transfer(0x60);  // Request count
	switch(_bytes){
	case FOUR_BYTE:
		// get the data it is kind of signed already...
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		break;
	case THREE_BYTE:
		// get the data
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		
		// sign extend the MSB
		if(count_value & 0x00800000){
			count_value |= 0xFF000000;
		}
		break;
		
	case TWO_BYTE:
		// get the data
		count_value = SPI.transfer(0x00);
		count_value = (count_value << 8) + SPI.transfer(0x00);
		
		// sign extend the MSB
		if(count_value & 0x00008000){
			count_value |= 0xFFFF0000;
		}
		
		break;
		
	default: // ONE_BYTE
		count_value = count_value + SPI.transfer(0x00);		
		// sign extend the MSB
		if(count_value & 0x00000080){
			count_value |= 0xFFFFFF00;
		}
		
		break;
	}
  digitalWrite(_SS, HIGH);

  

  return count_value;
}


