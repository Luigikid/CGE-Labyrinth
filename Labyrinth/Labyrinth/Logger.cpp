#include "Logger.h"


Logger* Logger::m_pLogger = NULL;

Logger::Logger()
{
}

Logger* Logger::getInstance()
{
	if (m_pLogger == NULL)
	{
		m_pLogger = new Logger();
	}
	return m_pLogger;
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
