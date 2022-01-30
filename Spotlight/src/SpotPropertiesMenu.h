#ifndef SPOT_PROPERTIES_MENU_H
#define SPOT_PROPERTIES_MENU_H

#include "Button.h"
#include "Display.h"
#include "SpotPropertyValueMenu.h"
#include "Spot.h"
#include "SpotProperty.h"

class SpotPropertyValueMenu;

class SpotPropertiesMenu
{
private:
	Button *_prevPropertyButton;
	Button *_nextPropertyButton;
	Button *_enterButton;
	SpotPropertyValueMenu *_spotPropertyValueMenu;
	Display *_display;
	bool _isActive;
	Spot *_spot;
	SpotProperty _currentProperty;

public:
	SpotPropertiesMenu(Button *prevPropertyButton, Button *nextPropertyButton, Button *enterButton, Display *display);

	void Loop();

	bool IsActive();
	SpotProperty GetCurrentProperty();
	void AssignSpotPropertyValueMenu(SpotPropertyValueMenu *spotPropertyValueMenu);
	void Activate(Spot *spot, SpotProperty currentProperty);
	void Deactivate();
};

#endif // SPOT_PROPERTIES_MENU_H