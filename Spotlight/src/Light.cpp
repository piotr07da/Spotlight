#include "Light.h"

#include <Particle.h>

Light::Light(int pin)
{
	_pin = pin;
}

void Light::Setup()
{
	pinMode(_pin, OUTPUT);
	_maxFrequency = analogWriteMaxFrequency(_pin);
}

void Light::Loop()
{
	switch (_activity)
	{
	case LightActivity::A_0:
		LoopForOff();
		break;

	case LightActivity::A_1:
		LoopForOn();
		break;

	case LightActivity::A_01:
		LoopForOffOn();
		break;

	case LightActivity::A_10:
		LoopForOnOff();
		break;

	case LightActivity::A_010:
		LoopForOffOnOff();
		break;

	case LightActivity::A_101:
		LoopForOnOffOn();
		break;

	case LightActivity::A_Strobe:
		LoopForStrobe();
		break;
	}
}

void Light::SetActivity(LightActivity activity, int duration)
{
	_activity = activity;
	_duration = duration;
	_t0 = millis();
	_value = -1;
}

void Light::LoopForOff()
{
	if (_value == -1)
	{
		_value = 0;
		PwmWriteValue();
	}
}

void Light::LoopForOn()
{
	if (_value == -1)
	{
		_value = 255;
		PwmWriteValue();
	}
}

void Light::LoopForOffOn()
{
	_value = CalculateOffOnValue();
	PwmWriteValue();
}

void Light::LoopForOnOff()
{
	_value = 255 - CalculateOffOnValue();
	PwmWriteValue();
}

void Light::LoopForOffOnOff()
{
	_value = CalculateOffOnOffValue();
	PwmWriteValue();
}

void Light::LoopForOnOffOn()
{
	_value = 255 - CalculateOffOnOffValue();
	PwmWriteValue();
}

void Light::LoopForStrobe()
{
	auto ms = millis();
	auto t = ms - _t0;
	if (t > (ulong)_duration || t % 77 < 38) // 13Hz, 50% pulse width
	{
		_value = 255;
	}
	else
	{
		_value = 0;
	}
	PwmWriteValue();
}

int Light::CalculateOffOnValue()
{
	int value = 255 * (millis() - _t0) / (float)_duration;
	if (value > 255)
	{
		value = 255;
	}
	return value;
}

int Light::CalculateOffOnOffValue()
{
	int t = millis() - _t0;
	int durationDiv2 = _duration / 2;
	int value;
	if (t < durationDiv2)
	{
		value = 255 * t / (float)durationDiv2;
	}
	else
	{
		value = 255 - 255 * (t - durationDiv2) / (float)durationDiv2;
	}
	return value;
}

void Light::PwmWriteValue()
{
	analogWrite(_pin, _value, _maxFrequency / 2);
}