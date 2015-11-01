#ifndef ads1256_H
#define ads1256_H

/* For information to the register and settings see manual page (p..) */

/* ADS1248 Register (see p42 for Register Map) */

#define		STATUS    0x00 //Status Control Register 0
#define		MUX 	  0x01 //Multiplexer Control Register 0
#define		ADCON     0x02 //A/D Control Register 0
#define		DRATE	  0x03 //A/D Data Rate Control Register 0
#define		IO	      0X04 //GPIO Control Register 0
#define		OFC0	  0x05 //Offset Calibration Coefficient Register 1
#define		OFC1	  0x06 //Offset Calibration Coefficient Register 2
#define		OFC2	  0x07 //Offset Calibration Coefficient Register 2
#define		FSC0 	  0x08 //Full scale Callibration Coefficient Register 0
#define		FSC1	  0x09 //Full scale Callibration Coefficient Register 1
#define		FSC2	  0x0A //Full scale Callibration Coefficient REgister 2

/*STATUS - Status Control Register 0 ( see p30)*/
/* BIT7 - BIT6 -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* ID   - ID   -  ID     -  ID     -  ORDER  -  ACAL   -  BUFEN  -  DRDY */
#define STATUS_RESET 0x01 // Reset STATUS Register
/*Bits 7 - 4 ID3, ID2, ID1, ID0 Factory Programmed Identification Bits(Read Only)*/
/*ORDER1:0  Data Output Bit Order*/
#define ORDER_MSB B00000000 // Most significant Bit first (default)
#define ORDER_LSB B00001000//Least significant Bit first
/*Input data is always shifted in most significant byte and bit first. Output data is always shifted out most significant
byte first. The ORDER bit only controls the bit order of the output data within the byte.*/
/*ACAL1:0 Auto Calibration*/
#define ACAL_OFF B00000000 // Auto Calibration Disabled (default)
#define ACAL_ON  B00000100 // Auto Calibration Enabled
/*When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
values.*/
/*BUFEN1:0 Analog Input Buffer Enable*/
#define BUFEN_OFF B00000000 //Buffer Disabled (default)
#define BUFEN_ON  B00000010 //BUffer Enabled
/*DRDY1:0 Data Ready (Read Only) Duplicates the state of the DRDY pin*/

/* MUX - Multiplexer Control Register 0 (see p31 - bring together with bitwise OR | */
/* BIT7  - BIT6  -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* PSEL3 - PSEL2 -  PSEL1  -  PSEL0  -  NSEL3  -  NSEL2   - NSEL1   - NSEL0 */
#define MUX_RESET 0x01      // Reset MUX0 Register
/* PSEL3:0 Positive input channel selection bits */
#define P_AIN0 B00000000 //(default)
#define P_AIN1 B00010000
#define P_AIN2 B00100000
#define P_AIN3 B00110000
#define P_AIN4 B01000000
#define P_AIN5 B01010000
#define P_AIN6 B01100000
#define P_AIN7 B01110000
#define P_AINCOM B10000000
/* NSEL3:0 Negativ input channel selection bits */
#define N_AIN0 B00000000
#define N_AIN1 B00000001 //(default)
#define N_AIN2 B00000010
#define N_AIN3 B00000011
#define N_AIN4 B00000100
#define N_AIN5 B00000101
#define N_AIN6 B00000110
#define N_AIN7 B00000111
#define N_AINCOM B00001000

