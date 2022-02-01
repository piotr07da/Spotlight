#ifndef MASTER_MENU_H
#define MASTER_MENU_H

#include "Button.h"
#include "AudioTrigger.h"
#include "Display.h"
#include "Motor.h"
#include "Light.h"
#include "Runner.h"
#include "SpotCollection.h"
#include "MenuSpotsNavigator.h"
#include "MasterMenuActivator.h"
#include "GlobalPropertiesMenu.h"
#include "SpotPropertiesMenu.h"
#include "SpotPropertyValueMenu.h"
#include "StandbyMenu.h"
#include "MasterMenuSubmenu.h"

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
	MenuSpotsNavigator _spotsNavigator;
	bool _isActive;
	bool _welcome;
	MasterMenuActivator _activator;
	GlobalPropertiesMenu _globalPropertiesMenu;
	SpotPropertiesMenu _spotPropertiesMenu;
	SpotPropertyValueMenu _spotPropertyValueMenu;
	StandbyMenu _standbyMenu;
	MasterMenuSubmenu _currentSubmenu;

	void ChangeSubmenu(int direction);

public:
	MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, AudioTrigger *audioTrigger, Display *display, Motor *motor, Light *light, Runner *runner, SpotCollection *spots);

	void Setup();
	void Loop();

	void Activate();
	void Deactivate();
};

#endif // MASTER_MENU_H