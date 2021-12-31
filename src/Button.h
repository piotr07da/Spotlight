#ifndef BUTTON_H
#define BUTTON_H

const int Button_DebounceDelay_SlowButton = 180;
const int Button_DebounceDelay_FastButton = 70;
const int Button_DebounceDelay_SuperFastButton = 25;

class Button
{
private:
	int _pin;
	int _debounceDelay;
	int _isPressed;
	int _isClicked;
	int _lastReading;
	long _lastDebounceTime;

	int _tmpFlag0;
	int _tmpFlag1;

public:
	Button(int pin, int debounceDelay);

	void Setup();
	void Loop();

	bool IsPressed();
	bool IsClicked(bool reset = true);
	void ChangeDebounceDelay(int debounceDelay);
};

#endif // BUTTON_H