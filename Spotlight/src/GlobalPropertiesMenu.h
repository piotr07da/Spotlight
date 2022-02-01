#ifndef GLOBAL_PROPERTIES_MENU_H
#define GLOBAL_PROPERTIES_MENU_H

#include "Button.h"
#include "Display.h"
#include "MenuSpotsNavigator.h"
#include "MasterMenuActivator.h"
#include "SpotCollection.h"

class MasterMenuActivator;

class GlobalPropertiesMenu
{
private:
	Button *_remSpotButton;
	Button *_addSpotButton;
	Button *_escapeButton;
	Display *_display;
	MenuSpotsNavigator *_spotsNavigator;
	SpotCollection *_spots;
	MasterMenuActivator *_masterMenuActivator;
	bool _isActive;

public:
	GlobalPropertiesMenu(Button *remSpotButton, Button *addSpotButton, Button *escapeButton, Display *display, MenuSpotsNavigator *spotsNavigator, SpotCollection *spots);

	void Loop();

	void AssingMasterMenuActivator(MasterMenuActivator *masterMenuActivator);
	bool IsActive();
	void Activate();
	void Deactivate();
	void AddDefaultSpot();
};

#endif // GLOBAL_PROPERTIES_MENU_H