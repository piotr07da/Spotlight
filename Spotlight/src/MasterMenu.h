#ifndef MASTER_MENU_H
#define MASTER_MENU_H

#include "Button.h"
#include "Display.h"
#include "Motor.h"
#include "SpotCollection.h"
#include "SpotPropertiesMenu.h"
#include "SpotPropertyValueMenu.h"

class MasterMenu
{
private:
	Button _ldButton;
	Button _luButton;
	Button _rdButton;
	Button _ruButton;
	Button _rlButton;
	Button _rrButton;
	Display *_display;
	Motor *_motor;
	SpotCollection *_spotCollection;
	SpotPropertiesMenu _spotPropertiesMenu;
	SpotPropertyValueMenu _spotPropertyValueMenu;

public:
	MasterMenu(int ldPin, int luPin, int rdPin, int ruPin, int rlPin, int rrPin, Display *display, Motor *motor, SpotCollection *spotCollection);

	void Setup();
	void Loop();
};

#endif // MASTER_MENU_H