#ifndef MOTOR_H
#define MOTOR_H

const float Motor_MinStepInterval = 0.00125; // 1.25ms

enum class MotorDirection
{
	None,
	CW,
	CCW,
};

class Motor
{
private:
	int _stepPin;
	int _directionPin;
	MotorDirection _direction;
	float _lastStepTime;
	float _stepInterval;
	long _currentPosition;
	long _targetPosition;
	bool _isRunning;
	bool _hasFinished;

	void InitializeMove();

public:
	Motor(int stepPin, int directionPin);

	void Setup();
	void Loop();
	void MoveToInTime(int position, float time);
	void MoveToWithSpeed(int position, int speed);
	bool IsRunning();
	bool HasFinished(bool clear = true);
	long GetCurrentPosition();
	long GetTargetPosition();
};

#endif // MOTOR_H