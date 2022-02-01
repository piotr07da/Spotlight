#ifndef SPOTS_MENU_H
#define SPOTS_MENU_H

#include "Button.h"
#include "Display.h"
#include "Light.h"
#include "MenuSpotsNavigator.h"
#include "MasterMenuActivator.h"
#include "SpotPropertiesMenu.h"
#include "SpotPropertyValueMenu.h"
#include "SpotCollection.h"

class MasterMenuActivator;
class SpotPropertiesMenu;
class SpotPropertyValueMenu;

class SpotsMenu
{
private:
	Button *_prevSpotButton;
	Button *_nextSpotButton;
	Button *_escapeButton;
	Button *_enterButton;
	Display *_display;
	Light *_light;
	MenuSpotsNavigator *_spotsNavigator;
	MasterMenuActivator *_masterMenuActivator;
	SpotPropertiesMenu *_spotPropertiesMenu;
	SpotPropertyValueMenu *_spotPropertyValueMenu;
	SpotCollection *_spots;
	bool _isActive;

	void Show();

public:
	SpotsMenu(Button *prevSpotButton, Button *nextSpotButton, Button *escapeButton, Button *enterButton, Display *display, Light *light, MenuSpotsNavigator *spotsNavigator);

	void Loop();

	void AssingMasterMenuActivator(MasterMenuActivator *masterMenuActivator);
	void AssingSpotPropertiesMenu(SpotPropertiesMenu *spotPropertiesMenu);
	void AssingSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu);
	void Activate();
	void Deactivate();
};

#endif // SPOTS_MENU_H