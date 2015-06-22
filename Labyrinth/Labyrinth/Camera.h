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
	GLfloat getViewCoordZ();
	GLfloat getAngleX();
	GLfloat getAngleY();

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();

	void logViewStats();

private:
	
	GLfloat angleX;
	GLfloat angleY;
	GLfloat beginAngleX;
	GLfloat beginAngleY;

	GLfloat deltaMovement;
	GLfloat deltaAngle;

	struct viewCoords
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};
	viewCoords viewCoords;

	int speedDivisor = 5;
	static Camera *pCamera;
	Camera();

	Logger* mLogger = Logger::getInstance();
	Level* mLevel = Level::getInstance();
};

