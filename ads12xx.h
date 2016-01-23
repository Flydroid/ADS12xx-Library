#ifndef ads12xx_H
#define ads12xx_H

//#define ADS1248
//#define ADS1256
#define ADS1258

/*Serial Template */

#ifdef ADS1248
#include "ads1248.h"
#endif // ADS1248

#ifdef ADS1256
#include "ads1256.h"
#endif // ADS1256

#ifdef ADS1258
#include "ads1258.h"
#endif // ADS1258

#include "SPI.h"
class ads12xx {
public:
	ads12xx(
		const int CS,
		const int START,
		const int DRDY
		);

	void Reset(
		const int RESET_PIN
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
#ifdef ADS1258
		uint8_t CONFIG0_val;
		uint8_t CONFIG1_val;
		uint8_t MUXSCH_val;
		uint8_t MUXDIF_val;
		uint8_t SYSRED_val;
		uint8_t GPIOC_val;
		uint8_t GPIOD_val;
		uint8_t ID_val;
#endif // ADS1258 
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
	volatile int DRDY_state;
};
#endif
