#ifndef SPOT_PROPERTY_VALUE_MENU_H
#define SPOT_PROPERTY_VALUE_MENU_H

#include "Button.h"
#include "Display.h"
#include "Motor.h"
#include "SpotPropertiesMenu.h"
#include "Spot.h"
#include "SpotSetting.h"

const int SpotPropertyValueMenu_ButtonSyncMotorSpeed = 1000 / Button_DebounceDelay_SuperFastButton;

class SpotPropertiesMenu;

class SpotPropertyValueMenu
{
private:
	static LightActivity _spotActivites[7];
	static int _spotActivitiesCount;
	static LightActivity _travelActivites[7];
	static int _travelActivitiesCount;

	Button *_decreasePropertyValueButton;
	Button *_increasePropertyValueButton;
	Button *_escapeButton;
	Button *_enterButton;
	Display *_display;
	Motor *_motor;
	SpotPropertiesMenu *_spotPropertiesMenu;
	bool _isActive;
	Spot *_spot;
	SpotSetting _currentProperty;
	int _valueDelta;
	int _valueChangeCounter;

	void ChangePropertyValue(int sign);
	LightActivity FindActivity(LightActivity *activities, int activitiesCount, LightActivity currentActivity, int indexChangeValue);

public:
	SpotPropertyValueMenu(Button *decreasePropertyValueButton, Button *increasePropertyValueButton, Button *escapeButton, Button *enterButton, Display *display, Motor *motor);

	void Loop();

	void AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu);
	void Activate(Spot *spot, SpotSetting currentProperty);
	void Deactivate();
};

#endif // SPOT_PROPERTY_VALUE_MENU_H