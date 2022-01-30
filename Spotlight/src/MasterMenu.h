#ifndef MASTER_MENU_H
#define MASTER_MENU_H

#include "Button.h"
#include "Display.h"
#include "Motor.h"
#include "Light.h"
#include "Runner.h"
#include "SpotCollection.h"
#include "GlobalPropertiesMenu.h"
#include "SpotPropertiesMenu.h"
#include "SpotPropertyValueMenu.h"
#include "StandbyMenu.h"

class MasterMenu
{
private:
	Button _ldButton;
	Button _luButton;
	Button _rdButton;
	Button _ruButton;
	Button _rlButton;
	Button _rrButton;
	AudioTrigger *_audioTrigger;
	Display *_display;
	Motor *_motor;
	Light *_light;
	Runner *_runner;
	SpotCollection *_spots;
	GlobalPropertiesMenu _globalPropertiesMenu;
	SpotPropertiesMenu _spotPropertiesMenu;
	SpotPropertyValueMenu _spotPropertyValueMenu;
	StandbyMenu _standbyMenu;
	int _currentSpotIndex;

	bool AnySpotMenuIsActive();
	void ActivateSpotMenuForCurrentSpotAndPositionMotor();
	void ActivateSpotMenuForCurrentSpot();
	void DeactivateAllSpotMenus();
	void PositionMotorOnCurrentSpot();
	void PositionMotorOnZero();
	void LightUp();
	void LightDown();

public:
	MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, AudioTrigger *audioTrigger, Display *display, Motor *motor, Light *light, Runner *runner, SpotCollection *spots);

	void Setup();
	void Loop();
};

#endif // MASTER_MENU_H