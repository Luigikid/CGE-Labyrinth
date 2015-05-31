#pragma once
#include <string>
#include <iostream>
#include <ctime>

class Logger
{
public:
	void Log(std::string Message);
	static Logger* getInstance();

private:
	Logger();
	~Logger();

	Logger *mLogger = NULL;
};
