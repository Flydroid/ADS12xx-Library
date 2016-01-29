#ifndef ads1258_H
#define ads1258_H

/* For information to the register and settings see manual page (p..) */

/* ADS1258 Register (see p36 for Register Map) */

#define		CONFIG0	0x00 //Configuration Register 0
#define		CONFIG1	0x01 //Configuration Register 1
#define		MUXSCH	0x02 //Multiplexer Fixed Channel Register
#define		MUXDIF	0x03 //Multiplexer Differential Input Select Register
#define		MUXSG0	0X04 //Multiplexer Single-Ended Input Select Register 0
#define		MUXSG1	0x05 //Multiplexer Single-Ended Input Select Register 1
#define		SYSRED	0x06 //System Reading Select Register
#define		GPIOC	0x07 //GPIO Configuration Register
#define		GPIOD	0x08 //GPIO Data Register
#define		ID		0x09 //Device ID Register


/*Configuration Register 0 ( see p36)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* 0 	- SPIRST - MUXMOD - BYPAS - CLKENB - CHOP - STAT - 0 */

/*Bit 7 - Must be 0 ( Default )*/

/*SPIRST:0  SPI Interface Reset Timer*/
#define SPIRST_LONG  B00000000 // Reset when SCLK inactive for 4096 cycles 
#define SPIRST_SHORT B01000000 // Reset when SCLK inactive for 256 cycles
/*Sets the number of fclk cycles in which SCLK is inactive the SPI interface will reset. 
Only SPI interface reset and not device. When SPI is reset, it is ready for a new command*/

/*MUXMOD:0 Auto-scan mode ( default )*/
#define MUXMOD_AUTO B00000000 // Auto-Scan Mode ( default)
#define MUXMOD_FIXED B00100000 // Fixed-Channel Mode
/* In auto scan mode, input channel selections are eight differential channels (DIFF0-DIFF7) and 16 single-ended channels (AIN0-AIN15).
Additionally, five internal monitor readings can be selected. These selections are made in registers MUXDIF, MUXSG0, MUXSG1, and SYSRED.
In this mode, settings in register MUXSCH have no effect.
In fixed channel mode, any of the analog input channels may be selected for the positive measurement and the negative measurement channels.
The inputs are selected in register MUXSCH. In this mode, registers MUXDIF, MUXSG0, MUXSG1, and SYSRED have no effect. Note that it is not
possible to select the internal monitor readings in this mode.*/

/*BYPAS:0 ADC inputs use internal multiplexer connection (default)*/
#define BYPAS_INT B00000000 // ADC inputs use internal multiplexer connection (default)
#define BYPAS_EXT B00010000 // ADC inputs use external ADC inputs (ADCINP and ADCINN)
/* Note that temperature, Vcc, Gain, and Reference internal monitor readings automatically use the internal connection, regardless
of the BYPAS setting. The Offset reading uses the setting of BYPAS.*/

/*CLKENB:0 Clock output on CLKIO disabled */
#define CLKENB_DISABLE B00000000 // Clock output on CLKIO disabled.
#define CLKENB_ENABLE B00001000 // Clock output on CLKIO enabled (default)
/*Note: if the CLKSEL pin is set to '1', the CLKIO pin is a clock input only. In this case, setting this bit has no effect.*/

/*CHOP:0 Chopping disabled (default)*/
#define CHOP_DISABLE B00000000 // Chopping Disabled (Default)
#define CHOP_ENABLE B00000100 // Chopping Enabled
/* The chopping features corrects for offset originating from components used in the external multiplexer loop. Note that for Internal System readings
(Temperature, VCC, Gain, and Reference) the CHOP bit must be 0.*/

/*STAT:0 Status byte disabled*/
#define STAT_DISABLE B00000000 // Status Byte Disabled
#define STAT_ENABLE B00000010 // Status Byte Enabled (default)

/* Bit 8 - Must be '0' */

/*Configuration Register 1 ( see p38)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* IDLMOD 	- DLY2 - DLY1 - DLY0 - SBCS1 - SBCS0 - DRATE1 - DRATE0 */

/* IDLMOD:0 Select Standby Mode*/
#define IDLMOD_STANDBY B00000000 // Select Standby Mode
#define IDLMOD_SLEEP B10000000 // Select Sleep Mode (default)

