#include "ads12xx.h"

volatile int DRDY_state = HIGH;

void waitforDRDY() {
	while (DRDY_state) continue;
}

void DRDY_Interuppt() {
	DRDY_state = LOW;
}

ads12xx::ads12xx(const int CS, const int START, const int DRDY) {
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWrite(CS, HIGH);            // CS HIGH = no select
#ifdef ADS1248
	pinMode(START, OUTPUT);		  // set START pin as Output
	digitalWrite(START, HIGH);        // HIGH = Start Convert Continuously
	_START = START;
#endif
	pinMode(DRDY, INPUT);             // DRDY read
	_CS = CS;
	_DRDY = DRDY;
	delay(500);
	SPI.begin();

	attachInterrupt(_DRDY, DRDY_Interuppt, FALLING);

	delay(500);

	// interal VREF einschalten
}



long ads12xx::GetConversion() {
	/*attachInterrupt(_DRDY,DRDY_Interuppt, FALLING);
	digitalWrite(_START, HIGH);
	delayMicroseconds(1);
	digitalWrite(_START, LOW);
	delayMicroseconds(10);
	*/
	//SPI.transfer(SELFOCAL);
	delay(10);
	int32_t regData;
	waitforDRDY();
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	digitalWrite(_CS, LOW); //Pull SS Low to Enable Communications with ADS1247
	delayMicroseconds(10); // RD: Wait 25ns for ADC12xx to get ready
	SPI.transfer(RDATA); //Issue RDATA
	delayMicroseconds(10);
	regData |= SPI.transfer(NOP);
	//delayMicroseconds(10);
	regData <<= 8;
	regData |= SPI.transfer(NOP);
	//delayMicroseconds(10);
	regData <<= 8;
	regData |= SPI.transfer(NOP);
	delayMicroseconds(10);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
	noInterrupts();
	DRDY_state = HIGH;
	interrupts();

	return regData;
}

void ads12xx::SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
#ifdef ADS1248
	if (regAdress == IDAC0) {
		regValue = regValue | IDAC0_ID;	  // add non 0 non-write register value IDAC0_ID
	}
#endif
	uint8_t regValuePre = ads12xx::GetRegisterValue(regAdress);
	// Catch to get non-write ID register values
//#ifdef ADS1256
//	if (regAdress == STATUS) {
//		regValue = regValue | 0x30;
//	}
//#endif
	//Serial.println(WREG|regAdress,HEX);

	if (regValue != regValuePre) {
		delayMicroseconds(10);
		waitforDRDY();
		SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
		digitalWrite(_CS, LOW);
		delayMicroseconds(10);
		SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
		SPI.transfer(0x00);		// send 2nd command byte, write only one register
		SPI.transfer(regValue);         // write data (1 Byte) for the register
		delayMicroseconds(10);
		digitalWrite(_CS, HIGH);
		if (regValue != ads12xx::GetRegisterValue(regAdress)) {
			Serial.print("Write to Register 0x");
			Serial.print(regAdress, HEX);
			Serial.println(" failed!");
		}
		else
		SPI.endTransaction();
	}

}

unsigned long ads12xx::GetRegisterValue(uint8_t regAdress) {
	waitforDRDY();
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	uint8_t bufr;
	digitalWrite(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
	SPI.transfer(0x00);			// send 2nd command byte, read only one register
	delayMicroseconds(10);
	bufr = SPI.transfer(NOP);	// read data of the register
	delayMicroseconds(10);
	digitalWrite(_CS, HIGH);
	return bufr;
	SPI.endTransaction();
}

/*
Sends a Command to the ADC
Like SELFCAL, GAIN, SYNC, WAKEUP
*/
void ads12xx::SendCMD(uint8_t cmd) {
	waitforDRDY();
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(cmd); 
	delayMicroseconds(10);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

void ads12xx::Reset(const int RESET_PIN) {
#ifdef ADS1256
	//Pulse reset line.
	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, HIGH);
	digitalWrite(RESET_PIN, LOW);
	delayMicroseconds(100);
	digitalWrite(RESET_PIN, HIGH); //Reset line must be high bevo continue"
#endif
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(10);
	//SPI.transfer(RESET); //Reset
	//delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(SDATAC); //Issue SDATAC
	delayMicroseconds(100);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

void Error(char* msg) {
	Serial.println(msg);
}

void ads12xx::SetRegister(regValues_t regValues) {
	for (uint8_t i = 0; i < sizeof(regValues) / sizeof(uint8_t);i++) {
		//ads12xx::SetRegisterValue(i,regValues[i]);
	}
}

long ads12xx::readSingle(regValues_t regValues) {
	long val = ads12xx::GetConversion();
	return val;
}

void ads12xx::calibration(int cal_cmd) {
	int data[3];
	String cmd_name;
	if (cal_cmd == SYSGCAL) {
		data[2] = FSC2;
		data[1] = FSC1;
		data[0] = FSC0;
		cmd_name = "FSC: ";
	}
	if (cal_cmd == SYSOCAL) {
		data[2] = OFC2;
		data[1] = OFC1;
		data[0] = OFC0;
		cmd_name = "OFC: ";
	}
	if (cal_cmd == SELFCAL) {
		data[2] = OFC2;
		data[1] = OFC1;
		data[0] = OFC0;
		cmd_name = "self OFC: ";
	}

	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWrite(_CS, LOW);
	delayMicroseconds(1);
	SPI.transfer(cal_cmd);
	delay(10);
	uint32_t val = 0;
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


