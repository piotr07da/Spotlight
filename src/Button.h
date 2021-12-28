#ifndef BUTTON_H
#define BUTTON_H

const int Button_DebounceDelay = 55;

class Button
{
private:
	int _pin;
	int _state;
	int _lastReading;
	long _lastDebounceTime;

	int _tmpFlag0;
	int _tmpFlag1;

public:
	Button(int pin);

	void Setup();
	void Loop();

	bool GetState(bool clear);
};

#endif // BUTTON_H