/* DLY2, DLY1, DLY0 Sets the amount of time the converter will delay after indexing to a new channel but before starting a new conversion. See p.38*/
#define DLY2_on B01000000 // Delay 2 on
#define DLY1_on B00100000 // Delay 1 on
#define DLY0_on B00010000 // Delay 0 on
/* Default = 000*/

/* SBCS[1:0] These bits set the sensor bias current source. */
#define SBCS_OFF B00000000 // Sensor Bias Current Source Off (default)
#define SBCS_1 	 B00000100 // 1.5 uA source
#define SBCS_3   B00001100 // 24 uA source

/* DRATE[1:0] These bits set the data rate at the converter.*/
#define DRATE_0 B00000000 // DRATE  1831 sps
#define DRATE_1 B00000001 // DRATE  6168 sps
#define DRATE_2 B00000010 // DRATE 15123 sps
#define DRATE_3 B00000011 // DRATE 23739 sps

/*Configuration Register MUXSCH Multiplexer Fixed Channel Register ( see p39)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* AINP3 	- AINP2 - AINP1 - AINP0 - AINN3 - AINN2 - AINN1 - AINN0 */
#define AINN0	B00000000 // Select AINN0
#define AINN1	B00000001 // Select AINN1
#define AINN2	B00000010 // Select AINN2
#define AINN3	B00000011 // Select AINN3
#define AINN4	B00000100 // Select AINN4
#define AINN5	B00000101 // Select AINN5
#define AINN6	B00000110 // Select AINN6
#define AINN7	B00000111 // Select AINN7
#define AINN8	B00001000 // Select AINN8
#define AINN9	B00001001 // Select AINN9
#define AINN10	B00001010 // Select AINN10
#define AINN11	B00001011 // Select AINN11
#define AINN12	B00001100 // Select AINN12
#define AINN13	B00001101 // Select AINN13
#define AINN14	B00001110 // Select AINN14
#define AINN15	B00001111 // Select AINN15
#define AINP0	B00000000 // Select AINP0
#define AINP1	B00010000 // Select AINP1
#define AINP2	B00100000 // Select AINP2
#define AINP3	B00110000 // Select AINP3
#define AINP4	B01000000 // Select AINP0
#define AINP5	B01010000 // Select AINP1
#define AINP6	B01100000 // Select AINP2
#define AINP7	B01110000 // Select AINP3
#define AINP8	B10000000 // Select AINP0
#define AINP9	B10010000 // Select AINP1
#define AINP10	B10100000 // Select AINP2
#define AINP11	B10110000 // Select AINP3
#define AINP12	B11000000 // Select AINP0
#define AINP13	B11010000 // Select AINP1
#define AINP14	B11100000 // Select AINP2
#define AINP15	B11110000 // Select AINP3

/*Configuration Register MUXDIF Multiplexer Differential Input Select Register (Address = 03h) ( see p39)
This register selects input channels of multiplexer to be used for Fixed channel mode*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* DIFF7	- DIFF6 - DIFF5 - DIFF4 - DIFF3 - DIFF2 - DIFF1 - DIFF0 */
#define DIFF7 B10000000 // Select DIFF7
#define DIFF6 B01000000 // Select DIFF6
#define DIFF5 B00100000 // Select DIFF5
#define DIFF4 B00010000 // Select DIFF4
#define DIFF3 B00001000 // Select DIFF3
#define DIFF2 B00000100 // Select DIFF2
#define DIFF1 B00000010 // Select DIFF1
#define DIFF0 B00000001 // Select DIFF0

/* Configuration Register MUXSG0 Multiplexer Single-Ended Input Select Register 0 (Address = 04h) (see p39)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* AIN7	- AIN6 - AIN5 - AIN4 - AIN3 - AIN2 - AIN1 - AIN0 */
#define AIN7 B10000000
#define AIN6 B01000000
#define AIN5 B00100000
#define AIN4 B00010000
#define AIN3 B00001000
#define AIN2 B00000100
#define AIN1 B00000010
#define AIN0 B00000001

