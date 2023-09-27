#ifndef FILE_LIB
#define FILE_LIB

#include <SPI.h>  //Serial Peripheral Interface: Used for communication between SD card and Arduino
#include <SD.h>

void initSD(); //called by smart_file constructor if SD_initialized is false

bool file_exists(const char *filepath);
bool file_exists(const String &filepath);

bool make_dir(const char *filepath);
bool make_dir(const String &filepath);

class smart_file : File 
{

	smart_file(File&&);

public:

	smart_file() = default;
	
	smart_file(const char *filename, uint8_t mode = FILE_READ);
	smart_file(const String &filename, uint8_t mode = FILE_READ);

	smart_file(smart_file&&);
	smart_file& operator=(smart_file&&);
	~smart_file();

	//using declarations to export member functions of File

	using File::name;
	using File::available;
	using File::availableForWrite;
	using File::close;
	using File::flush;
	using File::peek;
	using File::position;
	using File::print;
	using File::println;
	using File::seek;
	using File::size;
	using File::read;
	using File::write;
	using File::isDirectory;
	using File::rewindDirectory;
	explicit operator bool();
	smart_file openNextFile(uint8_t mode = O_RDONLY);

};

#endif
