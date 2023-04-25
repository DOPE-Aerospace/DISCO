#include <Arduino.h>
#include "config.h"
#include "status_handler.h"

void blink(unsigned on_time){
	// pinMode(LED_BUILTIN, OUTPUT); needs to be called before using this function

	#ifdef active_led
		digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
		delay(on_time);          // wait for time_out milliseconds
		digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
	#endif

}

[[noreturn]] void abort_blink(unsigned n_blinks, unsigned long_time, unsigned short_time){

	while(42){
		blink();
		for(unsigned n = 1; n < n_blinks; ++n){
			delay(short_time);
			blink();
		}
		delay(long_time);
	}
  
}
