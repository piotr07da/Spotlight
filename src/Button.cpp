#include "Button.h"

#include <Particle.h>

Button::Button(String name, int pin, int debounceDelay)
{
	_name = name;
	_pin = pin;
	_debounceDelay = debounceDelay;

	_isEnabled = true;
	_isPressed = LOW;
	_isClicked = LOW;
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
	if (_isEnabled)
	{
		if (millis() - _lastDebounceTime > (ulong)_debounceDelay)
		{
			int reading = digitalRead(_pin);
			_isPressed = reading;
			if (reading != _isClicked)
			{
				if (reading)
				{
					_isClicked = reading;
				}
				_lastDebounceTime = millis();
			}
		}
	}
}

void Button::Disable()
{
	_isEnabled = false;
	_isPressed = false;
	_isClicked = false;
}

void Button::Enable()
{
	_isEnabled = true;
}

bool Button::IsPressed()
{
	return _isPressed;
}

bool Button::IsClicked(bool reset)
{
	int isClicked = _isClicked;
	if (_isClicked && reset)
	{
		_isClicked = false;
	}
	if (isClicked)
	{
		Particle.publish("diag", _name + " clicked");
	}
	return isClicked;
}

void Button::ChangeDebounceDelay(int debounceDelay)
{
	_debounceDelay = debounceDelay;
}