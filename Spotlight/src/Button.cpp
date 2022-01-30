#include "Button.h"

#include <Particle.h>

Button::Button(String name, int pin, int debounceDelay)
{
	_name = name;
	_pin = pin;
	_debounceDelay = debounceDelay;

	_isEnabled = true;
	_isPressed = false;
	_isClicked = false;
	_isReleased = false;
	_lastReading = false;
	_lastDebounceTime = -debounceDelay - 1;
}

void Button::Setup()
{
	pinMode(_pin, INPUT_PULLDOWN);
}

void Button::Loop()
{
	if (_isEnabled)
	{
		if (millis() - _lastDebounceTime > (ulong)_debounceDelay)
		{
			bool reading = digitalRead(_pin) == HIGH;

			if (reading != _isClicked)
			{
				if (reading)
				{
					_isClicked = reading;
				}
				_lastDebounceTime = millis();
			}

			if (_isPressed && !reading)
			{
				_isReleased = HIGH;
			}

			_isPressed = reading;
		}
	}
}

void Button::ResetDisabled()
{
	_isEnabled = false;
	_isPressed = false;
	_isClicked = false;
	_isReleased = false;
}

void Button::ResetEnabled()
{
	_isEnabled = true;
	_isPressed = false;
	_isClicked = false;
	_isReleased = false;
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
	// if (isClicked)
	// {
	// 	Particle.publish("diag", _name + " clicked");
	// }
	return isClicked;
}

bool Button::IsReleased(bool reset)
{
	int isReleased = _isReleased;
	if (_isReleased && reset)
	{
		_isReleased = false;
	}
	// if (isReleased)
	// {
	// 	Particle.publish("diag", _name + " released");
	// }
	return isReleased;
}

void Button::ChangeDebounceDelay(int debounceDelay)
{
	_debounceDelay = debounceDelay;
}