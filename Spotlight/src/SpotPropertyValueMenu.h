#ifndef SPOT_PROPERTY_VALUE_MENU_H
#define SPOT_PROPERTY_VALUE_MENU_H

#include "Button.h"
#include "Display.h"
#include "Motor.h"
#include "MenuSpotsNavigator.h"
#include "SpotPropertiesMenu.h"
#include "SpotProperty.h"

const int SpotPropertyValueMenu_ButtonSyncMotorSpeed = 1000 / Button_DebounceDelay_SuperFastButton;

class SpotPropertiesMenu;

class SpotPropertyValueMenu
{
private:
	static LightActivity _spotActivites[7];
	static int _spotActivitiesCount;
	static LightActivity _travelActivites[7];
	static int _travelActivitiesCount;

	Button *_prevSpotButton;
	Button *_nextSpotButton;
	Button *_decreasePropertyValueButton;
	Button *_increasePropertyValueButton;
	Button *_escapeButton;
	Button *_enterButton;
	Display *_display;
	Motor *_motor;
	MenuSpotsNavigator *_spotsNavigator;
	SpotPropertiesMenu *_spotPropertiesMenu;
	bool _isActive;
	SpotProperty _currentProperty;
	int _valueDelta;
	int _valueChangeCounter;

	void ChangePropertyValue(int direction);
	LightActivity FindActivity(LightActivity *activities, int activitiesCount, LightActivity currentActivity, int indexChangeValue);
	void Show();

public:
	SpotPropertyValueMenu(Button *prevSpotButton, Button *nextSpotButton, Button *decreasePropertyValueButton, Button *increasePropertyValueButton, Button *escapeButton, Button *enterButton, Display *display, Motor *motor, MenuSpotsNavigator *spotsNavigator);

	void Loop();

	void AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu);
	bool IsActive();
	SpotProperty GetCurrentProperty();
	void Activate(SpotProperty currentProperty);
	void Deactivate();
};

#endif // SPOT_PROPERTY_VALUE_MENU_H