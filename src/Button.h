#ifndef BUTTON_H
#define BUTTON_H

const int Button_DebounceDelay_SlowButton = 300;
const int Button_DebounceDelay_FastButton = 70;

class Button
{
private:
	int _pin;
	int _debounceDelay;
	int _state;
	int _lastReading;
	long _lastDebounceTime;

	int _tmpFlag0;
	int _tmpFlag1;

public:
	Button(int pin, int debounceDelay);

	void Setup();
	void Loop();

	bool GetState(bool clear);
	void ChangeDebounceDelay(int debounceDelay);
};

#endif // BUTTON_H