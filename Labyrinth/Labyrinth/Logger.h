#pragma once
#include <string>
#include <iostream>
#include <ctime>

#define LOGGER Logger::getLogger()

class Logger
{
public:
	void Log(std::string Message);
	static Logger* getInstance();

private:
	Logger();
	static Logger* m_pLogger;
};