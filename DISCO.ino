#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include <MPU6050_light.h>
#include <Wire.h>

#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

Timer timer;      //global because needed in loop() and setup()
String log_name;  //global because needed in loop() and setup()
MPU6050 mpu(Wire);
unsigned long saved_time = 0;

void setup() {
	
	pinMode(LED_BUILTIN, OUTPUT);  //For the LEDs
	
	//Starting Serial
	Serial.begin(9600);  //Bit per second data transfer, for now we send only text which works good with 9600
	while (!Serial) {
	//this empty While is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
	// why not a delay()? becouse we cant know exactly how long we have to wait, in this way we dont loose time.
	}
	Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));
		
	Wire.begin(); //for the IMU
	byte status = mpu.begin();
	Serial.println("Inertial Mesuring Unit reports code: " + status);
	while(status!=0) {}
	Serial.println(F("Calculating offsets, do not move MPU6050"));
	mpu.calcOffsets();
	Serial.println(F("Done!"));
	
	SD_Connect();                  //connecting to SD card
	log_name = create_log("log");  //Creating
	
	timer.start();
	Serial.println("Setup finished.");
	
}

void loop() {

  mpu.update();
  if((timer.read() - saved_time)>10){ // print data every 10ms
    Serial.print("X : ");
    Serial.print(mpu.getAngleX());
    Serial.print("\tY : ");
    Serial.print(mpu.getAngleY());
    Serial.print("\tZ : ");
    Serial.println(mpu.getAngleZ());
    saved_time = timer.read();  
  }
  
}
