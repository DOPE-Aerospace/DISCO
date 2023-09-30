
//============================
//      Configuration
//============================

//----------------
//	ACTIVE LEDS
//----------------

//This define is for general LED usage.
//#define USE_LED

//If you have a RGB led connected this will indicate status signals with color!
//#define USE_RGB

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

//---------------
//  IMU CONFIG
//---------------

//How often the IMU querrys in milliseconds 
#define IMU_DELAY 500

//---------------
//  BARO CONFIG
//---------------
#define BARO_DELAY 500

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

//Falling Treshhold is the max falling distance from the maximum hight after which the parachute is triggered. 
//don't set it too low or the rocket opens the parachute before apogee. Don't set it to high or the parachute opens to late and the parachute breaks. 
#define FALLING_TH 10 //in meters, ABSOLUTLY dont set it under 1m.

//--------------
//	GPS Config
//--------------
#define GPS_DELAY 1000