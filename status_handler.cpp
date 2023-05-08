#include <Arduino.h>
#include "config.h"
#include "status_handler.h"

void blink(unsigned on_time){
	// pinMode(LED_BUILTIN, OUTPUT); needs to be called before using this function

	#ifdef USE_LED

	//i don't think this if is needed lol
	/*if(digitalRead(LED_BUILTIN) == HIGH) { //if the led is on, stop it
		digitalWrite(LED_BUILTIN, LOW);
	}
	*/

	digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
	delay(on_time);          // wait for time_out milliseconds
	digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
	#endif
}

void rgb_color(unsigned red, unsigned green, unsigned blue) {
	#ifdef USE_LED
	#ifdef USE_RGB
	digitalWrite(RGB_RED, red);
	digitalWrite(RGB_GREEN, green);
	digitalWrite(RGB_BLUE, blue);
	#endif
	#endif
}

void rgb_blink(unsigned red, unsigned green, unsigned blue, unsigned on_time) {

	#ifdef USE_LED 
	#ifdef USE_RGB

	rgb_color(red, green, blue);
	delay(on_time);
	rgb_color(0,0,0);

	#endif
	#endif

}

[[noreturn]] void abort_blink(unsigned n_blinks, unsigned long_time, unsigned short_time){

	while(42){
		blink();
		rgb_blink(255,0,0);
		for(unsigned n = 1; n < n_blinks; ++n){
			delay(short_time);
			blink();
			rgb_blink(255, 0, 0);
		}
		delay(long_time);
	}
}

float batteryStatus() {
	// read the input on analog pin 0:
	int sensorValue = analogRead(ADC_BATTERY);
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 4.3V):
	float voltage = sensorValue * (4.3 / 1023.0);
	return voltage;
}
