//Communication Librarys
#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <Wire.h>
#include <SD.h>

//Sensor Librarys
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <TinyGPS++.h>

//Custom Librarys
#include "log_lib.h"
#include "status_handler.h"
#include "config.h"

//Logger Globals
Logger gps_logger;        //global because needed in loop() and setup()
Logger bmp_logger;
MessageLogger info_logger; //global because needed in loop() and setup()

//IMU Globals
//MPU6050 mpu(Wire);    //Class for the IMU

//GPS Globals
bool fixFound = false;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

//Barometer Globals
Adafruit_BMP3XX bmp;
float  starting_pressure = 0;
float  saved_pressure = 0;
bool   falling = false;
double min_press = 0;

//Adding a new timer is simple, add it before the last enum.
enum timer
{
	BARO,
	GPS,
	NUMBER_OF_JOBS //THIS HAS TO BE THE LAST ENUM
};

unsigned long saved_times[NUMBER_OF_JOBS] = {};

template<typename F>
void if_time_expired(timer job, unsigned long delay, F fn)
{
	if (millis() - saved_times[job] > delay)
  {
		fn();
		saved_times[job] = millis();
	}
}

//=============
//	 SETUP
//=============
void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);  //For the LEDs
	digitalWrite(LED_BUILTIN, HIGH);
	pinMode(RGB_PIN_RED, OUTPUT);
	pinMode(RGB_PIN_GREEN, OUTPUT);
	pinMode(RGB_PIN_BLUE, OUTPUT);

	rgb_color(RGB_BLUE); //setup color

	//=============
	// SERIAL INIT
	//=============
	Serial.begin(115200);  //Bit per second data transfer, for now we send only text which works good with 9600

	unsigned long saved_time = millis();
	while (!Serial) 	//this empty while is intentional, sometimes serial connection is not established immediately, but we need it so we wait...
  {
		if (saved_time+1000u < millis()) //after onesecond 
      break; 
	}

	Serial.println(F("Serial started; May your Coffee kick in before the Rocket does..."));

	Wire.begin();
	
	//=============
	// Logger Init
	//=============
	unsigned int n = 0;  //counter for file creation
	bool created = false;
	String log_folder_name;

	while (!created) //if this log file already exists, we create another in the format log_2.txt
  {       
		log_folder_name = "yeet_" + String(n);

		if (!file_exists(log_folder_name)) //if the file is NOT present on the SD
    {  

			Serial.println(log_folder_name);

			if (!make_dir(log_folder_name)) 
      {
				Serial.println(F("ERROR_FILE3: Can't create directory."));
				abort_blink(3);
			}

			created = true;	
			Serial.println("Created new folder as: " + log_folder_name);
		}
    else //else we try again with log_(n+1).txt
			++n;
		
	}

	info_logger = MessageLogger(log_folder_name + "/info", "message");
	bmp_logger = Logger(log_folder_name + "/bmp", "p");
	gps_logger = Logger(log_folder_name + "/gps", "text");
	

	//=====================
	//   Barometer INIT
	//=====================
	if (!bmp.begin_I2C()) 
  {
		Serial.println("Could not find a valid BMP3 sensor, check wiring!");
		while (1);
	}

	// Set up oversampling and filter initialization
  	bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
  	bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
  	bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
  	bmp.setOutputDataRate(BMP3_ODR_25_HZ);

	Serial.println("Doing 10 Settling readings"); //the first readings are always wrong, we hate them

  	for (int i = 0; i < 10; ++i) //10 was chosen at random, i do not know if we need less or more
    { 
		  if (!bmp.performReading()) 
      {
    		Serial.println("Failed to perform reading :(");
    		return;
  		}
	}

	Serial.println("-----Calibrating Height-----");
  	Serial.println(String("Setting ") + (bmp.pressure) + " as ground pressure...");
  	starting_pressure = (bmp.pressure);
	min_press = starting_pressure;

	//=======
	// GPS
	//=======
	Serial1.begin(GPSBaud);


	//===========
	//   Misc
	//===========
	info_logger.record_event("Battery status is: " + String(batteryStatus()) + " volts");
	info_logger.record_event("Setup finished.");
	rgb_color(RGB_GREEN); //setup finished
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	
}

void loop() 
{

	//===========
	// IMU PART
	//===========
	if_time_expired(BARO, BARO_DELAY, []()
  { // print data every 500ms
		bmp.performReading();
		bmp_logger.record_event(String(bmp.pressure));
	});

	while(Serial1.available() > 0) //the gps object has to be fed data constantly to have the right mesurements
	{
		//Serial.write(Serial1.read());
		gps.encode(Serial1.read());
	}
	
	if_time_expired(GPS, GPS_DELAY, []()
	{ // print data every 500ms    		
		if (gps.location.isUpdated())
		{
			if (!fixFound) 
			{
				info_logger.record_event("--Found a Fix!!--");
				info_logger.record_event(String(gps.date.value()) + " " + String(gps.time.value()));
				fixFound = true; 
			}
			digitalWrite(LED_BUILTIN, HIGH);
			String event = String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6);
			gps_logger.record_event(event);
			digitalWrite(LED_BUILTIN, LOW);
		}
	});
}//loop end


