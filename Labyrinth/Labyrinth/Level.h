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


	bool checkAllowed(GLfloat OldX, GLfloat OldZ, GLfloat &NewX, GLfloat &NewZ);
	
	void renderLevel();
	void getStartingPoint(float &x, float &y);
	void setFloorTextureId(GLuint id);
	void setWallTextureId(GLuint id);
	bool checkLevelFinished();

private:
	Logger *mLogger = Logger::getInstance();
	bool levelFinished = false;
	
	Level();
	static Level* m_pLevel;
	float mBlockSize = 1.0f;
	std::vector<std::vector<char>> mLevel;
	bool isValidFieldType(char fieldType);
	void drawCube();
	void drawFloor();
	void drawGoal();
	void loadLevelFromFile(std::string fileName);
	bool isFree(int x, int y);
	void getLevelFieldCoords(float x, float z, int &xInt, int &zInt);
	void getWorldSpaceCoordsFromLevelCoords(float levelX, float levelZ, float &worldX, float &worldZ);
	char getFieldTypeForCoords(int x, int z);
	
	void drawText(int x, int y, int font, std::string Text);

	GLuint floorTextureId = 0;
	GLuint wallTextureId = 0;
	GLfloat mCollissionOffset = 1.0;

};

