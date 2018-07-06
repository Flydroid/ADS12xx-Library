#include <Arduino.h>
#include <SPI.h>
#include "ads12xx.h"
#include <Streaming.h>

/*
Working: Everything working
- START: set HIGH
- RESET: set HIGH
- CLOK: set LOW



*/

int  START = 8;
int  CS = 9;
int  DRDY = 10;
int _RESET = 7;


//Define PT100 Coeffients
static double R0 =  100;
static double A =  0.00390802;
static double B = -0.0000005802;

//Define Thermocouple Type-K Coeffients
static double a0 = 0.118597600000E+00;
static double  a1 = -0.118343200000E-03;
static double  a2 =  0.126968600000E+03;

static double c_i[10]={
 -0.176004136860E-01,
  0.389212049750E-01,
  0.185587700320E-04,
 -0.994575928740E-07,
  0.318409457190E-09,
 -0.560728448890E-12,
  0.560750590590E-15,
 -0.320207200030E-18,
  0.971511471520E-22,
 -0.121047212750E-25};


//Define Thermocouple Type-K inverse Coeffients
//Range 0°C to 500°C
static double d_n[10] = {
	0.00E+00,
	2.51E+01,
	7.86E-02,
	-2.50E-01,
	8.32E-02,
	-1.23E-02,
	9.80E-04,
	-4.41E-05,
	1.06E-06,
	-1.05E-08};

 

//Define which ADC to use in the ads12xx.h file
ads12xx ADS;

void test_intTemp(void);
void test_intTemp(void);
void test_supVoltage(void);
void test_extrefVoltage(void);
double test_RTD(void);
void test_Thermo(void);
void test_inputRange(void);

void setup()
{
	
	Serial.begin(115200);
	while (!Serial) {
		
	}
	Serial.println("Serial online");
	//pinMode(20, OUTPUT);
	//digitalWrite(20, HIGH); //Force other Chip into CS high
	ADS.begin(CS, START, DRDY,_RESET);  //initialize ADS as object of the ads12xx class

	ADS.Reset();
	
	delay(10);
	ADS.SetRegisterValue(OFC0,50); //set calibrated offset register Value 

	Serial.println("Commands for testing:");
	Serial.println("'r' to read Register");
	Serial.println("'w' to write Register");
	Serial.println("'R' to get a Conversion Result");
	Serial.println("'x' to SDATAC, 'd' for SDATA");
	Serial.println("'o' to write Pre Predefinde Registers");
	Serial.println("'f' to write a command");
	Serial.println("'c' to select calibration methode");
	Serial.println("'t' to chose test");
	Serial.println("'h' for this help");
}


