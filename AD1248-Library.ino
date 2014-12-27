#include <SPI.h>
#include "ads1248.h"

const int  START = 14;
const int  CS = 10;
const int  DRDY = 15;


ads1248 ADS(CS,START, DRDY);  //initialize ADS as object of the ads1248 class



void setup()
{
	

	Serial.begin(9600);
	Serial.println("Serial online");
	
	
	

	

}

void loop(){





	delay(500);


}











