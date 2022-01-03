#include "Motor.h"

#include <Particle.h>
#include "DiagLed.h"

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

	auto t = (float)micros();

	if (t - _lastStepTime >= _stepInterval)
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
		if (t - _lastStepTime >= _stepInterval)
		{
			_lastStepTime = t;
		}
	}

	if (_currentPosition == _targetPosition)
	{
		_isRunning = false;
		_hasFinished = true;

		Particle.publish("diag-MoveEnd", "cp:" + String(_currentPosition) + " t:" + String(millis() - _diagT0));
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

	InitializeMove("MoveToInTime");
}

void Motor::MoveToWithSpeed(int position, int speed, String diag)
{
	_targetPosition = position;
	_stepInterval = 1000000.0f / speed;

	InitializeMove(diag);
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

void Motor::InitializeMove(String diag)
{
	if (_targetPosition != _currentPosition)
	{
		if (_stepInterval < Motor_MinStepInterval)
		{
			_stepInterval = Motor_MinStepInterval;
		}

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

	auto t = DiagLed::Toggle();
	_diagT0 = millis();
	Particle.publish("diag-MoveInit", String(_currentPosition) + "->" + String(_targetPosition) + " si:" + String(_stepInterval) + " t:" + String(t) + " diag:" + diag);
}