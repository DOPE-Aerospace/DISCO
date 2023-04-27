#ifndef log_lib
#define log_lib

#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include "status_handler.h"
#include "config.h"

class SDcard {
		
	static bool initialized;
		
public:
		
	SDcard();
		
};

class Logger {
	
	SDcard sd;
	
	String job_name, catagories, log_file_name;
	void create_log();
	
public:
	
	Logger(String _job_name, String _catagories);
	
	void record_event(String text, Timer& timer);

};



#endif
