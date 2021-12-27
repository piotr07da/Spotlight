#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "Button.h"
#include "SpotManager.h"

enum InputControllerMode
{
	GlobalSettings,
	SpotSettings,
};

class InputController
{
private:
	InputControllerMode _mode;
	Button _previousSpotBtn;
	Button _nextSpotBtn;
	Button _previousSettingBtn;
	Button _nextSettingBtn;
	Button _decreaseSettingValueBtn;
	Button _increaseSettingValueBtn;
	int _settingValueSensitivityPin;
	SpotManager *_spotManager;

public:
	InputController(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, int settingValueSensitivityPin, SpotManager *spotManager);
	virtual ~InputController();

	void Setup();
	void Loop();
};

#endif // INPUT_CONTROLLER_H