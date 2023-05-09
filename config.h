
//============================
//      Configuration
//============================

//----------------
//	ACTIVE LEDS
//----------------

//This define is for general LED usage.
#define USE_LED

//If you have a RGB led connected this will indicate status signals with color!
#define USE_RGB

//RGB pinout options.
#define RGB_PIN_RED 0
#define RGB_PIN_GREEN 1
#define RGB_PIN_BLUE 2

#define RGB_RED 100, 0, 0
#define RGB_GREEN 0, 100, 0
#define RGB_BLUE 0, 0, 100

//-----------------
// ACTIVE LOGGING
//-----------------

//This define activates the logging functions, it works only if a SD card is inserted.
#define DO_LOGGING

//This define pipes the SD_CARD input to serial.
//#define FAKE_SD

//----------------------------
