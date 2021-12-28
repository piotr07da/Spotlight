#include "Button.h"

#include "Particle.h"

Button::Button(int pin)
{
	_pin = pin;

	_state = LOW;
	_lastReading = LOW;
	_lastDebounceTime = -Button_DebounceDelay - 1;

	_tmpFlag0 = 0;
	_tmpFlag1 = 0;
}

void Button::Setup()
{
	pinMode(_pin, INPUT);
}

void Button::Loop()
{
	if (millis() - _lastDebounceTime > Button_DebounceDelay)
	{
		int reading = digitalRead(_pin);

		if (reading != _state)
		{
			_lastDebounceTime = millis();
			_state = reading;
		}
	}
}

bool Button::GetState(bool clear)
{
	int state = _state;
	if (state && clear)
	{
		_state = LOW;
	}
	return state;
}