void loop() {

	if (Serial.available()) {
		char cin = Serial.read();
		char  check_ser = 'y';
		uint8_t cmd, FSC_0, FSC_1, FSC_2;
		uint8_t cin1, OFC_0, OFC_1, OFC_2;
	
		switch (cin) {
		case 'r':
			Serial.println("Which Register to read?");
			while (!Serial.available());
			Serial.print("Register Value for: ");
			cin1 = Serial.parseInt();
			Serial.println(cin1);
			Serial.println(ADS.GetRegisterValue(cin1));
			break;
		case 'w':
			Serial.println("Which Register to write?");
			while (!Serial.available());
			cin1 = Serial.parseInt();
			Serial.println("Which Value to write?");
			while (!Serial.available());
			ADS.SetRegisterValue(cin1, Serial.parseInt());
			break;
		case 'R':
			Serial.println("Conversion Result");
			Serial.println(ADS.GetConversion());
			break;
		case 'x':
			Serial.println("Stop SDATAC");
			ADS.Reset();
			break;
		case 'o':
			Serial.println("Writing predefind Registers");
#ifdef ADS1256
			ADS.SetRegisterValue(MUX, P_AIN0 | N_AINCOM);
			ADS.SetRegisterValue(DRATE, DR_1000);
#endif
#ifdef ADS1248
			ADS.SetRegisterValue(SYS0, DOR3_2000 | PGA2_0);

			ADS.SetRegisterValue(IDAC0, DRDY_ONLY | IMAG2_OFF
				//	| IDAC0_ID
				);
			ADS.SetRegisterValue(IDAC1, I1DIR_OFF);
			ADS.SetRegisterValue(MUX1, BCS1_1 | MUX_SP2_AIN0 | MUX_SN2_AIN2);
#endif
			Serial.println("Writing sucessfull");
			break;
		case 'd':
			while (check_ser == 'y') {
				if (Serial.available()) {
					check_ser = Serial.read();

				}
				/*
				uint32_t data = ADS.GetConversion();
				int timer1 = micros();
				if (long minus = data >> 23 == 1) {
					long data = data - 16777216;
				}
				Serial.println(data);
				*/
				test_Thermo();

#ifdef ADS1256
				//	double voltage = (4.9986 / 8388608)*data;
				//	Serial.println(voltage);
#endif
			}
			break;
		case 'f':
			Serial.println("Which command to write");
			while (!Serial.available());
			cmd = Serial.parseInt();
			Serial.print(cmd, HEX);
			ADS.SendCMD(cmd);
			break;
		case 'c':
			Serial.println("Which Calibration to run?");
#ifdef ADS1256			
			Serial.println("'1' for SELFCAL");
#endif
#ifdef ADS1248
			Serial.println("'1' for SELFOCAL");
#endif
			Serial.println("'2' for SYSOGCAL\n'3' for SYSGCAL");
			while (!Serial.available());
			cmd = Serial.parseInt();
			switch (cmd)
			{
#ifdef ADS1256
			case 1:
				Serial.println("Preforming Self Gain and Offset Callibration");
				ADS.SendCMD(SELFCAL);
				delay(5);
				Serial.print("OFC0: ");
				Serial.println(ADS.GetRegisterValue(OFC0));
				Serial.print("OFC1: ");
				Serial.println(ADS.GetRegisterValue(OFC1));
				Serial.print("OFC2: ");
				Serial.println(ADS.GetRegisterValue(OFC2));
				Serial.print("FSC0: ");
				Serial.println(ADS.GetRegisterValue(FSC0));
				Serial.print("FSC1: ");
				Serial.println(ADS.GetRegisterValue(FSC1));
				Serial.print("FSC2: ");
				Serial.println(ADS.GetRegisterValue(FSC2));
				break;
#endif
#ifdef ADS1248
			case 1:
				Serial.println("Preforming Self Offset Callibration");
				ADS.SetRegisterValue(MUX0, MUX_SN2_AIN0 | MUX_SP2_AIN1);
				delay(1);
				ADS.SendCMD(SELFOCAL);
				delay(5);
				OFC_0 = ADS.GetRegisterValue(OFC0);
				OFC_1 = ADS.GetRegisterValue(OFC1);
				OFC_2 = ADS.GetRegisterValue(OFC2);
				Serial.print("OFC0 ");
				Serial.println(OFC_0);
				Serial.print("OFC1: ");
				Serial.println(OFC_1);
				Serial.print("OFC2: ");
				Serial.println(OFC_2);
				Serial.print("OFC Value:");
				Serial.println(OFC_0 | OFC_1 <<8 | OFC_2 << 16);
				break;
#endif
			case 2:
				Serial.println("Preforming System Offset Callibration");
				ADS.SetRegisterValue(MUX0, MUX_SN2_AIN0 | MUX_SP2_AIN1);
				delay(1);
				ADS.SendCMD(SYSOCAL);
				delay(5);
				OFC_0 = ADS.GetRegisterValue(OFC0);
				OFC_1 = ADS.GetRegisterValue(OFC1);
				OFC_2 = ADS.GetRegisterValue(OFC2);
				Serial.print("OFC1: ");
				Serial.println(OFC_0);
				Serial.print("OFC1: ");
				Serial.println(OFC_1);
				Serial.print("OFC2: ");
				Serial.println(OFC_2);
				Serial.print("OFC Value:");
				Serial.println(OFC_0 | OFC_1 <<8 | OFC_2 << 16);
				break;
			case 3:
				Serial.println("Preforming System Gain Callibration");
				ADS.SetRegisterValue(MUX0, MUX_SN2_AIN3 | MUX_SP2_AIN2);
				delay(1);
				ADS.SendCMD(SYSGCAL);
				delay(5);
				FSC_0 = ADS.GetRegisterValue(FSC0);
				FSC_1 = ADS.GetRegisterValue(FSC1);
				FSC_2 = ADS.GetRegisterValue(FSC2); 
				Serial.print("FSC0: ");
				Serial.println(FSC_0);
				Serial.print("FSC1: ");
				Serial.println(FSC_1);
				Serial.print("FSC2: ");
				Serial.println(FSC_2);
				Serial.print("FSC Value:");
				Serial.println(FSC_0 | FSC_1 <<8 | FSC_2 << 16);
				break;
			default:
				break;
			}
			break;
		case 'h':
			Serial.println("Commands for testing:");
			Serial.println("'r' to read Register");
			Serial.println("'w' to write Register");
			Serial.println("'R' to get a Conversion Result");
			Serial.println("'x' to SDATAC, 'd' for SDATA");
			Serial.println("'o' to write Pre Predefinde Registers");
			Serial.println("'f' to write a command");
			Serial.println("'c' to select calibration methode");
			Serial.println("'t' to chose test");
			Serial.println("'h' for this help");
			break;
#ifdef ADS1248
		case 't':
			Serial.println("Chose which test to run");
			Serial.println("'1' for Internal Temperature\n'2' for Supply Voltage Measurement\n'3' for External Voltage Reference\n'4' for Voltage Measurement\n'5' for Thermocouple Measurement");
			while (!Serial.available());
			cmd = Serial.parseInt();
			switch (cmd)
			{
			case 1:
            
				test_intTemp();
				break;
			case 2:
				test_supVoltage();
				break;
			case 3:
				test_extrefVoltage();
				break;
			case 4:
				test_RTD();
				break;
			case 5:
				test_Thermo();
				break;
			}

#endif

		default:
			break;
		}
	}
}

