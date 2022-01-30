#ifndef GLOBAL_PROPERTIES_MENU_H
#define GLOBAL_PROPERTIES_MENU_H

#include "Button.h"
#include "Display.h"
#include "SpotCollection.h"

class GlobalPropertiesMenu
{
private:
	Button *_remSpotButton;
	Button *_addSpotButton;
	Display *_display;
	SpotCollection *_spots;
	bool _isActive;

public:
	GlobalPropertiesMenu(Button *remSpotButton, Button *addSpotButton, Display *display, SpotCollection *spots);

	void Loop();

	bool IsActive();
	void Activate(bool immediateDisplay);
	void Deactivate();
};

#endif // GLOBAL_PROPERTIES_MENU_H