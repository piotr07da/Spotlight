#ifndef SPOT_PROPERTIES_MENU_H
#define SPOT_PROPERTIES_MENU_H

#include "Button.h"
#include "Display.h"
#include "Light.h"
#include "MenuSpotsNavigator.h"
#include "MasterMenuActivator.h"
#include "SpotPropertyValueMenu.h"
#include "SpotProperty.h"

class SpotsMenu;
class SpotPropertyValueMenu;

class SpotPropertiesMenu
{
private:
	Button *_prevSpotButton;
	Button *_nextSpotButton;
	Button *_prevPropertyButton;
	Button *_nextPropertyButton;
	Button *_escapeButton;
	Button *_enterButton;
	Display *_display;
	Light *_light;
	MenuSpotsNavigator *_spotsNavigator;
	MasterMenuActivator *_masterMenuActivator;
	SpotPropertyValueMenu *_spotPropertyValueMenu;

	bool _isActive;
	SpotProperty _currentProperty;

	void ChangeProperty(int direction);
	void Show();

public:
	SpotPropertiesMenu(Button *prevSpotButton, Button *nextSpotButton, Button *prevPropertyButton, Button *nextPropertyButton, Button *escapeButton, Button *enterButton, Display *display, Light *light, MenuSpotsNavigator *spotsNavigator);

	void Loop();

	void AssignMasterMenuActivator(MasterMenuActivator *masterMenuActivator);
	void AssignSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu);
	bool IsActive();
	SpotProperty GetCurrentProperty();
	void Activate(SpotProperty currentProperty);
	void Deactivate();
};

#endif // SPOT_PROPERTIES_MENU_H