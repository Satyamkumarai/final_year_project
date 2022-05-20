#include <Arduino.h>
const int BAUD = 115200;
const int MAX_BITS = 4;
const int MAX_NUM = 2;

// Arduino to mux select pins map
static const int mux_select_pins [MAX_BITS] = {14, 12 , 13 ,15};



int read_val( int select ){
	if (select > MAX_NUM){
		return BAUD;
	} 

	//Toggle SELECT 
	for (int bit = 0 ; bit < MAX_BITS; bit++){		
		// Serial.print(select );
		// Serial.print(" : ");
		// Serial.print( bit );
		// Serial.print("  ");
		// Serial.print(mux_select_pins[bit]);
		// Serial.print(" >> ");
		// Serial.println(((select>> bit )&1));
		digitalWrite(mux_select_pins[bit],(select >> bit) & 1);

	}
	return analogRead(A0);
	
	
}



int vals[MAX_NUM]  =  {0};


void setup()
{
	Serial.begin(BAUD);
	//init Mux Select Pins 
	for (int i=0;i<MAX_NUM;i++)
		pinMode(mux_select_pins[i],OUTPUT);

}

void loop()
{
	// Serial.print("hello");
	Serial.print(read_val(0));
	Serial.print(",");
	Serial.println(read_val(1));
	delay(100);
}