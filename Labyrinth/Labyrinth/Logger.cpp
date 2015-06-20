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

Logger::~Logger()
{
	mLogFile.close();
}

void Logger::Log(std::string Message, Logger::Level Level = Level::Info)
{
	time_t t = time(0);   // get time now
#pragma warning (disable : 4996)
	struct tm * now = localtime(&t);
#pragma warning(default: 4996)

	//prefix date and time to message
	std::stringstream ss;
	ss << now->tm_year + 1900 << '-' << now->tm_mon + 1 << '-' << now->tm_mday << " "
		<< now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " " 
		<< Message << std::endl;
	std::string LogMessage = ss.str();

	// log message to console 
	std::cout << LogMessage;

	//log message to log File
	mLogFile.open("Labyrinth.log", std::fstream::out | std::fstream::app);		// TODO: open this in constructor
	mLogFile << LogMessage;
	mLogFile.close();

	// log message to output Window from Visual Studio
	std::wstring stemp = std::wstring(LogMessage.begin(), LogMessage.end());
	OutputDebugString(stemp.c_str());
	
}

void Logger::LogInfo(std::string Message)
{
	Log(Message, Logger::Level::Info);
}

void Logger::LogWarning(std::string Message)
{
	Log(Message, Level::Warning);
}

void Logger::LogError(std::string Message)
{
	Log(Message, Level::Error);
}