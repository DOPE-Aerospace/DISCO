#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>

#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

Timer timer;      //global because needed in loop() and setup()
String log_name;  //global because needed in loop() and setup()

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  //For the LEDs
  //Starting Serial
  
  Serial.begin(9600);  //Bit per second data transfer, for now we send only text which works good with 9600
  while (!Serial) {
    //this empty While is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
    // why not a delay()? becouse we cant know exactly how long we have to wait, in this way we dont loose time.
  }
  Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));


  SD_Connect();                  //connecting to SD card
  log_name = create_log("log");  //Creating


}

void loop() {

  Serial.println("Start logging...");
  record_event("status, Rocket Launched!", log_name, timer);
  delay(5000);
  record_event("gyro, -687.87", log_name, timer);
  delay(3000);
  record_event("acc, 23.6", log_name, timer);
  
}
