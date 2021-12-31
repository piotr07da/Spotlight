#ifndef MOTOR_H
#define MOTOR_H

#include <AccelStepper.h>

class Motor
{
private:
	AccelStepper *_stepper;
	bool _isRunning;
	bool _hasFinished;

public:
	Motor(AccelStepper *stepper);

	void Setup();
	void Loop();
	void MoveTo(int position);
	void MoveToInTime(int position, int time);
	void MoveToWithSpeed(int position, int speed);
	bool IsRunning();
	bool HasFinished(bool clear = true);
};

#endif // MOTOR_H