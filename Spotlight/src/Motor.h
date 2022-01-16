#ifndef MOTOR_H
#define MOTOR_H

#include <Particle.h>

enum class MotorDirection
{
	None,
	CW,
	CCW,
};

const int Motor_MaxSpeed = 500;
const double Motor_MinStepInterval = 2000.0; // microseconds

class Motor
{
private:
	int _stepPin;
	int _directionPin;
	MotorDirection _direction;
	double _lastStepTime;
	double _stepInterval;
	long _currentPosition;
	long _targetPosition;
	bool _isRunning;
	bool _hasFinished;

	void InitializeMove();

public:
	Motor(int stepPin, int directionPin);

	void Setup();
	void Loop();
	void MoveToInTime(int position, int time);
	void MoveToWithSpeed(int position, int speed);
	bool IsRunning();
	bool HasFinished(bool clear = true);
	long GetCurrentPosition();
	long GetTargetPosition();
};

#endif // MOTOR_H