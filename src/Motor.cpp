#include "Motor.h"

#include <AccelStepper.h>

Motor::Motor(AccelStepper *stepper)
{
	_stepper = stepper;
}

void Motor::Setup()
{
	_stepper->setMaxSpeed(1000);
	_stepper->setSpeed(200);
	_stepper->moveTo(200);
}

void Motor::Loop()
{
	_stepper->runSpeedToPosition();
}

bool Motor::IsMoving()
{
	return _stepper->currentPosition() != _stepper->targetPosition();
}

void Motor::Move(int position)
{
	_stepper->moveTo(position);
}

void Motor::Move(int position, int time)
{
}
