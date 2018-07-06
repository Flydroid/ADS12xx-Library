#ifndef ads12xx_H
#define ads12xx_H

#define ADS1248
//#define ADS1256

/*Serial Template */

#ifdef ADS1248
#include "ads1248.h"
#endif // ADS1248

#ifdef ADS1256
#include "ads1256.h"
#endif // ADS1256

#include "SPI.h"
class ads12xx {
public:
	ads12xx();
	void begin(
		int CS,
		int START,
		int DRDY,
		int _RESET
		);

	void Reset(
		
		);

	unsigned long  GetRegisterValue(
		uint8_t regAdress
		);
	
	void SendCMD(
		uint8_t cmd
		);

	void SetRegisterValue(
		uint8_t regAdress,
		uint8_t regValue
		);

	struct regValues_t
	{
#ifdef ADS1248
		uint8_t MUX0_val;
		uint8_t VBIAS_val;
		uint8_t MUX1_val;
		uint8_t SYS0_val;
		uint8_t IDAC0_val;
		uint8_t IDAC1_val;
#endif // ADS1248
#ifdef ADS1256
		uint8_t STATUS_val = STATUS_RESET;
		uint8_t MUX_val = MUX_RESET;
		uint8_t ADCON_val = ADCON_RESET;
		uint8_t DRATE_val = DRATE_RESET;
		uint8_t IO_val = IO_RESET;
#endif // ADS1256
	};


	long  GetConversion(
		);
	private:
	int _CS;
	int _DRDY;
	int _START;
	volatile int DRDY_state;
};
#endif
