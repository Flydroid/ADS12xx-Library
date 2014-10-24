#include <SPI.h>
#include "ads1248.h"

const int  START = 14;
const int  CS = 10;
const int  DRDY = 15;


ads1248 ADS;  //initialize ADS as object of the ads1248 class



void setup()
{
	

	Serial.begin(9600);
	Serial.println("Serial online");
	
	
	ADS.begin(CS,START,DRDY);

	ADS.reset();
	/*
	ADS.writeRegister(0x02, 0x38); //Set MUX1 Register (02h) Write 38h
	ADS.writeRegister(0x03, 0x52); //Set SYS0 Register (03h) Write 52h 
	ADS.writeRegister(0x0A, 0x07); //Set IDAC0 Register (0Ah) Write 07h - Select 1.5mA reference current for RTD
	ADS.writeRegister(0x0B, 0x01); //Set IDAC1 Register (0Bh) Write 01h - Output reference current on ANIN0,1
	ADS.selectChannel(0);
	
	Serial.println(ADS.readRegister(0x00),HEX);
	Serial.println(ADS.readRegister(0x02), HEX);
	Serial.println(ADS.readRegister(0x03), HEX);
	Serial.println(ADS.readRegister(0x0A), HEX);
	Serial.println(ADS.readRegister(0x0B), HEX);
	*/
}

void loop()
{






	delay(500);


}


void test(){
		
	SPI.beginTransaction(SPISettings());
	digitalWrite(CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x40); // send 1st command byte, address of the register
	SPI.transfer(0x00);		// send 2nd command byte, write only one register 
	SPI.transfer(0x07); // write data for the register
	digitalWrite(CS, HIGH);
	delay(10);
	digitalWrite(CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x40); // send 1st command byte, address of the register
	SPI.transfer(0x01);			// send 2nd command byte, read two registers 
	Serial.println(SPI.transfer(0xFF));	// read data of the register 
	Serial.println(SPI.transfer(0xFF));
	digitalWrite(CS, HIGH);
	SPI.endTransaction();

}








/*
		Channel Switcher mit case - switch
		oder mit array
*/
