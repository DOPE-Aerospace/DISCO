#include <Timer.h>
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include <MPU6050_light.h>
#include <Wire.h>

#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

Timer timer;          //global because needed in loop() and setup()
Logger imu_logger;      //global because needed in loop() and setup()

MPU6050 mpu(Wire);    //Class for the IMU
unsigned long saved_time = 0; //dynamic delay 


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

	timer.start();

	while (!Serial) {
	//this empty while is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
	// why not a delay()? because we cant know exactly how long we have to wait, in this way we dont loose time.
		if(timer.read() > 500) {break;}
	}
	Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));

	//===========
	// IMU INIT
	//===========
	Wire.begin(); //for the IMU
	byte status = mpu.begin();
	Serial.print(F("Inertial Measuring Unit yells code: "));
	Serial.println(String(status));
	if (status != 0) {
		abort_blink(4);
	}
	Serial.println(F("Calculating offsets, DON'T YEEET THE DEVICE!"));
	mpu.calcOffsets();
	Serial.println(F("Done! the offsets aree:"));
	Serial.println("Accel: X:" + String(mpu.getAccXoffset()) + " Y:" + mpu.getAccYoffset() + " Z:" + mpu.getAccZoffset());
	Serial.println("Gyro: X:" + String(mpu.getGyroXoffset()) + " Y:" + mpu.getGyroYoffset() + " Z:" + mpu.getGyroZoffset());
	
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

	imu_logger = Logger(log_folder_name + "/imu", "time, x, y, z");
	
	//===========
	//   Misc
	//===========
	Serial.println("Battery status is: " + String(batteryStatus()) + " volts");
	Serial.println(F("Setup finished."));
	rgb_color(RGB_GREEN); //setup finished
	
}

void loop() {

	mpu.update();
	if((timer.read() - saved_time)>500){ // print data every 500ms
		imu_logger.record_event(String(mpu.getAngleX()) + ", " + mpu.getAngleY(), timer);
		saved_time = timer.read();
	}

}
