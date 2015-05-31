#include "Logger.h"

Logger::Logger()
{
}


Logger::~Logger()
{
}

Logger* Logger::mLogger = NULL;

Logger* Logger::getInstance()
{
	if (mLogger == NULL)
	{
		mLogger = new Logger();
	}
	return mLogger;
}


void Logger::Log(std::string Message)
{
	time_t t = time(0);   // get time now
#pragma warning (disable : 4996)
	struct tm * now = localtime(&t);
#pragma warning(default: 4996)
	

	std::cout << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << " "
		<< Message
		<< std::endl;
	
}
