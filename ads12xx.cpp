#include "ads12xx.h"

volatile int DRDY_state = HIGH;

// Waits untill DRDY Pin is falling (see Interrupt setup). 
// Some commands like WREG, RREG need the DRDY to be low.
bool waitforDRDY() {
	int timeout=0;
	timeout = millis();
	while (DRDY_state) {
		//Serial.println("Wait for DRDY");
		//if(millis()-timeout >10000){
		//	return 0;
		//}
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

void ads12xx::begin(int CS, int START, int DRDY, int _RESET) {
	pinMode(CS, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWriteFast(CS, HIGH); // CS HIGH = nothing selected
	pinMode(DRDY, INPUT);             // DRDY read
	pinMode(START, OUTPUT);
	digitalWriteFast(START, HIGH);
	pinMode(RESET, OUTPUT);              // set the slaveSelectPin as an output:
	digitalWriteFast(RESET, HIGH);
	_CS = CS;
	_DRDY = DRDY;
	delay(500);
	SPI.begin();

	attachInterrupt(digitalPinToInterrupt(_DRDY), DRDY_Interuppt, FALLING); //Interrupt setup for DRDY detection

	delay(500);
	

	
}


// function to get a 3byte conversion result from the adc
long ads12xx::GetConversion() {
	int32_t regData;;
	if(waitforDRDY()==0){  // Wait until DRDY is LOW
		Serial.println("Wait for DRDY Timeout");
		return 0;
		}; 
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	digitalWriteFast(_CS, LOW); //Pull SS Low to Enable Communications with ADS1247
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
	digitalWriteFast(_CS, HIGH);
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
		//digitalWriteFast(_START, HIGH);
		delayMicroseconds(10);
		//waitforDRDY();
		SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
		digitalWriteFast(_CS, LOW);
		delayMicroseconds(10);
		//SPI.transfer(SDATAC); //Issue SDATAC
		delayMicroseconds(10);
		SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
		SPI.transfer(0x00);		// send 2nd command byte, write only one register
		SPI.transfer(regValue);         // write data (1 Byte) for the register
		delayMicroseconds(10);
		digitalWriteFast(_CS, HIGH);
		//digitalWriteFast(_START, LOW);
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
	uint8_t bufr;
	//digitalWriteFast(_START, HIGH);
	//waitforDRDY();
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
	digitalWriteFast(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
	SPI.transfer(0x00);			// send 2nd command byte, read only one register
	delayMicroseconds(10);
	bufr = SPI.transfer(NOP);	// read data of the register
	delayMicroseconds(10);
	digitalWriteFast(_CS, HIGH);
	//digitalWriteFast(_START, LOW);
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
	digitalWriteFast(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(cmd);
	delayMicroseconds(10);
	digitalWriteFast(_CS, HIGH);
	SPI.endTransaction();
}


// function to reset the adc
void ads12xx::Reset() {
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with  clock, MSB first, SPI Mode1
	digitalWriteFast(_CS, LOW);
	delayMicroseconds(10);
	SPI.transfer(RESET); //Reset
	delay(2); //Minimum 0.6ms required for Reset to finish.
	SPI.transfer(SDATAC); //Issue SDATAC
	delayMicroseconds(100);
	digitalWriteFast(_CS, HIGH);
	SPI.endTransaction();
}

