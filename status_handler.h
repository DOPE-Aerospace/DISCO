#ifndef status_handler
#define status_handler

void blink(unsigned = 500);
void rgb_blink(unsigned, unsigned, unsigned, unsigned = 500);

void rgb_color(unsigned, unsigned, unsigned);
float batteryStatus();

[[noreturn]] void abort_blink(unsigned, unsigned = 3000, unsigned = 500);

#endif
