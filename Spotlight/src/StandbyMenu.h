#ifndef STANDBY_MENU_H
#define STANDBY_MENU_H

#include "Button.h"
#include "AudioTrigger.h"
#include "Display.h"
#include "Motor.h"
#include "Light.h"
#include "Runner.h"
#include "SpotCollection.h"

class StandbyMenu
{
private:
	Button *_stopButton;
	Button *_startButton;
	AudioTrigger *_audioTrigger;
	Display *_display;
	Motor *_motor;
	Light *_light;
	Runner *_runner;
	SpotCollection *_spots;
	bool _isActive;

public:
	StandbyMenu(Button *stopButton, Button *startButton, AudioTrigger *audioTrigger, Display *display, Motor *motor, Light *light, Runner *runner, SpotCollection *spots);

	void Loop();

	bool IsActive();
	void Activate();
	void Deactivate();
	void Start();
	void Stop();
	void Standby();
};

#endif // STANDBY_MENU_H