/* Configuration Register MUXSG1 Multiplexer Single-Ended Input Select Register 1 (Address = 05h) (see p39)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* AIN15	- AIN14 - AIN13 - AIN12 - AIN11 - AIN10 - AIN9 - AIN8 */
#define AIN15 B10000000
#define AIN14 B01000000
#define AIN13 B00100000
#define AIN12 B00010000
#define AIN11 B00001000
#define AIN10 B00000100
#define AIN9 B00000010
#define AIN8 B00000001

/* Configuration Register SYSRED System Reading Select Register (Address = 06h) (see p39)*/
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* 0	- 0 - REF - GAIN - TEMP - VCC - 0 - OFFSET */
#define REF B00100000
#define GAIN B00010000
#define TEMP B00001000
#define VCC B00000100
#define OFFSET B00000001
/* Bits indicated as '0' must be set to 0*/

/* Configuration Register GPIOC GPIO Configuration Register */
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* CIO7	- CIO6 - CIO5 - CIO4 - CIO3 - CIO2 - CIO1 - CIO0 */
/* 0=GPIO is an output, 1=GPIO is an input (default). Should not be left floating.*/
#define CIO7_input B10000000 // Pin GPIO7 input
#define CIO6_input B01000000 // Pin GPIO6 input
#define CIO5_input B00100000 // Pin GPIO5 input
#define CIO4_input B00010000 // Pin GPIO4 input
#define CIO3_input B00001000 // Pin GPIO3 input
#define CIO2_input B00000100 // Pin GPIO2 input
#define CIO1_input B00000010 // Pin GPIO1 input
#define CIO0_input B00000001 // Pin GPIO0 input
#define CIO7_output B00000000 // PIN GPIO7 output
#define CIO6_output B00000000
#define CIO5_output B00000000
#define CIO4_output B00000000
#define CIO3_output B00000000
#define CIO2_output B00000000
#define CIO1_output B00000000
#define CIO0_output B00000000

/* Configuration Register GPIOD: GPIO Data Register */
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* DIO7	- DIO6 - DIO5 - DIO4 - DIO3 - DIO2 - DIO1 - DIO0 */
/* This register is used to read and write data to the GPIO port pins. Data returned corresponds to GPIO external pin state
Logic low 0=GPIO is default */
#define DIO7_high B10000000 // Pin GPIO7 input
#define DIO6_high B01000000 // Pin GPIO6 input
#define DIO5_high B00100000 // Pin GPIO5 input
#define DIO4_high B00010000 // Pin GPIO4 input
#define DIO3_high B00001000 // Pin GPIO3 input
#define DIO2_high B00000100 // Pin GPIO2 input
#define DIO1_high B00000010 // Pin GPIO1 input
#define DIO0_high B00000001 // Pin GPIO0 input
#define DIO7_low B00000000 // PIN GPIO7 output
#define DIO6_low B00000000
#define DIO5_low B00000000
#define DIO4_low B00000000
#define DIO3_low B00000000
#define DIO2_low B00000000
#define DIO1_low B00000000
#define DIO0_low B00000000

/* Device ID register */
/* BIT7 - BIT6   -  BIT5  -  BIT4 - BIT3   - BIT2 - BIT1 - BIT0 */
/* ID7	- ID6 - ID5 - ID4 - ID3 - ID2 - ID1 - ID0 */
/* Bits 7-0 are factory programmed ID bits, read only. ID4 is 0 for ADS1258 24 bit, 1 for ADS1158 16 bit. */

/* SPI COMMAND DEFINITIONS (p35) */
/* Command Byte C2 - C1 - C0 - MUL - A3 - A2 - A1 - A0 */
/* MUL allows for accessing multiple registers */

#define MUL		0x10	//Multiple register write enabled
#define CDRD	0x00	//Channel Data read direct (no command) (CDRD in 1258 data sheet)
#define RDATA	0x30	//Channel data read command (register format, set MUL=1, status byte included in data) data sheet CDRC
#define RREG	0x40	//Register read command A[3:0]= 0000
#define WREG	0x60	//Register write command
#define PULCON	0x80	//Pulse convert command, MUL, A[3:0]: don't care
#define RESET	0xC0	//Reset 
#define	NOP		0x00	//

#define 	SDATAC 		0x0F	//Stop reading data continously, from 1256, not sure that will work

#define SPI_SPEED 4000000

#endif
