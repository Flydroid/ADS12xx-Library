#ifndef ads12xx_H
#define ads12xx_H

//#define ADS1248
//define ADS1256

/*Function */


#ifdef ADS1248
#include "ads1248.h"
#endif // ADS1248

#ifdef ADS1256
#include "ads1256.h"
#endif // ADS1256

#include "SPI.h"
class ads12xx{
public:
	ads12xx(
		const int CS,
		const int START,
		const int DRDY
		);



	void Reset(
		);

	unsigned long  GetRegisterValue(
		uint8_t regAdress
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
		uint8_t STATUS_val;
		uint8_t MUX_val;
		uint8_t ADCON_val;
		uint8_t DRATE_val;
		uint8_t IO_val;
#endif // ADS1256


		

	};

	long readSingle(
		regValues_t regValues
		);



	long  GetConversion(
		);
	void calibration(
		int cal_cmd
		);
	void SetRegister(
		regValues_t regValues
		);

private:
	int _CS;
	int _DRDY;
	int _START;


};
#endif