#ifdef ADS1248

/*
Theses are test functions for the ADS1248 system monitor as well
as some examples for 4-wire and thermocouple measurment (Warning these are not well tested).

Run them atleast 3 times to get proper values.

--------------------------------------------------------------

This function gets temperature from the internal diode
*/
void test_intTemp(void) {
	ADS.SetRegisterValue(MUX1, MUXCAL2_TEMP | VREFCON1_ON | REFSELT1_ON);
	Serial.println("Wait for Measurement:");
	delay(1000);
	unsigned long temp_val = ADS.GetConversion();
	Serial.println(temp_val, DEC);
	double temp_voltage = (2.048 / 8388608)*temp_val;	//only 23bit data here.
	Serial.println(temp_voltage, DEC);
	double temp = ((temp_voltage - 0.118) / (405 * 0.000001)) + 25;	//see Datasheet S.30
	Serial.print("Internal Temperature is ");
	Serial.print(temp);
	Serial.println(" Grad Celsius!");
}

/*
This function measures the supplied voltages
*/
void test_supVoltage(void) {
	ADS.SetRegisterValue(MUX1, MUXCAL2_DVDD | VREFCON1_ON | REFSELT1_ON);
	Serial.println("Wait for Measurement:");
	delay(1000);
	unsigned long volt_val = ADS.GetConversion();
	Serial.println(volt_val, DEC);
	double voltage = (2.048 / 16777216)*volt_val;
	voltage *= 4 * 2.048;
	Serial.print("DVDD Voltage: ");
	Serial.print(voltage);
	Serial.println("V");

	ADS.SetRegisterValue(MUX1, MUXCAL2_AVDD | VREFCON1_ON | REFSELT1_ON);
	Serial.println("Wait for Measurement:");
	delay(1000);
	unsigned long volt1_val = ADS.GetConversion();
	Serial.println(volt1_val, DEC);
	double voltage1 = (2.048 / 16777216)*volt1_val;
	voltage1 *= 4 * 2.048;
	Serial.print("AVDD Voltage: ");
	Serial.print(voltage1);
	Serial.println("V");
}

/*
This function measures the voltage of the external voltage reference 1
You have to connect the AIN0 to the specific REF0 or REF1 output
*/
void test_extrefVoltage(void) {
	ADS.SetRegisterValue(MUX1, REFSELT1_ON | VREFCON1_ON | MUXCAL2_REF0);	  //ADS Reference on Intern, Internal Reference on, System Montitor on REF1
	ADS.SetRegisterValue(IDAC0, IMAG2_1500);			 //	IDAC at 1,5mA current
	ADS.SetRegisterValue(IDAC1, I1DIR_AIN0 | I2DIR_OFF);			 // IDAC1 Currentoutput on AIN0, IDAC2 off
	ADS.SetRegisterValue(SYS0, DOR3_5);

	unsigned long volt_val = ADS.GetConversion();
	//Serial.println(volt_val, DEC);
	double voltage = (2.048 / (16777216))*volt_val;
	voltage *= 4 * 2.048;
	Serial.print("External V_Ref: ");
	Serial.print(voltage, DEC);
	Serial.println("V");


}

