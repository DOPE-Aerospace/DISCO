// Created by David Hermes as possible Logging Library for D.O.P.E. Rocketry Club Rockets.
// it uses as Apr 18 2023 20648 byte
// As file format I'm using CSV (RFC 4180) for now, it's easy to implement and stupidly easy to transform in any data format we want.
// Conversion can be accomplished by a python script or directly with Elasticsearch.

/* POSSIBLE ERROR MESSAGES

  File Error -> three rapid blinks and three seconds idle...
  
  ERROR_FILE1: Can't open file.
  ERROR_FILE2: File doesn't exist.

*/

#include "log_lib.h"

Logger::Logger(const String& _job_name, const String& _catagories) 
{
	#ifdef DO_LOGGING
	job_name = _job_name;
	catagories = _catagories;
	create_log();
	#endif
}

void Logger::create_log() 
{
	#ifdef DO_LOGGING
	#ifdef FAKE_SD
	//FAKE_SD makes log print to Serial

	Serial.println(catagories);

	#else

	//create_log() requires a job name, which it uses for creating the csv file with appropriate Initialization.
	//it returns the file name. in this way we don't need global variables which nobody likes.

	unsigned int n = 0;  //counter for file creation
	bool created = false;

	while (!created) 
  {         //if this log file already exists, we create another in the format log_2.txt

		String log_file_name = job_name + "_" + n + ".csv";
		if (!file_exists(log_file_name)) 
    {  //if the file is NOT present on the SD
			file = smart_file(log_file_name, FILE_WRITE);
			if (!file) 
      {
				Serial.println(F("ERROR_FILE1: Can't open file. Probably a file didn't close properly"));
				abort_blink(3);
			}

			Serial.println("Created file at: " + log_file_name);
			created = true;
			
		}
    else 
    {  //else we try again with log_(n+1).txt
			++n;
		}

	}

	file.println("time, " + catagories);

	#endif
	#endif
}


void Logger::record_event(const String& text) 
{
	#ifdef DO_LOGGING
	//this is the logger function, after creating a log file we can use this to append new log entries onto the file.
	String text_to_write = String(millis()) + ", " + text;
	#ifdef FAKE_SD
	Serial.println(text_to_write);
	#else
	file.println(text_to_write);
	file.flush();
	#endif
	#endif
}

void MessageLogger::record_event(const String& text) 
{
	Serial.println(text);
	Logger::record_event("\"" + text + "\"");
}
