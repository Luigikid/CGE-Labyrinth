#include "Camera.h"

Camera* Camera::pCamera = NULL;

Camera::Camera()
{
	viewCoords.x = 0;
	viewCoords.y = 0;
	viewCoords.z = -2;

	beginAngleX = 0;
	beginAngleY = 0;
	angleX = 0;
	angleY = 0;
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


void Camera::calculateViewAngle(int x, int y)
{
	/* calculate new modelview matrix values */
	angleY = angleY + (x - beginAngleX);	// aktuelle x coord - x coordinate von beginn
	angleX = angleX + (y - beginAngleY);	// wenn ich mich nach links rechts drehen will -> drehung um y Achse -> daher sind hier x und y "vertauscht"

	if (angleX > 360.0)
		angleX -= 360.0;
	else if (angleX < -360.0)
		angleX += 360.0;

	if (angleY > 360.0)
		angleY -= 360.0;
	else if (angleY < -360.0)
		angleY += 360.0;

	beginAngleX = x;
	beginAngleY = y;
}

void Camera::moveForward()
{
	viewCoords.x += float(sin(RAD(angleY))) / speedDivisor;
	viewCoords.z -= float(cos(RAD(angleY))) / speedDivisor;
	//viewCoords.y -= float(sin(RAD(angleX))) / speedDivisor;
}

void Camera::moveBack()
{
	viewCoords.x -= float(sin(RAD(angleY))) / speedDivisor;
	viewCoords.z += float(cos(RAD(angleY))) / speedDivisor;
	//viewCoords.y += float(sin(RAD(angleX))) / speedDivisor;
}

void Camera::moveLeft()
{
	viewCoords.x -= float(cos(RAD(angleY))) * 0.2;
	viewCoords.z -= float(sin(RAD(angleY))) * 0.2;
}

void Camera::moveRight()
{
	viewCoords.x += float(cos(RAD(angleY))) * 0.2;
	viewCoords.z += float(sin(RAD(angleY))) * 0.2;
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