void test_inputRange(){
	ADS.SetRegisterValue(MUX0,MUX_SN2_AIN0 | MUX_SP2_AIN1);
	ADS.SetRegisterValue(MUX1, VREFCON1_ON | REFSELT1_ON);
	ADS.SetRegisterValue(SYS0, DOR3_20 | PGA2_2);

	uint32_t val = ADS.GetConversion();
	
	if (val >> 23 == 1) {
		val = ~val-  0xFF000000 + 1;
		Serial << millis() <<  "\t-" << (2.048/8388608)*val << "\t-" << val << endl;
		
	}
	else{
		Serial << millis() << "\t" << (2.048/8388608)*val << "\t" << val << endl;
	}
	//Serial.println(val, HEX);


}



/*
Example for a 4-wire measurement (ex PT100 probe)
See ADS1248 application sheet for the setup
*/
double test_RTD(void) {
	double result_temp = 0;
	ADS.SetRegisterValue(MUX0, MUX_SP2_AIN0 | MUX_SN2_AIN1);
	ADS.SetRegisterValue(MUX1, REFSELT1_REF1 | VREFCON1_ON);	  //ADS Reference on REF1, Internal Reference on
	ADS.SetRegisterValue(IDAC0, IMAG2_1000);			 //	IDAC at 1,5mA current
	ADS.SetRegisterValue(IDAC1, I1DIR_IEXT1 |I2DIR_IEXT1);			 // IDAC Currentsink on AIN1
	ADS.SetRegisterValue(SYS0, PGA2_2 | DOR3_20);

	unsigned long val = ADS.GetConversion();
	if (val >> 23 == 1){		
		Serial << "Impossible Result, Error: Negative Value" << endl;
		
	}
	else{
		double result_volt = (1.6408/8388608)*(val/2.0);
		double result_resistance = result_volt/0.002; //
		result_temp = (-R0*A+sqrt(pow(R0*A,2)-4*R0*B*(R0-result_resistance)))/(2*R0*B);
		Serial << millis() << "\t" << _FLOAT(result_volt,4) << "\t" << _FLOAT(result_resistance,4) << "\t" <<  _FLOAT(result_temp,4) << "\t" << val << endl;
	}
	return result_temp;
}

/*
Untested function for Thermocouple measurment
*/
void test_Thermo(void) {
	double cjc_volt=0;
	double tc_volt=0;
	double result_temp=0;
	double result_volt=0;
	unsigned long val=0;

	ADS.SetRegisterValue(MUX0, MUX_SP2_AIN4 | MUX_SN2_AIN5);
	ADS.SetRegisterValue(MUX1, REFSELT1_ON | VREFCON1_ON);	  //ADS Reference on Intern, Internal Reference on
	//ADS.SetRegisterValue(VBIAS, VBIAS_5);
	ADS.SetRegisterValue(IDAC0, IMAG2_1000);			 //	IDAC at 1,5mA current
	ADS.SetRegisterValue(IDAC1, I1DIR_IEXT1 |I2DIR_IEXT1);
	ADS.SetRegisterValue(SYS0, PGA2_32 | DOR3_20);		  

	
	val = ADS.GetConversion();
	if (val >> 23 == 1) {
		val = ~val-  0xFF000000 + 1;
		tc_volt = -(1.813728/7429030)*(val/32.0)*1000;
		Serial << millis() <<  "\t" << _FLOAT(result_volt,4) << "\t-" << val << endl;
		
	}
	else{
		tc_volt = (1.813728/7429030)*(val/32.0)*1000;
		Serial << millis() << "\t" << _FLOAT(result_volt,4) << "\t" << val << endl;
	}

	/*
	double t = test_RTD();
	for(int i=0;i<10;i++){
		cjc_volt += c_i[i]*pow(t,i) + a0*exp(a1*pow((t - a2),2));
	}
	result_volt = result_volt+cjc_volt;

	for(int i=0;i<10;i++){
		result_temp += d_n[i]*pow(result_volt,i);
	}
	Serial << millis() << "\t" << _FLOAT(tc_volt,4) << "\t" << _FLOAT(cjc_volt,4)<<"\t"<< _FLOAT(result_volt,4)<< "\t"<<_FLOAT(result_temp,4)<<"\t"  << val << endl;
	*/
}
#endif
