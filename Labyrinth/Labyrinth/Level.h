#pragma once

#include "Logger.h"
#include <glut.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>

class Level
{
public:
	static Level * getInstance();
	~Level();


	bool checkAllowed(GLfloat OldX, GLfloat OldZ, GLfloat NewX, GLfloat NewZ);
	void loadLevelFromFile(std::string fileName);
	void renderLevel();
private:
	Logger *mLogger = Logger::getInstance();
	Level();
	static Level* m_pLevel;
	float mBlockSize = 1.0f;
	std::vector<std::vector<char>> mLevel;
	bool isValidFieldType(char fieldType);
	void drawCube();

};

