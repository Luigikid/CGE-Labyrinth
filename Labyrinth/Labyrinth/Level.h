#pragma once

#include "Logger.h"
#include <glut.h>
class Level
{
public:
	static Level * getInstance();
	~Level();


	bool checkAllowed(GLfloat OldX, GLfloat OldZ, GLfloat NewX, GLfloat NewZ);
private:
	Logger *mLogger = Logger::getInstance();
	Level();
	static Level* m_pLevel;
};

