// Created by David Hermes as possible Logging Library for D.O.P.E. Rocketry Club Rockets.
// it uses as Apr 18 2023 20648 byte
// As file format I'm using CSV (RFC 4180) for now, it's easy to implement and stupidly easy to transform in any data format we want.
// Conversion can be accomplished by a python script or directly with Elasticsearch.

/* POSSIBLE ERROR MESSAGES

  SD Error -> two rapid blinks and three seconds idle...

  ERROR_SD1: SD Card INIT Failed.
  
  File Error -> three rapid blinks and three seconds idle...
  
  ERROR_FILE1: Can't open file.
  ERROR_FILE2: File doesn't exist.
*/

#include "log_lib.h"

Logger::Logger(String _job_name, String _catagories) {
  #ifdef do_logging

  job_name = _job_name;
  catagories = _catagories;

  if (!SD.begin(SDCARD_SS_PIN)) {  //CS is the Chip Select and could be interpreted like a Context Switch for peripherals, we use a Macro here.
    Serial.println(F("ERROR_SD1: SD Card INIT Failed. is the card inserted? (logger.ino under SD_Connect())"));
    abort_blink(2);
  }
  Serial.println(F("SD Card Recognized; The Gods are on our side..."));

  create_log();

  #endif
}

void Logger::create_log() {
  #ifdef do_logging
  //create_log() requires a job name, which it uses for creating the csv file with appropriate Initialization.
  //it returns the file name. in this way we don't need global variables which nobody likes.

  unsigned int n = 0;  //counter for file creation
  bool created = false;

  while (!created) {         //if this log file already exists, we create another in the format log_2.txt

    log_file_name = job_name + "_" + n + ".csv";
    if (!SD.exists(log_file_name)) {  //if the file is NOT present on the SD

      File tmp_file = SD.open(log_file_name, FILE_WRITE);
      
      if (!tmp_file) {
        Serial.println(F("ERROR_FILE1: Can't open file. Probably a file didn't close properly (logger.ino under create_file())"));
        abort_blink(3);
      }

      tmp_file.println(catagories);
      tmp_file.close();
      created = true;
    } else {  //else we try again with log_(n+1).txt

      n++;
    }
  }
  #endif
}


void Logger::record_event(String text, Timer& timer) {
  #ifdef do_logging
  //this is the logger function, after creating a log file we can use this to append new log entries onto the file.

  if (!SD.exists(log_file_name)) {
    Serial.println(F("ERROR_FILE2: File doesn't exist. Probably a file didn't close properly (logger.ino under record_event())"));
    abort_blink(3);
  }
  File file = SD.open(log_file_name, FILE_WRITE);
  if (!file) {
    Serial.println(F("ERROR_FILE1: Can't open file. Probably a file didn't close properly (logger.ino under record_event())"));
    abort_blink(3);
  }
  String text_to_write = timer.read() + ", " + text;
  file.println(text_to_write);
  file.close();
  #endif
}