#include "GlobalPropertiesMenu.h"

#include "DiagLed.h"

GlobalPropertiesMenu::GlobalPropertiesMenu(Button *remSpotButton, Button *addSpotButton, Display *display, SpotCollection *spots)
	: _remSpotButton(remSpotButton),
	  _addSpotButton(addSpotButton),
	  _display(display),
	  _spots(spots)
{
}

void GlobalPropertiesMenu::Loop()
{
	if (!_isActive)
	{
		return;
	}

	if (_remSpotButton->IsClicked())
	{
		_spots->RemoveLast();
		_display->ShowGlobalProperties(_spots);
	}
	else if (_addSpotButton->IsClicked())
	{
		_spots->Add(0, 1000, LightActivity::A_1, 200, LightActivity::A_0);
		_display->ShowGlobalProperties(_spots);
	}
}

bool GlobalPropertiesMenu::IsActive()
{
	return _isActive;
}

void GlobalPropertiesMenu::Activate(bool immediateDisplay)
{
	_remSpotButton->ResetEnabled();
	_addSpotButton->ResetEnabled();

	_remSpotButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);
	_addSpotButton->ChangeDebounceDelay(Button_DebounceDelay_SlowButton);

	if (immediateDisplay)
	{
		_display->ShowGlobalProperties(_spots);
	}

	_isActive = true;
}

void GlobalPropertiesMenu::Deactivate()
{
	_isActive = false;
}