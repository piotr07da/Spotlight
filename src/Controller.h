#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Particle.h>

#include "Button.h"
#include "Motor.h"
#include "SpotManager.h"
#include "Event.h"

enum class ControllerMode
{
	GlobalSettings,
	SpotSettings,
	Standby,
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
	SpotManager *_spotManager;
	Motor *_motor;
	int _settingValueDelta;
	int _settingValueChangeCounter;

	void ChangeMode(ControllerMode mode);
	void OnModeChanged();
	void OnSettingChanged();
	void ChangeSettingValue(int sign);
	void PositionMotor(int speed);
	void ReconfigureButtons();

public:
	Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, SpotManager *spotManager, Motor *motor);

	Event1<ControllerMode> ModeChanged;
	Event0 StartRequested;
	Event0 StopRequested;

	void Setup();
	void Loop();
};

#endif // CONTROLLER_H