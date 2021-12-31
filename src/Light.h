#ifndef LIGHT_H
#define LIGHT_H

#include "LightActivity.h"

class Light
{
private:
	int _pin;
	LightActivity _activity;
	int _duration;
	int _t0;
	int _value;
	int _maxFrequency;
	void LoopForOff();
	void LoopForOn();
	void LoopForOffOn();
	void LoopForOnOff();
	void LoopForOffOnOff();
	void LoopForOnOffOn();
	void LoopForStrobe();
	int CalculateOffOnValue();
	int CalculateOffOnOffValue();
	void PwmWriteValue();

public:
	Light(int pin);

	void Setup();
	void Loop();
	void SetActivity(LightActivity activity, int duration);
};

#endif // LIGHT_H
