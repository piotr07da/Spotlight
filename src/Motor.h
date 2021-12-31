#ifndef MOTOR_H
#define MOTOR_H

#include <AccelStepper.h>

class Motor
{
private:
	AccelStepper *_stepper;

public:
	Motor(AccelStepper *stepper);

	void Setup();
	void Loop();
	bool IsMoving();
	void Move(int position);
	void Move(int position, int time);
};

#endif // MOTOR_H