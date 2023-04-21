#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include "status_handler.h"
#include "config.h"

#ifndef log_lib
#define log_lib

void SD_Connect();
void record_event(String, String&, Timer&);
String create_log(String, String = "Time, Event, Data");

#endif