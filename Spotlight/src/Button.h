#ifndef BUTTON_H
#define BUTTON_H

#include <Particle.h>

const int Button_DebounceDelay_SlowButton = 180;
const int Button_DebounceDelay_FastButton = 70;
const int Button_DebounceDelay_SuperFastButton = 25;

class Button
{
private:
	String _name;
	int _pin;
	int _debounceDelay;
	bool _isEnabled;
	bool _isPressed;
	bool _isClicked;
	bool _isReleased;
	bool _lastReading;
	long _lastDebounceTime;

public:
	Button(String name, int pin, int debounceDelay);

	void Setup();
	void Loop();

	void ResetDisabled();
	void ResetEnabled(int debounceDelay);
	void ResetEnabled();
	bool IsPressed();
	bool IsClicked(bool reset = true);
	bool IsReleased(bool reset = true);
	void ChangeDebounceDelay(int debounceDelay);
};

#endif // BUTTON_H