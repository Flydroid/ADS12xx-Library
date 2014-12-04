
#include "ads1248.h"

char msg;


ads1248::ads1248(const int CS, const int START, const int DRDY){
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWrite(CS, HIGH);            // CS HIGH = no select
	pinMode(START, OUTPUT);		  // set START pin as Output
	digitalWrite(START, HIGH);        // HIGH = Start Convert Continuously
	pinMode(DRDY, INPUT);             // DRDY read
	_CS = CS;
	delay(500);
	SPI.begin();
	ads1248::Reset();
	delay(500);
									  // interal VREF einschalten
}

ads1248::ads1248(const int CS, const int START){
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWrite(CS, HIGH);            // CS HIGH = no select
	
	_CS = CS;
	delay(500);
	SPI.begin();
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1));
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(0x06);               // 0x16 SDATAC - 0x14 RDATAC
	delay(210);
	SPI.transfer(0x16);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
	delay(500);
									  //Internal VRef einschalten
}



 




unsigned long ads1248::GetConversion(){
	uint32_t regData;
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1));
	digitalWrite(_CS, LOW); //Pull SS Low to Enable Communications with ADS1247
	delayMicroseconds(1); // RD: Wait 25ns for ADC1248 to get ready

	SPI.transfer(RDATA); //Issue RDATA
	regData |= SPI.transfer(NOP);
	regData <<= 8;
	regData |= SPI.transfer(NOP);
	regData <<= 8;
	regData |= SPI.transfer(NOP);


	digitalWrite(_CS, HIGH);
	SPI.endTransaction();

	return regData;
}								

void ads1248::SetRegisterValue(uint8_t regAdress, uint8_t regValue){
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register 
	SPI.transfer(0x00);		// send 2nd command byte, write only one register 
	SPI.transfer(regValue);         // write data (1 Byte) for the register 
	delayMicroseconds(1);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

unsigned long ads1248::GetRegisterValue(uint8_t regAdress){
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	uint8_t bufr;
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
	SPI.transfer(0x00);			// send 2nd command byte, read only one register 
	bufr = SPI.transfer(NOP);	// read data of the register 
	digitalWrite(_CS, HIGH);
	return bufr;
	delayMicroseconds(1);
	SPI.endTransaction();
}

void ads1248::Reset(){
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(RESET); //Reset the ADS1247
	delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(SDATAC); //Issue SDATAC
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

void Error(char* msg){
	Serial.println(msg);
}



unsigned long ads1248::readPT100(regValues_t regValues){
	ads1248::SetRegisterValue(MUX0, regValues.MUX0_val);
	ads1248::SetRegisterValue(IDAC1,regValues.IDAC1_val);
	ads1248::SetRegisterValue(VBIAS,regValues.VBIAS_val);
	ads1248::SetRegisterValue(MUX1, regValues.MUX1_val);
	ads1248::SetRegisterValue(SYS0, regValues.SYS0_val);
	ads1248::SetRegisterValue(IDAC0,regValues.IDAC0_val);
	unsigned long PT100_val = ads1248::GetConversion();
	return PT100_val;
}

unsigned long* ads1248::readTE(regValues_t TE_regValues,regValues_t PT100_regValues){
	unsigned long TE_data[2];
	TE_data[0] = ads1248::readPT100(PT100_regValues);
	ads1248::SetRegisterValue(MUX0, TE_regValues.MUX0_val);
	ads1248::SetRegisterValue(IDAC1, TE_regValues.IDAC1_val);
	ads1248::SetRegisterValue(VBIAS, TE_regValues.VBIAS_val);
	ads1248::SetRegisterValue(MUX1, TE_regValues.MUX1_val);
	ads1248::SetRegisterValue(SYS0, TE_regValues.SYS0_val);
	ads1248::SetRegisterValue(IDAC0, TE_regValues.IDAC0_val);
	TE_data[1] = ads1248::GetConversion();
	return TE_data;
}

unsigned long ads1248::readVoltage(regValues_t Voltage_regValues){
	ads1248::SetRegisterValue(MUX0, Voltage_regValues.MUX0_val);
	ads1248::SetRegisterValue(IDAC1, Voltage_regValues.IDAC1_val);
	ads1248::SetRegisterValue(VBIAS, Voltage_regValues.VBIAS_val);
	ads1248::SetRegisterValue(MUX1, Voltage_regValues.MUX1_val);
	ads1248::SetRegisterValue(SYS0, Voltage_regValues.SYS0_val);
	ads1248::SetRegisterValue(IDAC0, Voltage_regValues.IDAC0_val);
	unsigned long Voltage_val = ads1248::GetConversion();
	return Voltage_val;
}