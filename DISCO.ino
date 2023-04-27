#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include <MPU6050_light.h>
#include <Wire.h>

#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

Timer timer;          //global because needed in loop() and setup()
Logger logger("log", "time, data, content");      //global because needed in loop() and setup()
MPU6050 mpu(Wire);    //Class for the IMU
unsigned long saved_time = 0; //dynamic delay 


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  //For the LEDs


  //=============
  // SERIAL INIT
  //=============
  Serial.begin(9600);  //Bit per second data transfer, for now we send only text which works good with 9600
  while (!Serial) {
    //this empty while is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
    // why not a delay()? becouse we cant know exactly how long we have to wait, in this way we dont loose time.
  }
  Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));

  //===========
  // IMU INIT
  //===========
  Wire.begin(); //for the IMU
  byte status = mpu.begin();
  Serial.println("Inertial Mesuring Unit reports code: " + status);
  if (status != 0) {
    abort_blink(4);
  }
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  mpu.calcOffsets();
  delay(1000);
  Serial.println(F("Done!"));


  //=============
  // Logger Init
  //=============
  //logger =

  //===========
  //   Misc
  //===========
  timer.start();
  Serial.println(F("Setup finished."));
}

void loop() {

  mpu.update();
  if((timer.read() - saved_time)>10){ // print data every 10ms
    Serial.print(mpu.getAngleX());
    Serial.print(", ");
    Serial.println(mpu.getAngleY());
    saved_time = timer.read();  
  }
  
}
