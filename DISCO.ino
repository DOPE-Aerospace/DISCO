#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include <MPU6050_light.h>
#include <Wire.h>

#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

Logger imu_logger;        //global because needed in loop() and setup()
MessageLogger info_logger; //global because needed in loop() and setup()

MPU6050 mpu(Wire);    //Class for the IMU

//Adding a new timer is simple, add it before the last enum.
enum timer {

	imu,

	bmp,

	antenna,

	number_of_jobs //THIS HAS TO BE THE LAST OF THE ENUMS

};

unsigned long saved_times[number_of_jobs] = {};

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);  //For the LEDs
	pinMode(RGB_PIN_RED, OUTPUT);
	pinMode(RGB_PIN_GREEN, OUTPUT);
	pinMode(RGB_PIN_BLUE, OUTPUT);

	rgb_color(RGB_BLUE); //setup color

	//=============
	// SERIAL INIT
	//=============
	Serial.begin(9600);  //Bit per second data transfer, for now we send only text which works good with 9600

	unsigned long saved_time = millis();

	while (!Serial) {
	//this empty while is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
		if(saved_time+1000u < millis() ) {break;} //after onesecond 
	}

	Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));
	
	//=============
	// Logger Init
	//=============
	unsigned int n = 0;  //counter for file creation
	bool created = false;
	String log_folder_name;

	while (!created) {         //if this log file already exists, we create another in the format log_2.txt

		log_folder_name = "yeet_" + String(n);
		if (!file_exists(log_folder_name)) {  //if the file is NOT present on the SD
			Serial.println(log_folder_name);
			if(!make_dir(log_folder_name)) {
				Serial.println(F("ERROR_FILE3: Can't create directory."));
				abort_blink(3);
			}
			created = true;	
			Serial.println("Created new folder as: " + log_folder_name);
		} else {  //else we try again with log_(n+1).txt
			++n;
		}
	}

	info_logger = MessageLogger(log_folder_name + "/info", "message");
	imu_logger = Logger(log_folder_name + "/imu", "x, y, z");
	
	//===========
	// IMU INIT
	//===========
	Wire.begin(); //for the IMU
	byte status = mpu.begin();
	info_logger.record_event("Inertial Measuring Unit yells code: ");
	info_logger.record_event(String(status));
	if (status != 0) {
		abort_blink(4);
	}
	info_logger.record_event("Calculating offsets, DON'T YEEET THE DEVICE!");
	mpu.calcOffsets();
	info_logger.record_event("Done! the offsets aree:");
	info_logger.record_event("Accel: X:" + String(mpu.getAccXoffset()) + " Y:" + mpu.getAccYoffset() + " Z:" + mpu.getAccZoffset());
	info_logger.record_event("Gyro: X:" + String(mpu.getGyroXoffset()) + " Y:" + mpu.getGyroYoffset() + " Z:" + mpu.getGyroZoffset());

	//===========
	//   Misc
	//===========
	info_logger.record_event("Battery status is: " + String(batteryStatus()) + " volts");
	info_logger.record_event("Setup finished.");
	rgb_color(RGB_GREEN); //setup finished
	
}

void loop() {

	//===========
	// IMU PART
	//===========
	if((millis() - saved_times[imu])>IMU_DELAY){ // print data every 500ms
		mpu.update();
		imu_logger.record_event(String(mpu.getAngleX()) + ", " + mpu.getAngleY());
		saved_times[imu] = millis();
	}

}
