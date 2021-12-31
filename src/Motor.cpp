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

void Motor::MoveToInTime(int position, int time)
{
	_stepper->moveTo(position);
	long distance = position - _stepper->currentPosition();
	if (distance < 0)
	{
		distance *= -1;
	}
	float speed = distance / (float)time;
	_stepper->setSpeed(speed);
}

void Motor::MoveToWithSpeed(int position, int speed)
{
	_stepper->moveTo(position);
	_stepper->setSpeed(speed);
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
