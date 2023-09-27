/* POSSIBLE ERROR MESSAGES

  SD Error -> two rapid blinks and three seconds idle...

  ERROR_SD1: SD Card INIT Failed.

*/

#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>
#include "status_handler.h"
#include "file_lib.h"

bool SD_initialized = false; //false at startup, it becomes true if initSD is called

smart_file::smart_file(File&& file) : File(static_cast<File&&>(file)) {}

smart_file::smart_file(const char *filename, uint8_t mode)
{
	if (!SD_initialized)
  {
		initSD();
	}
	static_cast<File&>(*this) = SD.open(filename, mode);
}

smart_file::smart_file(const String &filename, uint8_t mode)
{
	if (!SD_initialized)
  {
		initSD();
	}
	static_cast<File&>(*this) = SD.open(filename.c_str(), mode);
}

smart_file::smart_file(smart_file&& original_file)
{
	static_cast<File&>(*this) = static_cast<File&>(original_file);
	static_cast<File&>(original_file) = File();
}

smart_file& smart_file::operator=(smart_file&& original_file)
{
	static_cast<File&>(*this) = static_cast<File&>(original_file);
	static_cast<File&>(original_file) = File();
	return *this;
}

smart_file::~smart_file()
{
	if (*this)
  {
		close();
	}
}

smart_file::operator bool()
{
	return static_cast<File>(*this);
}

smart_file smart_file::openNextFile(uint8_t mode)
{
	return static_cast<File>(*this).openNextFile(mode);
}

void initSD()
{

	if (!SD.begin(SDCARD_SS_PIN))
  {  //CS is the Chip Select and could be interpreted like a Context Switch for peripherals, we use a Macro here.
		Serial.println(F("ERROR_SD1: SD Card INIT Failed. is the card inserted?"));
		abort_blink(2);
	}
	Serial.println(F("SD Card Recognized; The Gods are on our side..."));
	SD_initialized = true;

}

bool make_dir(const char *filepath)
{
	if (!SD_initialized)
  {
		initSD();
	}
	return SD.mkdir(filepath);
}

bool make_dir(const String &filepath)
{
	if (!SD_initialized)
  {
		initSD();
	}
	return SD.mkdir(filepath.c_str());
}

bool file_exists(const char *filepath)
{
	if (!SD_initialized)
  {
		initSD();
	}
	return SD.exists(filepath);
}

bool file_exists(const String &filepath)
{
	if (!SD_initialized)
  {
		initSD();
	}
	return SD.exists(filepath.c_str());
}
