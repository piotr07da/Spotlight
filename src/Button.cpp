#include "Button.h"

#include <Particle.h>

Button::Button(int pin, int debounceDelay)
{
	_pin = pin;
	_debounceDelay = debounceDelay;

	_state = LOW;
	_lastReading = LOW;
	_lastDebounceTime = -debounceDelay - 1;

	_tmpFlag0 = 0;
	_tmpFlag1 = 0;
}

void Button::Setup()
{
	pinMode(_pin, INPUT);
}

void Button::Loop()
{
	if (millis() - _lastDebounceTime > _debounceDelay)
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

void Button::ChangeDebounceDelay(int debounceDelay)
{
	_debounceDelay = debounceDelay;
}