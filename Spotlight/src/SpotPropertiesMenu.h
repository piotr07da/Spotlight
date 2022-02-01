#ifndef SPOT_PROPERTIES_MENU_H
#define SPOT_PROPERTIES_MENU_H

#include "Button.h"
#include "Display.h"
#include "MenuSpotsNavigator.h"
#include "SpotsMenu.h"
#include "SpotPropertyValueMenu.h"
#include "Spot.h"
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
	MenuSpotsNavigator *_spotsNavigator;
	SpotsMenu *_spotsMenu;
	SpotPropertyValueMenu *_spotPropertyValueMenu;

	bool _isActive;
	SpotProperty _currentProperty;

	void ChangeProperty(int direction);
	void Show();

public:
	SpotPropertiesMenu(Button *prevSpotButton, Button *nextSpotButton, Button *prevPropertyButton, Button *nextPropertyButton, Button *escapeButton, Button *enterButton, Display *display, MenuSpotsNavigator *spotsNavigator);

	void Loop();

	void AssignSpotsMenu(SpotsMenu *spotsMenu);
	void AssignSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu);
	bool IsActive();
	SpotProperty GetCurrentProperty();
	void Activate(SpotProperty currentProperty);
	void Deactivate();
};

#endif // SPOT_PROPERTIES_MENU_H