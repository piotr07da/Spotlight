#include "Motor.h"

#include <AccelStepper.h>

Motor::Motor(AccelStepper *stepper)
{
	_stepper = stepper;
	_isRunning = false;
	_hasFinished = false;
}

void Motor::Setup()
{
	_stepper->setMaxSpeed(1000);
}

void Motor::Loop()
{
	bool prevIsRunning = _isRunning;
	_isRunning = _stepper->currentPosition() != _stepper->targetPosition();
	if (prevIsRunning && !_isRunning)
	{
		_hasFinished = true;
	}

	_stepper->runSpeedToPosition();
}

void Motor::MoveTo(int position)
{
	_stepper->moveTo(position);
	_stepper->setSpeed(800);
}

void Motor::MoveTo(int position, int time)
{
}

bool Motor::IsRunning()
{
	return _isRunning;
}

bool Motor::HasFinished(bool clear)
{
	bool hasFinished = _hasFinished;
	if (hasFinished && clear)
	{
		_hasFinished = false;
	}
	return hasFinished;
}
