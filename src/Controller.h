#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Particle.h>

#include "Button.h"
#include "Motor.h"
#include "Display.h"
#include "SpotManager.h"

enum ControllerMode
{
	GlobalSettings,
	SpotSettings,
};

const int Controller_MaxMotorSpeed = 800;
const int Controller_ButtonSyncMotorSpeed = 1000 / Button_DebounceDelay_SuperFastButton;

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
	Motor *_motor;
	Display *_display;
	SpotManager *_spotManager;
	int _settingValueDelta;
	int _settingValueChangeCounter;

	void ChangeMode(ControllerMode mode);
	void OnModeChanged();
	void OnSettingChanged();
	void ChangeSettingValue(int sign);
	void OnSettingValueChanged();
	void PositionMotor(int speed);
	void RefreshDisplay();
	void ReconfigureButtons();

public:
	Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, Motor *motor, Display *display, SpotManager *spotManager);

	void Setup();
	void Loop();
};

#endif // CONTROLLER_H