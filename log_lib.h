#ifndef log_lib
#define log_lib

#include <Timer.h>
#include "status_handler.h"
#include "config.h"
#include "file_lib.h"

class Logger {
	
	smart_file file;
	
	String job_name, catagories;
	
	void create_log();
	
public:
	
	Logger() = default;

	Logger(String _job_name, String _catagories);
	
	void record_event(String text, Timer& timer);

};



#endif
