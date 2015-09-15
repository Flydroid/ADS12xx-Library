
#include "ads12xx.h"


volatile int DRDY_state = HIGH;


ads12xx::ads12xx(const int CS, const int START, const int DRDY){
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWrite(CS, HIGH);            // CS HIGH = no select
	pinMode(START, OUTPUT);		  // set START pin as Output
	digitalWrite(START, HIGH);        // HIGH = Start Convert Continuously
	pinMode(DRDY, INPUT);             // DRDY read 
	_CS = CS;
	_START = START;
	_DRDY = DRDY;
	delay(500);
	SPI.begin();
	ads12xx::Reset();
	delay(500);

	// interal VREF einschalten
}



void DRDY_Interuppt(){
	DRDY_state = LOW;
}





long ads12xx::GetConversion(){
	/*attachInterrupt(_DRDY,DRDY_Interuppt, FALLING);
	digitalWrite(_START, HIGH);
	delayMicroseconds(1);
	digitalWrite(_START, LOW);
	delayMicroseconds(10);
	*/
	SPI.transfer(SELFOCAL);
	delay(10);
	int32_t regData;
	//	while (!DRDY_state){
	
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1));
	digitalWrite(_CS, LOW); //Pull SS Low to Enable Communications with ADS1247
	delayMicroseconds(1); // RD: Wait 25ns for ADC12xx to get ready
	SPI.transfer(RDATA); //Issue RDATA
	regData |= SPI.transfer(NOP);
	regData <<= 8;
	regData |= SPI.transfer(NOP);
	regData <<= 8;
	regData |= SPI.transfer(NOP);
	delayMicroseconds(1);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
	//}
	//detachInterrupt(_DRDY);
	return regData;
}

void ads12xx::SetRegisterValue(uint8_t regAdress, uint8_t regValue){
#ifdef ADS1248
	if (regAdress == IDAC0){
		regValue = regValue | IDAC0_ID;	  // add non 0 non-write register value IDAC0_ID
	}
#endif
	
	if (regValue != ads12xx::GetRegisterValue(regAdress)){
		SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
		digitalWrite(_CS, LOW);
		delayMicroseconds(1);
		SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register 
		SPI.transfer(0x00);		// send 2nd command byte, write only one register 
		SPI.transfer(regValue);         // write data (1 Byte) for the register 
		delayMicroseconds(1);
		digitalWrite(_CS, HIGH);
		if (regValue != ads12xx::GetRegisterValue(regAdress)){
			Serial.print("Write to Register 0x");
			Serial.print(regAdress, HEX);
			Serial.println(" failed!");
		}
		SPI.endTransaction();
	}
	/*
	else{
		Serial.print("Register 0x");
		Serial.print(regAdress, HEX);
		Serial.println(" wurde nicht geaendert");
	}
	*/


	

}

unsigned long ads12xx::GetRegisterValue(uint8_t regAdress){
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	uint8_t bufr;
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
	SPI.transfer(0x00);			// send 2nd command byte, read only one register 
	bufr = SPI.transfer(NOP);	// read data of the register 
	delayMicroseconds(1);
	digitalWrite(_CS, HIGH);
	return bufr;
	SPI.endTransaction();
}

void ads12xx::Reset(){
	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(RESET); //Reset 
	delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(SDATAC); //Issue SDATAC
	delayMicroseconds(1); 

	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

void Error(char* msg){
	Serial.println(msg);
}

#ifdef ADS1248
void ads12xx::SetRegister(regValues_t regValues){
	ads12xx::SetRegisterValue(MUX0, regValues.MUX0_val);
	ads12xx::SetRegisterValue(IDAC1, regValues.IDAC1_val);
	ads12xx::SetRegisterValue(VBIAS, regValues.VBIAS_val);
	ads12xx::SetRegisterValue(MUX1, regValues.MUX1_val);
	ads12xx::SetRegisterValue(SYS0, regValues.SYS0_val);
	ads12xx::SetRegisterValue(IDAC0, regValues.IDAC0_val);
}
#endif



long ads12xx::readSingle(regValues_t regValues){
	
	
	long val = ads12xx::GetConversion();
	return val;

}

void ads12xx::calibration(int cal_cmd){
	int data[3];
	String cmd_name;
	if (cal_cmd == SYSGCAL){
		data[2] = FSC2;
		data[1] = FSC1;
		data[0] = FSC0;
		cmd_name = "FSC: ";
	}
	if (cal_cmd == SYSOCAL){
		data[2] = OFC2;
		data[1] = OFC1;
		data[0] = OFC0;
		cmd_name = "OFC: ";
	}
	if (cal_cmd == SELFOCAL){
		data[2] = OFC2;
		data[1] = OFC1;
		data[0] = OFC0;
		cmd_name = "self OFC: ";
	}

	SPI.beginTransaction(SPISettings(4096000, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(cal_cmd);
	delay(10);
	uint32_t val =0;
	val |= ads12xx::GetRegisterValue(data[2]);
	val << 8;
	val |= ads12xx::GetRegisterValue(data[1]);
	val << 8;
	val |= ads12xx::GetRegisterValue(data[0]);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();	
	Serial.print(cmd_name);
	Serial.println(val, DEC);
}

