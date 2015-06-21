#include "Walker.h"


Walker::Walker()
{
	mCamera	= Camera::getInstance();
}

Walker::~Walker()
{
}

void Walker::walk(EWalkingDirection direction)
{
	switch (direction)
	{
		case EWalkingDirection::up:
			break;

		case EWalkingDirection::down:
			break;

		case EWalkingDirection::left:
			break;

		case EWalkingDirection::right:
			break;
	}
}

void Walker::timerRight(int value)
{
	if (value > 0)
	{
		decreaseDeltaMovementX();

		glutPostRedisplay();
		//glutTimerFunc(1000 / getAnimationSpeed(), &timerRight, --value);	// this is the problem here: can´t pass oop void class
	}
}

void Walker::walkUp()
{
	switch (decideWalkingDirection())
	{
	case EWalkingDirection::up:
		//register glut timer function for translating upwards
		//glutTimerFunc(1000 / 500, &timerUp, 20);
		break;
	case EWalkingDirection::down:
		//register glut timer function for translating downwards
		//glutTimerFunc(1000 / 500, &timerDown, 20);
		break;
	case EWalkingDirection::left:
		//register glut timer function for translating left
		//glutTimerFunc(1000 / 50, &timerLeft, 20);
		break;
	case EWalkingDirection::right:
		//register glut timer function for translating right
		//glutTimerFunc(1000 / 500, &timerRight, 20);	// how to get void pointer here?
		break;
	}

	glutPostRedisplay();

	
}

void Walker::decreaseDeltaMovementX()
{
	deltaMovementX -= 2.0 * blockSize / AnimationSteps;
}

int Walker::getAnimationSpeed()
{
	return AnimationSpeed;
}

Walker::EWalkingDirection Walker::decideWalkingDirection()
{
	if (mCamera->getAngleY() >= 45 && mCamera->getAngleY() < 135)	// right
		return EWalkingDirection::right;
	if (mCamera->getAngleY() >= 135 && mCamera->getAngleY() < 255)	// down
		return EWalkingDirection::down;
	if (mCamera->getAngleY() >= 255 && mCamera->getAngleY() < 315)	// left
		return EWalkingDirection::left;
	if (mCamera->getAngleY() >= 315 || mCamera->getAngleY() < 45)	// up
		return EWalkingDirection::up;
}

void Walker::walkDown()
{

}

void Walker::walkRight()
{

}

void Walker::walkLeft()
{

}