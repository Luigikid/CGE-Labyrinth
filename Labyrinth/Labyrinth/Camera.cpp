#include "Camera.h"

Camera* Camera::pCamera = NULL;

Camera::Camera()
{
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


	viewCoords.x = -sinf(RAD(angleY));
	viewCoords.y = sinf(RAD(angleX));
	viewCoords.z = cosf(RAD(angleY));
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