#pragma once

#include "Camera.h"
class Walker
{
public:
	Walker();
	~Walker();

	enum EWalkingDirection
	{
		up,
		down,
		left,
		right
	};

	void walk(EWalkingDirection direction);
	void decreaseDeltaMovementX();
	int getAnimationSpeed();

private:
	void walkUp();
	void walkDown();
	void walkRight();
	void walkLeft();
	EWalkingDirection decideWalkingDirection();
	Camera *mCamera;

	GLfloat deltaMovementX;
	void timerRight(int value);

	int blockSize = 1;
	int AnimationSteps = 20;
	int AnimationSpeed = 500;
};

