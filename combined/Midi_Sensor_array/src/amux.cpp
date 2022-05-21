#include <Arduino.h>
#include "amux.h"
extern int mux_select_pins[];
int read_val( int select ){
	if (select > MAX_NUM){
		return BAUD;
	} 

	//Toggle SELECT 
	for (int bit = 0 ; bit < MAX_BITS; bit++){		
		digitalWrite(mux_select_pins[bit],(select >> bit) & 1);

	}
	return analogRead(A0);
	
}