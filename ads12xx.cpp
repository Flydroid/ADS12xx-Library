#include "ads12xx.h"

volatile int DRDY_state = HIGH;

// Waits untill DRDY Pin is falling (see Interrupt setup). 
// Some commands like WREG, RREG need the DRDY to be low.
void waitforDRDY() {
	while (DRDY_state) {
		continue;
	}
	noInterrupts();
	DRDY_state = HIGH;
	interrupts();
}

//Interrupt function
void DRDY_Interuppt() {
	DRDY_state = LOW;
}


// ads12xx setup
ads12xx::ads12xx() {}

void ads12xx::begin(int CS, int START, int DRDY) {
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWrite(CS, HIGH); // CS HIGH = nothing selected
#ifdef ADS1248
#endif
	pinMode(DRDY, INPUT);             // DRDY read
	pinMode(START, OUTPUT);
	digitalWrite(START, HIGH);
	_CS = CS;
	_DRDY = DRDY;
	delay(500);
	SPI.begin();

	attachInterrupt(_DRDY, DRDY_Interuppt, FALLING); //Interrupt setup for DRDY detection

	delay(500);

	// interal VREF einschalten
}


// function to get a 3byte conversion result from the adc
long ads12xx::GetConversion() {
	int32_t regData;
	waitforDRDY(); // Wait until DRDY is LOW
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
	

	return regData;
}


// function to write a register value to the adc
// argumen: adress for the register to write into, value to write
void ads12xx::SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
#ifdef ADS1248
	if (regAdress == IDAC0) {
		regValue = regValue | IDAC0_ID;	  // add non 0 non-write register value IDAC0_ID
	}
#endif
	uint8_t regValuePre = ads12xx::GetRegisterValue(regAdress);
	if (regValue != regValuePre) {
		//digitalWrite(_START, HIGH);
		delayMicroseconds(10);
		waitforDRDY();
		SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
		digitalWrite(_CS, LOW);
		delayMicroseconds(10);
		SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
		SPI.transfer(0x00);		// send 2nd command byte, write only one register
		SPI.transfer(regValue);         // write data (1 Byte) for the register
		delayMicroseconds(10);
		digitalWrite(_CS, HIGH);
		//digitalWrite(_START, LOW);
		if (regValue != ads12xx::GetRegisterValue(regAdress)) {   //Check if write was succesfull
			Serial.print("Write to Register 0x");
			Serial.print(regAdress, HEX);
			Serial.println(" failed!");
		}
		SPI.endTransaction();
		
	}

}


//function to read a register value from the adc
//argument: adress for the register to read
unsigned long ads12xx::GetRegisterValue(uint8_t regAdress) {
	//digitalWrite(_START, HIGH);
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
	//digitalWrite(_START, LOW);
	SPI.endTransaction();
	return bufr;
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


// function to reset the adc
void ads12xx::Reset() {
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with  clock, MSB first, SPI Mode1
	digitalWrite(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(RESET); //Reset
	delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(SDATAC); //Issue SDATAC
	delayMicroseconds(100);
	digitalWrite(_CS, HIGH);
	SPI.endTransaction();
}

