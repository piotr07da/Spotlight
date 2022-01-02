#include "Motor.h"

#include <Particle.h>

Motor::Motor(int stepPin, int directionPin)
{
	_stepPin = stepPin;
	_directionPin = directionPin;
	_isRunning = false;
	_hasFinished = false;
}

void Motor::Setup()
{
	pinMode(_stepPin, OUTPUT);
	pinMode(_directionPin, OUTPUT);
}

void Motor::Loop()
{
	if (!_isRunning)
	{
		return;
	}

	if (_lastStepTime < 0)
	{
		_lastStepTime = micros();
	}

	auto t = micros();
	if ((float)t - _lastStepTime >= _stepInterval)
	{
		switch (_direction)
		{
		case MotorDirection::CW:
			_currentPosition += 1;
			break;

		case MotorDirection::CCW:
			_currentPosition -= 1;
			break;
		}

		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(1);
		digitalWrite(_stepPin, LOW);

		_lastStepTime += _stepInterval;
	}

	if (_currentPosition == _targetPosition)
	{
		_isRunning = false;
		_hasFinished = true;
	}
}

void Motor::MoveToInTime(int position, float time)
{
	_targetPosition = position;
	if (_targetPosition != _currentPosition)
	{
		auto distance = abs(_targetPosition - _currentPosition);
		_stepInterval = 1000000.0f * time / distance;
	}

	InitializeMove();
}

void Motor::MoveToWithSpeed(int position, int speed)
{
	_targetPosition = position;
	_stepInterval = 1000000.0f / speed;

	InitializeMove();
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

long Motor::GetCurrentPosition()
{
	return _currentPosition;
}

long Motor::GetTargetPosition()
{
	return _targetPosition;
}

void Motor::InitializeMove()
{
	if (_targetPosition != _currentPosition)
	{
		if (_targetPosition > _currentPosition)
		{
			_direction = MotorDirection::CW;
			digitalWrite(_directionPin, HIGH);
		}

		if (_targetPosition < _currentPosition)
		{
			_direction = MotorDirection::CCW;
			digitalWrite(_directionPin, LOW);
		}

		digitalWrite(_stepPin, LOW);

		_isRunning = true;
		_lastStepTime = -1;
	}
	else
	{
		_direction = MotorDirection::None;
		_stepInterval = 0;

		if (_isRunning)
		{
			_isRunning = false;
			_hasFinished = true;
		}
	}
}