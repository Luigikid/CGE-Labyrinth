#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <Windows.h>
#include <sstream>
#include <fstream>

#define LOGGER Logger::getLogger()

class Logger
{
public:
	
	static Logger* getInstance();
	Logger::~Logger();
	enum Level 
	{ 
		Info, 
		Warning, 
		Error 
	};
	void Log(std::string Message, Logger::Level);
	void LogInfo(std::string Message);
	void LogWarning(std::string Message);
	void LogError(std::string Message);

private:
	Logger();
	static Logger* m_pLogger;
	std::fstream mLogFile;

};