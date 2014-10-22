#ifndef ads1248_h
#define ads1248_h

#define BCS10 0x00
#define VBIAS 0x01
#define MUX0 0x00




//---------------------------------------------------//
//Defines for MUX1 -Multiplexer Controllregister
//--------------------------------------------------//
#define MUX1 0x02 //MUX1 Address
#define CLKSTAT7 0 // ready only
#define VREFCON65 00 // controls the internal voltage referens 
#define REFSELT43 00 //select the referens input for the ADC
#define MUXCAL20 000 //used to select system Monitor, overrides MUX_SP, MUX_SN, VBIAs
#define MUX1DATA  CLKSTAT7 <<7 & VREFCON65 <<5 & REFSELT43 <<4 & MUXCAL20


//------------------------------------
//     Configuration
//--------------------------------------
#define C_CONTINUOUS LOW //Start without continous conversion
// Channel Configuration:
//CH_BUF[Number of Channels in use] = {addresses for theses channels according to the MUX0 register}





#include <SPI.h>

class ads1248{
	public:
		ads1248();
		void begin(
			const int CS,
			const int START,
			const int DRDY
			);
		unsigned long readADConce();
		unsigned long readADC();
		void selectChannel(
			int CH_NUM			// channel to switch to			
			);
		unsigned long readRegister(uint8_t regadd);
		void writeRegister(uint8_t regadd, uint8_t regbyte);
		void reset();
		uint8_t CH_BUF[7] = { 0x07, 0x0F, 0x17, 0x1F, 0x27, 0x2F, 0x37 }; // Example with 7 channels which measure to GND.
		//uint8_t DIFF[4] = { 0x01, 0x13, 0x25, 0x37 };		// 1 to 3 for Differntial measurement, 4 for PT100 for cold junction reference
		//uint8_t TGND[7] = { 0x07, 0x0F, 0x17, 0x1F, 0x27, 0x2F, 0x37 }; //channel 1 to 7 for measure to ground, 8 is  tied to GND via Transistor controllable with digital IO
	private:
		int _CS;


};
#endif