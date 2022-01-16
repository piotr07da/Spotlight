#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Particle.h>

#include "Button.h"
#include "SpotManager.h"
#include "Motor.h"
#include "Light.h"
#include "Event.h"

enum class ControllerMode
{
	GlobalSettings,
	SpotSettings,
	Standby,
};

const int Controller_ButtonSyncMotorSpeed = 1000 / Button_DebounceDelay_SuperFastButton;

class Controller
{
private:
	Button _previousSpotBtn;
	Button _nextSpotBtn;
	Button _previousSettingBtn;
	Button _nextSettingBtn;
	Button _decreaseSettingValueBtn;
	Button _increaseSettingValueBtn;
	SpotManager *_spotManager;
	Motor *_motor;
	Light *_light;
	ControllerMode _mode;
	int _settingValueDelta;
	int _settingValueChangeCounter;

	void ChangeMode(ControllerMode mode);
	void OnModeChanged();
	void OnSettingChanged();
	void ChangeSettingValue(int sign);
	void PositionMotorOnFirstSpot(int speed);
	void PositionMotorOnCurrentSpot(int speed);
	void LightUp();
	void LightDown();
	void ReconfigureButtons();

public:
	Controller(int previousSpotPin, int nextSpotPin, int previousSettingPin, int nextSettingPin, int decreaseSettingValuePin, int increaseSettingValuePin, SpotManager *spotManager, Motor *motor, Light *light);

	Event1<ControllerMode> ModeChanged;
	Event0 StartRequested;
	Event0 StopRequested;

	void Setup();
	void Loop();
};

#endif // CONTROLLER_H