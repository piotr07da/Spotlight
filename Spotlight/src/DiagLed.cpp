#include "DiagLed.h"

bool DiagLed::_on;

bool DiagLed::Toggle()
{
	_on = !_on;
	digitalWrite(D7, _on);
	return _on;
}