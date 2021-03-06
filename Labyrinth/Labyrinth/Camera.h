#pragma once

#include <stdlib.h>
#include <glut.h>
#include <vector>
#include "Logger.h"
#include "Level.h"

/* some math.h files don't define pi... */
#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)	// TODO: Create method in Main Class

class Camera
{
public:

	static Camera* getInstance();
	Camera::~Camera();

	void calculateViewAngle(int x, int y);
	GLfloat getViewCoordX();
	GLfloat getViewCoordY();
	void modifyViewCoordY(GLfloat value);
	GLfloat getViewCoordZ();
	GLfloat getAngleX();
	GLfloat getAngleY();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();

	void modifyKeyStatus(char key, bool pressed);
	void updateMovement();

	void logViewStats();
	void setStartingPoint(int x, int y);

private:
	
	GLfloat angleX;
	GLfloat angleY;

	GLfloat beginAngleX;
	GLfloat beginAngleY;

	struct viewCoords
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};
	viewCoords viewCoords;

	int speedDivisor = 10;
	GLfloat maxSpeedDiff = 0.15;	// if this is 0.1 we get a lot of gimble locks
	static Camera *pCamera;
	Camera();

	Logger* mLogger = Logger::getInstance();
	Level* mLevel = Level::getInstance();

	bool forwardKeyPressed = false;
	bool backwardKeyPressed = false;
	bool leftKeyPressed = false;
	bool rightKeyPressed = false;

	float getDiff(float value1, float value2);
};

