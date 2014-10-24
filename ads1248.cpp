
#include "ads1248.h"

char msg;


ads1248::ads1248(){
	
}


void ads1248::begin(const int CS, const int START, const int DRDY){
	pinMode(CS, OUTPUT);  // set the slaveSelectPin as an output:
	pinMode(START, OUTPUT);		  //set START pin as Output
	//digitalWrite(START, LOW);		// Start Convert Continuously
	pinMode(DRDY, INPUT);
	SPI.begin();

	_CS = CS;

	}

   
/*
unsigned long ads1248::readADC(){

}

 */
void ads1248::selectChannel(int CH_NUM){
	writeRegister(MUX0, CH_BUF[CH_NUM]);
}





unsigned long ads1248::readADConce(){
	SPI.beginTransaction(SPISettings()); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	uint32_t val = 0x00;
	digitalWrite(_CS, LOW); //Pull SS Low to Enable Communications with ADS1247
	delayMicroseconds(1); // RD: Wait 25ns for ADC1248 to get ready
	SPI.transfer(0x13); //Issue RDATA 
	val |= SPI.transfer(0xFF); // 0xFF do nothing, just recive byte
	val <<= 8;
	val |= SPI.transfer(0xFF); // 0xFF do nothing, just recive byte
	val <<= 8;
	val |= SPI.transfer(0xFF); // 0xFF do nothing, just recive byte
	digitalWrite(_CS, HIGH);
	return val;
	SPI.endTransaction();
}

void ads1248::writeRegister(uint8_t regadd, uint8_t regbyte){
	SPI.beginTransaction(SPISettings()); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x40 | regadd); // send 1st command byte, address of the register
	SPI.transfer(0x00);		// send 2nd command byte, write only one register 
	SPI.transfer(regbyte); // write data for the register
	digitalWrite(_CS, HIGH);
}

unsigned long ads1248::readRegister(uint8_t regadd){
	SPI.beginTransaction(SPISettings()); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	uint8_t bufr;
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x20 | regadd); // send 1st command byte, address of the register
	SPI.transfer(0x00);			// send 2nd command byte, read only one register 
	bufr = SPI.transfer(0xFF);	// read data of the register 
	digitalWrite(_CS, HIGH);
	return bufr;
	SPI.endTransaction();
}


void ads1248::reset(){
	SPI.beginTransaction(SPISettings()); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, HIGH);
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x06); //Reset the ADS1247
	delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(0x16); //Issue SDATAC
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

void Error(char* msg){
	Serial.println(msg);
}
