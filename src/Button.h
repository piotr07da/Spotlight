#ifndef BUTTON_H
#define BUTTON_H

const int Button_DebounceDelay = 50;

class Button
{
private:
	int _pin;
	int _state;
	int _lastReading;
	long _lastDebounceTime;

public:
	Button();
	Button(int pin);

	void Setup();
	void Loop();

	bool GetState();
};

#endif BUTTON_H