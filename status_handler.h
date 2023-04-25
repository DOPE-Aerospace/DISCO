#ifndef status_handler
#define status_handler

void blink(unsigned = 1000);

[[noreturn]] void abort_blink(unsigned, unsigned = 3000, unsigned = 500);

#endif
