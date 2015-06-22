#include "Camera.h"

Camera* Camera::pCamera = NULL;

Camera::Camera()
{
	viewCoords.x = 0;
	viewCoords.y = 0;
	viewCoords.z = 0;

	beginAngleX = 360;
	beginAngleY = 180;
	angleX = 0;
	angleY = -180;
	calculateViewAngle(beginAngleX, beginAngleY);
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


void Camera::calculateViewAngle(int x, int y)
{
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
	GLfloat newX = viewCoords.x + (float(sin(RAD(angleY))) / speedDivisor);
	GLfloat newZ = viewCoords.z - (float(cos(RAD(angleY))) / speedDivisor);

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveBack()
{
	GLfloat newX = viewCoords.x - (float(sin(RAD(angleY))) / speedDivisor);
	GLfloat newZ = viewCoords.z + (float(cos(RAD(angleY))) / speedDivisor);

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveLeft()
{
	GLfloat newX = viewCoords.x - (float(cos(RAD(angleY))) * 0.2);
	GLfloat newZ = viewCoords.z - (float(sin(RAD(angleY))) * 0.2);

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
}

void Camera::moveRight()
{
	GLfloat newX = viewCoords.x + (float(cos(RAD(angleY))) * 0.2);
	GLfloat newZ = viewCoords.z + (float(sin(RAD(angleY))) * 0.2);

	if (mLevel->checkAllowed(viewCoords.x, viewCoords.z, newX, newZ))
	{
		viewCoords.x = newX;
		viewCoords.z = newZ;
	}
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
