#include "Camera.h"

Camera* Camera::pCamera = NULL;

Camera::Camera()
{
	float x, y;
	mLevel->getStartingPoint(x, y);
	mLogger->LogInfo("Starting x=<" + std::to_string(x) + ">");
	mLogger->LogInfo("Starting z=<" + std::to_string(y) + ">");
	viewCoords.x = (x*2)+2;
	viewCoords.y = 0;
	viewCoords.z = y*2;

	beginAngleX = 360;
	beginAngleY = 180;
	angleX = 0;
	angleY = -180;
	//calculateViewAngle(beginAngleX, beginAngleY);
	//Unfortunately this is not working as the initial cursor position inits the view
}


Camera::~Camera()
{
}

Camera* Camera::getInstance()
{
	if (pCamera == NULL)
	{
		pCamera = new Camera();

	}
	return pCamera;
}

void Camera::modifyKeyStatus(char key, bool status)
{
	key = tolower(key);
	switch (key)
	{
		case 'w':
			forwardKeyPressed = status;
			break;

		case 's':
			backwardKeyPressed = status;
			break;

		case'a':
			leftKeyPressed = status;
			break;

		case'd':
			rightKeyPressed = status;
			break;
	}
}


void Camera::calculateViewAngle(int x, int y)
{
	oldAngleY = angleY;
	/* calculate new modelview matrix values */
	angleY = angleY + (x - beginAngleX);	// aktuelle x coord - x coordinate von beginn
	angleX = angleX + (y - beginAngleY);	// wenn ich mich nach links rechts drehen will -> drehung um y Achse -> daher sind hier x und y "vertauscht"

	//if (angleX > 360.0)
	//	angleX -= 360.0;
	//else if (angleX < -360.0)
	//	angleX += 360.0;
	int AngleXClamp = 30;
	if (angleX >= AngleXClamp)
		angleX = AngleXClamp;
	if (angleX <= -AngleXClamp)
		angleX = -AngleXClamp;

	if (angleY > 360.0)
		angleY -= 360.0;
	else if (angleY < -360.0)
		angleY += 360.0;

	beginAngleX = x;
	beginAngleY = y;

}

void Camera::logViewStats()
{
	mLogger->LogInfo("angleX=<" + std::to_string(angleX) + ">");
	mLogger->LogInfo("angleY=<" + std::to_string(angleY) + ">");
	mLogger->LogInfo("x=<" + std::to_string(beginAngleX) + ">");
	mLogger->LogInfo("y=<" + std::to_string(beginAngleY) + ">");
}

void Camera::moveForward()
{
	GLfloat newX = viewCoords.x + ((float)(sin(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));
	GLfloat newZ = viewCoords.z - ((float)(cos(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveBack()
{
	GLfloat newX = viewCoords.x - ((float)(sin(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));
	GLfloat newZ = viewCoords.z + ((float)(cos(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveLeft()
{
	GLfloat newX = viewCoords.x - ((float)(cos(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));
	GLfloat newZ = viewCoords.z - ((float)(sin(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveRight()
{
	GLfloat newX = viewCoords.x + ((float)(cos(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));
	GLfloat newZ = viewCoords.z + ((float)(sin(RAD(angleY))) / (speedDivisor * getDiff(angleY, oldAngleY)));

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

float Camera::getDiff(float value1, float value2)
{
	mLogger->LogInfo("1");
	float diff = 0;
	if (value1 >= value2)
	{
		mLogger->LogInfo("value1 - value2");
		diff = value1 - value2;
		mLogger->LogInfo("diff=<" + std::to_string(diff) + ">");
	}
	else
	{
		mLogger->LogInfo("value2 - value1");
		diff = value2 - value1;
		mLogger->LogInfo("diff=<" + std::to_string(diff) + ">");
	}

	if (diff == 0)
		diff = 1;
	if (diff > 2)
		diff = 2;

	mLogger->LogInfo("2");
	return diff;
}

void Camera::updateMovement()
{
	if (forwardKeyPressed)
		moveForward();
	if (backwardKeyPressed)
		moveBack();
	if (leftKeyPressed)
		moveLeft();
	if (rightKeyPressed)
		moveRight();
}

GLfloat Camera::getViewCoordX()
{
	return viewCoords.x;
}
GLfloat Camera::getViewCoordY()
{
	return viewCoords.y;
}
GLfloat Camera::getViewCoordZ()
{
	return viewCoords.z;
}
GLfloat Camera::getAngleX()
{
	return angleX;
}
GLfloat Camera::getAngleY()
{
	return angleY;
}

void Camera::setStartingPoint(int x, int y)
{
	viewCoords.x = x;
	viewCoords.y = y;
}