#include "status_handler.h"

void blink(int n_blinks, int time_out, int on_time) {
  // I know this is a stupid function but writing 3 lines of code for every blink annoyed me too much...
  // pinMode(LED_BUILTIN, OUTPUT); needs to be called before using this functio

  #if active_led == true

  for (unsigned int n = 0; n < n_blinks; n++) {
    digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(on_time);                   // wait for time_out milliseconds
    digitalWrite(13, LOW);   // turn the LED off by making the voltage LOW
    delay(time_out);
  }

  #endif
}