/*ADCON - A/D Control Register 0 ( see p31)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* 0    - CLK1   -  CLK0   -  SDCS1  -  SDCS0  -  PGA2   -  PGA1   -  PAG0 */
#define ADCON_RESET 0x20 // Reset ADCON Register
/*CLK2:0 D0/CLKOUT Clock Out Rate Setting*/
#define CLK_OFF B00000000 //Clock Out off
#define CLK_1   B00100000 //Clock Out Frequency = fCLKIN (default)
#define CLK_2   B01000000 //Clock Out Frequency = fCLKIN/2
#define CLK_4   B01100000 //Clock Out Frequency = fCLKIN/4
/*When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.*/
/*SDCS2:0 Sensor Detection Current Sources*/
#define SDCS_OFF B00000000//Sensor Detect Off (default)
#define SDCS_05  B00001000//Sensor Detect Current 0.5?A
#define SDCS_2   B00010000//Sensor Detect Current 2?A
#define SDCS_10  B00011000//Sensor Detect Current 10?A
/*The Sensor Detect Current Sources can be activated to verify the integrity of an external sensor supplying a signal to the
ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.*/
/*PGA3:0 Programmable Gain Amplifier Setting*/
#define PGA_1 //(default)
#define PGA_2
#define PGA_4
#define PGA_8
#define PGA_16
#define PGA_32
#define PGA_64

/*DRATE - A/D Data Rate Register 0 ( see p32)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* DR7  - DR6    -  DR5    -  DR4    -  DR3    -  DR2    -  DR1    -  DR0 */
#define DRATE_RESET 0xF0 // Reset DRATE Register
/*DR7:0 Data Rate Setting*/
#define DR_30000 B11110000 //30.000 SPS (default)
#define DR_15000 B11100000 //15.000 SPS
#define DR_7500  B11010000 //7.500 SPS
#define DR_3750  B11000000 //3.750 SPS
#define DR_2000  B10110000 //2.000 SPS
#define DR_1000  B10100001 //1.000 SPS
#define DR_500   B10010010 //500 SPS
#define DR_100   B10000010 //100 SPS
#define DR_60    B01110010 //60 SPS
#define DR_50    B01100011 //50 SPS
#define DR_30    B01010011 //30 SPS
#define DR_25    B01000011 //25 SPS
#define DR_15    B00110011 //15 SPS
#define DR_10    B00100011 //10 SPS
#define DR_5     B00010011 //5 SPS
#define DR2_5    B00000011 //2,5 SPS

/*IO - GPIO Control Register 0 ( see p32)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* DIR3 - DIR2   -  DIR1   -  DIR0   -  DIO3   -  DIO2   -  DIO1   -  DIO0 */
#define IO_RESET 0xE0 // Reset IO Register
/*DIR3 - Digital I/O Direction for Pin D3*/
#define DIR3_OUT B00000000 //D3 is an output
#define DIR_IN   B10000000 //D3 is an input (default)
/*DIR2 - Digital I/O Direction for Pin D3*/
#define DIR2_OUT B00000000 //D2 is an output
#define DIR2_IN   B01000000 //D2 is an input (default)
/*DIR1 - Digital I/O Direction for Pin D3*/
#define DIR1_OUT B00000000 //D1 is an output
#define DIR1_IN   B001ßß000 //D1 is an input (default)
/*DIR0 - Digital I/O Direction for Pin D3*/
#define DIR0_OUT B00000000 //D0/CLKOUT is an output
#define DIR0_IN   B00010000 //D0/CLKOUT is an input (default)
/*DIO3:0 Status of Digital I/O, Read Only*/

/* SPI COMMAND DEFINITIONS (p34) */
/*SYSTEM CONTROL */
#define		WAKEUP		0x00 	//Exit Sleep Mode
#define 	STANDBY		0xFD	//Enter Sleep Mode
#define 	SYNC 		0xFC    //Synchornize the A/D Conversion
#define 	RESET		0xFE	//Reset To Power UP values
#define		NOP			0xFF	//No operation
/*DATA READ*/
#define		RDATA		0x01	//Read data once
#define 	RDATAC		0x03	//Read data continously
#define 	SDATAC 		0x0F	//Stop reading data continously
/*READ REGISTER */
#define 	RREG		0x10	//Read From Register
#define 	WREG 		0x50	//Write To Register
/*Calibration */
#define 	SYSOCAL		0xF3	//System Offset Calibration
#define 	SYSGCAL		0xF2	//System Gain Calibration
#define 	SELFCAL	    0xF0	//Self Offset Calibration

#define SPI_SPEED 1700000


#endif
