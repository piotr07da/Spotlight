#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Particle.h>

#include "Button.h"
#include "Display.h"
#include "SpotManager.h"

enum ControllerMode
{
	GlobalSettings,
	SpotSettings,
};

class Controller
{
private:
	ControllerMode _mode;
	Button *_previousSpotBtn;
	Button *_nextSpotBtn;
	Button *_previousSettingBtn;
	Button *_nextSettingBtn;
	Button *_decreaseSettingValueBtn;
	Button *_increaseSettingValueBtn;
	int _settingValueSensitivityPin;
	Display *_display;
	SpotManager *_spotManager;
	int _settingValueDelta;
	int _settingValueChangeCounter;

	void ChangeSettingValue(int sign);

public:
	Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, int settingValueSensitivityPin, Display *display, SpotManager *spotManager);

	void Setup();
	void Loop();
};

#endif // CONTROLLER_H